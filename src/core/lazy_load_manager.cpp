#include "lazy_load_manager.h"
#include <algorithm>

namespace {
	constexpr int MAX_BACKOFF_EXPONENT = 4;

	std::chrono::seconds RetryDelay(int failures) {
		return std::chrono::seconds(1 << std::min(failures - 1, MAX_BACKOFF_EXPONENT));
	}
} // namespace

LazyLoadManager::LazyLoadManager(Fetcher fetch, PolicySource policy, std::function<Clock::time_point()> now)
	: fetch_(std::move(fetch)), policy_(std::move(policy)), now_(std::move(now)) {}

void LazyLoadManager::RequestPlayerData(const std::string& account, const std::string& provider) {
	Key key {account, provider};
	LoadPolicy policy = policy_();
	{
		std::scoped_lock lock(mutex_);
		auto now = now_();
		auto& entry = entries_[key];
		if (entry.inFlight || entry.retryAt > now) return;
		if (entry.loaded && now - entry.loadedAt < policy.cacheTimeout) return;
		entry.inFlight = true;
	}
	// Fetch callbacks can run synchronously, so the lock must not be held here.
	fetch_(account, provider, [this, key, policy](const FetchResult& result) { OnFetched(key, result, policy); });
}

void LazyLoadManager::OnFetched(const Key& key, const FetchResult& result, const LoadPolicy& policy) {
	std::scoped_lock lock(mutex_);
	auto& entry = entries_[key];
	auto now = now_();
	entry.inFlight = false;

	if (result.status != FetchStatus::FAILED) {
		entry.proofs = result.proofs;
		entry.loaded = true;
		entry.loadedAt = now;
		entry.failures = 0;
		entry.retryAt = {};
		return;
	}

	if (++entry.failures >= policy.maxAttempts) {
		entry.failures = 0;
		entry.retryAt = now + policy.cacheTimeout;
	} else {
		entry.retryAt = now + RetryDelay(entry.failures);
	}
}

PlayerLoad LazyLoadManager::GetPlayerLoad(const std::string& account, const std::string& provider) {
	std::scoped_lock lock(mutex_);
	auto it = entries_.find({account, provider});
	if (it == entries_.end()) return {LoadState::LOADING, nullptr};
	const auto& entry = it->second;
	if (entry.loaded) return {LoadState::READY, entry.proofs};
	return {entry.inFlight ? LoadState::LOADING : LoadState::FAILED, nullptr};
}

void LazyLoadManager::PruneExpired() {
	std::scoped_lock lock(mutex_);
	auto now = now_();
	auto timeout = policy_().cacheTimeout;
	std::erase_if(entries_, [&](const auto& item) {
		const auto& entry = item.second;
		if (entry.inFlight) return false;
		auto lastActivity = std::max(entry.loadedAt, entry.retryAt);
		return now - lastActivity > timeout;
	});
}

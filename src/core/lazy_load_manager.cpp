#include "lazy_load_manager.h"
#include "../providers/common/provider_registry.h"
#include "settings.h"
#include "shared.h"
#include <algorithm>

// ProofCache Implementation
std::string ProofCache::MakeKey(const std::string& account, const std::string& provider) {
	return account + "|" + provider;
}

bool ProofCache::HasValidEntry(const std::string& key) {
	std::scoped_lock lock(cacheMutex);
	auto it = cache.find(key);
	return it != cache.end() && !IsExpired(it->second) && !it->second.permanentFailure;
}

std::unique_ptr<PlayerProofData> ProofCache::GetEntry(const std::string& key) {
	std::scoped_lock lock(cacheMutex);
	auto it = cache.find(key);
	if (it != cache.end() && !IsExpired(it->second) && it->second.data) {
		return std::make_unique<PlayerProofData>(*it->second.data);
	}
	return nullptr;
}

void ProofCache::SetEntry(const std::string& key, std::unique_ptr<PlayerProofData> data) {
	std::scoped_lock lock(cacheMutex);
	auto& entry = cache[key];
	entry.data = std::move(data);
	entry.loadTime = std::chrono::steady_clock::now();
	entry.retryCount = 0;
	entry.permanentFailure = false;
}

void ProofCache::MarkFailure(const std::string& key) {
	std::scoped_lock lock(cacheMutex);
	auto& entry = cache[key];
	entry.retryCount++;
	entry.nextRetryTime = std::chrono::steady_clock::now() + GetRetryDelay(entry.retryCount);

	if (entry.retryCount >= GetMaxRetries()) {
		entry.permanentFailure = true;
	}
}

bool ProofCache::ShouldRetry(const std::string& key) {
	std::scoped_lock lock(cacheMutex);
	auto it = cache.find(key);
	if (it == cache.end()) return true;

	const auto& entry = it->second;
	if (entry.permanentFailure) return false;
	if (entry.retryCount >= GetMaxRetries()) return false;

	return std::chrono::steady_clock::now() >= entry.nextRetryTime;
}

void ProofCache::CleanExpiredEntries() {
	std::scoped_lock lock(cacheMutex);
	auto it = cache.begin();
	while (it != cache.end()) {
		if (IsExpired(it->second)) {
			it = cache.erase(it);
		} else {
			++it;
		}
	}
}

void ProofCache::OnWindowOpened() {
	std::scoped_lock lock(cacheMutex);
	windowCurrentlyOpen = true;
}

void ProofCache::OnWindowClosed() {
	std::scoped_lock lock(cacheMutex);
	windowCurrentlyOpen = false;
	windowLastClosed = std::chrono::steady_clock::now();
}

bool ProofCache::IsExpired(const CacheEntry& entry) {
	auto now = std::chrono::steady_clock::now();
	auto cacheAge = now - entry.loadTime;

	auto timeout = GetCacheTimeout();
	if (windowCurrentlyOpen) {
		return cacheAge > timeout;
	} else {
		auto timeSinceWindowClosed = now - windowLastClosed;
		return cacheAge > timeout && timeSinceWindowClosed > timeout;
	}
}

std::chrono::minutes ProofCache::GetCacheTimeout() const {
	return std::chrono::minutes(Settings::CacheTimeoutMinutes);
}

int ProofCache::GetMaxRetries() const {
	return Settings::MaxRetryAttempts;
}

std::chrono::seconds ProofCache::GetRetryDelay(int retryCount) {
	return std::chrono::seconds(1 << (std::min) (retryCount - 1, 4)); // 1s, 2s, 4s, 8s, 16s
}

// LazyLoadManager Implementation
std::string LazyLoadManager::MakeKey(const std::string& account, const std::string& provider) {
	return account + "|" + provider;
}

void LazyLoadManager::OnWindowStateChanged(bool isOpen) {
	windowOpen = isOpen;
	if (isOpen) {
		cache.OnWindowOpened();
	} else {
		cache.OnWindowClosed();
	}
}

void LazyLoadManager::RequestPlayerData(const std::string& account, const std::string& provider) {
	if (!windowOpen) return;

	std::string key = MakeKey(account, provider);

	if (cache.HasValidEntry(key)) return;
	if (!cache.ShouldRetry(key)) return;

	{
		std::scoped_lock lock(pendingMutex);
		if (pendingLoads.count(key)) return;
		pendingLoads.insert(key);
	}

	// Use external threadpool (will be injected)
	if (loadFunction) {
		loadFunction(account, provider, key);
	}
}

LoadState LazyLoadManager::GetPlayerState(const std::string& account, const std::string& provider) {
	std::string key = MakeKey(account, provider);

	if (cache.HasValidEntry(key)) {
		return LoadState::READY;
	}

	{
		std::scoped_lock lock(pendingMutex);
		if (pendingLoads.count(key)) {
			return LoadState::LOADING;
		}
	}

	return LoadState::FAILED;
}

std::unique_ptr<PlayerProofData> LazyLoadManager::GetPlayerData(const std::string& account, const std::string& provider) {
	std::string key = MakeKey(account, provider);
	return cache.GetEntry(key);
}

void LazyLoadManager::CleanupExpiredEntries() {
	cache.CleanExpiredEntries();

	std::scoped_lock lock(pendingMutex);
	// Clear pending loads for expired entries
	auto it = pendingLoads.begin();
	while (it != pendingLoads.end()) {
		if (!cache.HasValidEntry(*it) && !cache.ShouldRetry(*it)) {
			it = pendingLoads.erase(it);
		} else {
			++it;
		}
	}
}

void LazyLoadManager::SetLoadFunction(void (*func)(const std::string&, const std::string&, const std::string&)) {
	loadFunction = func;
}

void LazyLoadManager::OnLoadComplete(const std::string& key, std::unique_ptr<PlayerProofData> data) {
	cache.SetEntry(key, std::move(data));
	std::scoped_lock lock(pendingMutex);
	pendingLoads.erase(key);
}

void LazyLoadManager::OnLoadFailed(const std::string& key) {
	cache.MarkFailure(key);
	std::scoped_lock lock(pendingMutex);
	pendingLoads.erase(key);
}
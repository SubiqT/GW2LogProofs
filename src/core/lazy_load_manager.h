#ifndef LAZY_LOAD_MANAGER_H
#define LAZY_LOAD_MANAGER_H

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include "../providers/common/provider.h"

enum class LoadState {
	LOADING,
	READY,
	FAILED
};

struct LoadPolicy {
	std::chrono::seconds cacheTimeout;
	int maxAttempts;
};

struct PlayerLoad {
	LoadState state;
	std::shared_ptr<const PlayerProofs> proofs; // Kept while a refresh is in flight; null if not found.
};

// Caches proofs per account and provider. Callers request data whenever they display it; a fetch only starts once the
// cached entry is older than the cache timeout, and failed fetches are retried with exponential backoff.
class LazyLoadManager {
public:
	using Clock = std::chrono::steady_clock;
	using Fetcher = std::function<void(const std::string& account, const std::string& provider, FetchCallback callback)>;
	// Called on the thread that requests data; fetch completions use the policy captured with their request.
	using PolicySource = std::function<LoadPolicy()>;

	LazyLoadManager(Fetcher fetch, PolicySource policy, std::function<Clock::time_point()> now = Clock::now);

	// Starts a fetch unless the cached entry is fresh, a fetch is in flight, or a retry is not yet due.
	void RequestPlayerData(const std::string& account, const std::string& provider);
	PlayerLoad GetPlayerLoad(const std::string& account, const std::string& provider);
	// Drops entries older than the cache timeout, such as those of players who have left.
	void PruneExpired();

private:
	using Key = std::pair<std::string, std::string>;

	struct Entry {
		std::shared_ptr<const PlayerProofs> proofs;
		bool loaded = false; // Also true for accounts the provider reported as not found.
		bool inFlight = false;
		Clock::time_point loadedAt;
		int failures = 0;
		Clock::time_point retryAt;
	};

	void OnFetched(const Key& key, const FetchResult& result, const LoadPolicy& policy);

	Fetcher fetch_;
	PolicySource policy_;
	std::function<Clock::time_point()> now_;
	std::mutex mutex_;
	std::map<Key, Entry> entries_;
};

#endif

#ifndef LAZY_LOAD_MANAGER_H
#define LAZY_LOAD_MANAGER_H

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "../providers/common/data_provider.h"
#include "player_data.h"


class ProofCache {
private:
	struct CacheEntry {
		std::unique_ptr<PlayerProofData> data;
		std::chrono::steady_clock::time_point loadTime;
		int retryCount = 0;
		std::chrono::steady_clock::time_point nextRetryTime;
		bool permanentFailure = false;
	};

	std::unordered_map<std::string, CacheEntry> cache;
	std::mutex cacheMutex;
	std::chrono::steady_clock::time_point windowLastClosed;
	bool windowCurrentlyOpen = false;

	std::chrono::minutes GetCacheTimeout() const;
	int GetMaxRetries() const;

public:
	bool HasValidEntry(const std::string& key);
	std::unique_ptr<PlayerProofData> GetEntry(const std::string& key);
	void SetEntry(const std::string& key, std::unique_ptr<PlayerProofData> data);
	void MarkFailure(const std::string& key);
	bool ShouldRetry(const std::string& key);
	std::vector<std::string> CleanExpiredEntries();
	void OnWindowOpened();
	void OnWindowClosed();

private:
	std::string MakeKey(const std::string& account, const std::string& provider);
	bool IsExpired(const CacheEntry& entry);
	std::chrono::seconds GetRetryDelay(int retryCount);
};

class LazyLoadManager {
private:
	ProofCache cache;
	std::set<std::string> pendingLoads;
	std::mutex pendingMutex;
	bool windowOpen = false;
	void (*loadFunction)(const std::string&, const std::string&, const std::string&) = nullptr;

public:
	void OnWindowStateChanged(bool isOpen);
	void RequestPlayerData(const std::string& account, const std::string& provider);
	LoadState GetPlayerState(const std::string& account, const std::string& provider);
	std::unique_ptr<PlayerProofData> GetPlayerData(const std::string& account, const std::string& provider);
	void CleanupExpiredEntries();
	void SetLoadFunction(void (*func)(const std::string&, const std::string&, const std::string&));
	void OnLoadComplete(const std::string& key, std::unique_ptr<PlayerProofData> data);
	void OnLoadFailed(const std::string& key);

private:
	std::string MakeKey(const std::string& account, const std::string& provider);
};

#endif
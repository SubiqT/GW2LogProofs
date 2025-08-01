#include "player_manager.h"
#include "settings.h"
#include "shared.h"
#include <format>
#include <queue>

namespace PlayerManager {
	std::vector<Player> players;
	std::string selfAccountName;
	std::mutex playerMutex;
	LazyLoadManager lazyLoadManager;
	static std::queue<std::pair<std::string, std::string>> loadQueue;

	static long long GetPlayerIndex(const std::string& account) {
		for (Player& p : players)
			if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));
		return -1;
	}

	static std::string GetCurrentProviderName() {
		return (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
	}

	void AddPlayer(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(playerMutex);
		for (const Player& p : players)
			if (p.account == playerInfo.account) return;

		players.emplace_back();
		players.back().id = playerInfo.id;
		players.back().account = playerInfo.account;
		players.back().state = ::LoadState::LOADING;
		players.back().proofData = nullptr;
		players.back().providerName = "";

		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added to tracking: {}", playerInfo.account).c_str());

		if (Settings::ShowWindowLogProofs) {
			lazyLoadManager.RequestPlayerData(playerInfo.account, GetCurrentProviderName());
		}
	}

	void RemovePlayer(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(playerMutex);
		long long index = GetPlayerIndex(playerInfo.account);
		if (index != -1) {
			players.erase(players.begin() + index);
			APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player removed from tracking: {}", playerInfo.account).c_str());
		}
	}

	void SetSelf(const PlayerInfo& playerInfo) {
		selfAccountName = playerInfo.account;
		AddPlayer(playerInfo);
	}

	void ClearPlayers() {
		std::scoped_lock lck(playerMutex);
		long long index = GetPlayerIndex(selfAccountName);
		if (index == -1) {
			players.clear();
		} else {
			Player self = std::move(players[index]);
			players.clear();
			players.emplace_back(std::move(self));
		}
	}

	static void ProcessLoadQueue() {
		if (loadQueue.empty()) return;

		// Count currently loading players
		int loadingCount = 0;
		std::scoped_lock lck(playerMutex);
		for (const auto& player : players) {
			auto state = lazyLoadManager.GetPlayerState(player.account, GetCurrentProviderName());
			if (state == LoadState::LOADING) loadingCount++;
		}

		// Start new requests to maintain up to MaxConcurrentRequests concurrent
		while (loadingCount < Settings::MaxConcurrentRequests && !loadQueue.empty()) {
			auto [account, provider] = loadQueue.front();
			loadQueue.pop();
			lazyLoadManager.RequestPlayerData(account, provider);
			loadingCount++;
		}
	}

	void OnWindowStateChanged(bool isOpen) {
		lazyLoadManager.OnWindowStateChanged(isOpen);
		if (isOpen) {
			std::string providerName = GetCurrentProviderName();
			std::scoped_lock lck(playerMutex);
			// Clear any existing queue
			while (!loadQueue.empty()) loadQueue.pop();
			// Load first MaxConcurrentRequests immediately, queue the rest
			int requestCount = 0;
			for (const auto& player : players) {
				if (requestCount < Settings::MaxConcurrentRequests) {
					lazyLoadManager.RequestPlayerData(player.account, providerName);
					requestCount++;
				} else {
					loadQueue.push({player.account, providerName});
				}
			}
		}
	}

	void ProcessPendingLoads() {
		ProcessLoadQueue();
	}
} // namespace PlayerManager
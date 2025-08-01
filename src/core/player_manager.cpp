#include "player_manager.h"
#include "settings.h"
#include "shared.h"
#include <format>

namespace PlayerManager {
	std::vector<Player> players;
	std::string selfAccountName;
	std::mutex playerMutex;
	LazyLoadManager lazyLoadManager;

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

		if (Settings::ShowWindowLogProofs) {
			lazyLoadManager.RequestPlayerData(playerInfo.account, GetCurrentProviderName());
		}
	}

	void RemovePlayer(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(playerMutex);
		long long index = GetPlayerIndex(playerInfo.account);
		if (index != -1) {
			players.erase(players.begin() + index);
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

	void OnWindowStateChanged(bool isOpen) {
		lazyLoadManager.OnWindowStateChanged(isOpen);
		if (isOpen) {
			std::string providerName = GetCurrentProviderName();
			std::scoped_lock lck(playerMutex);
			for (const auto& player : players) {
				lazyLoadManager.RequestPlayerData(player.account, providerName);
			}
		}
	}
} // namespace PlayerManager
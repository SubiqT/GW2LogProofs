#include <chrono>
#include <format>
#include <queue>
#include <vector>


#include "../arcdps/arcdps.h"

#include "../providers/common/provider_registry.h"
#include "../utils/threadpool.hpp"
#include "log_proofs.h"
#include "player_info.h"
#include "player_tracker_manager.h"
#include "settings.h"
#include "shared.h"


namespace LogProofs {
	std::vector<Player> players;
	std::string selfAccountName;
	bool unofficialExtrasEnabled = false; // Legacy flag for tracker availability

	Threadpool threadpool = Threadpool();
	std::mutex Mutex;
	LazyLoadManager lazyLoadManager;
	PlayerTrackerManager trackerManager;

	// Internal helper functions
	long long GetPlayerIndex(std::string account) {
		try {
			for (Player& p : players)
				if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to get player index for account {}: {}", account, e.what()).c_str());
		}
		return -1;
	}


	void LoadPlayerDataLazy(const std::string& account, const std::string& providerName, const std::string& key) {
		try {
			auto provider = ProviderRegistry::Instance().CreateProvider(providerName);
			if (!provider) {
				lazyLoadManager.OnLoadFailed(key);
				return;
			}

			PlayerProofData data = provider->LoadPlayerData(account);
			lazyLoadManager.OnLoadComplete(key, std::make_unique<PlayerProofData>(data));
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to load {} data for {}: {}", providerName, account, e.what()).c_str());
			lazyLoadManager.OnLoadFailed(key);
		}
	}

	void LoadPlayerData(const std::string& account, const std::string& providerName) {
		try {
			auto provider = ProviderRegistry::Instance().CreateProvider(providerName);
			if (!provider) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Provider not found: {}", providerName).c_str());
				return;
			}

			PlayerProofData data = provider->LoadPlayerData(account);

			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(account);
			if (index == -1) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Player not found when loading data: {}", account).c_str());
				return;
			}

			players[index].proofData = std::make_unique<PlayerProofData>(data);
			players[index].state = ::LoadState::READY;
			players[index].providerName = providerName;

			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Loaded {} data for {}", providerName, account).c_str());
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error loading {} data for {}: {}", providerName, account, e.what()).c_str());

			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(account);
			if (index != -1) {
				players[index].state = ::LoadState::FAILED;
			}
		}
	}


	void ClearPlayers() {
		try {
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(selfAccountName);
			if (index == -1) {
				players.clear();
				APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "Cleared all players (self not found)");
			} else {
				Player self;
				self.id = players[index].id;
				self.account = players[index].account;
				self.state = players[index].state;
				self.proofData = std::move(players[index].proofData);
				self.providerName = players[index].providerName;
				players.clear();
				players.emplace_back(std::move(self));
				APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "Cleared all players except self");
			}
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error clearing players: {}", e.what()).c_str());
			return;
		}
	}

	void ReloadAllPlayersWithProvider(const std::string& providerName) {
		std::scoped_lock lck(Mutex);
		for (auto& player : players) {
			if (player.providerName != providerName) {
				player.state = ::LoadState::LOADING;
				player.proofData = nullptr;
				player.providerName = "";
				std::string account = player.account;
				threadpool.spawn([account, providerName]() { LoadPlayerData(account, providerName); return nullptr; });
			}
		}
	}

	void ReloadKpmePlayersForLinkedAccounts() {
		std::scoped_lock lck(Mutex);
		for (auto& player : players) {
			if (player.providerName == "KPME") {
				player.state = ::LoadState::LOADING;
				player.proofData = nullptr;
				std::string account = player.account;
				threadpool.spawn([account]() { LoadPlayerData(account, "KPME"); return nullptr; });
			}
		}
	}

	void OnWindowStateChanged(bool isOpen) {
		lazyLoadManager.OnWindowStateChanged(isOpen);
		if (isOpen) {
			// Trigger loading for all players when window opens
			std::string providerName = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
			std::scoped_lock lck(Mutex);
			for (const auto& player : players) {
				lazyLoadManager.RequestPlayerData(player.account, providerName);
			}
		}
	}

	// Utility function for account name processing
	std::string StripAccount(std::string account) {
		if (!account.empty() && account.at(0) == ':') {
			return account.erase(0, 1);
		}
		return account;
	}

	// Helper functions for tracker integration
	void AddPlayerFromTracker(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(Mutex);
		for (const Player& p : players)
			if (p.account == playerInfo.account) return; // skip already added players

		players.emplace_back();
		players.back().id = playerInfo.id;
		players.back().account = playerInfo.account;
		players.back().state = ::LoadState::LOADING;
		players.back().proofData = nullptr;
		players.back().providerName = "";

		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added from {}: {}", playerInfo.source, playerInfo.account).c_str());

		// Trigger lazy loading if window is currently open
		if (Settings::ShowWindowLogProofs) {
			std::string providerName = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
			lazyLoadManager.RequestPlayerData(playerInfo.account, providerName);
		}
	}

	void RemovePlayerFromTracker(const PlayerInfo& playerInfo) {
		std::scoped_lock lck(Mutex);
		long long index = GetPlayerIndex(playerInfo.account);
		if (index == -1) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Player not found for removal: {}", playerInfo.account).c_str());
			return;
		}
		players.erase(players.begin() + index);
		APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player removed from {}: {}", playerInfo.source, playerInfo.account).c_str());
	}

	void SetSelfFromTracker(const PlayerInfo& playerInfo) {
		selfAccountName = playerInfo.account;
		APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Self detected from {}: {}", playerInfo.source, playerInfo.account).c_str());
		AddPlayerFromTracker(playerInfo);
	}

	void InitializeTrackerManager() {
		trackerManager.Initialize();
	}

	void ShutdownTrackerManager() {
		trackerManager.Shutdown();
	}

	// Event handlers - route through tracker manager
	void UnExSquadEventHandler(void* eventArgs) {
		try {
			SquadUpdate* squadUpdate = (SquadUpdate*) eventArgs;
			unofficialExtrasEnabled = true;
			for (size_t i = 0; i < squadUpdate->UsersCount; i++) {
				if (!squadUpdate) return;
				if (!squadUpdate->UserInfo) return;
				if (!squadUpdate->UserInfo[i].AccountName) return;
				std::string account = StripAccount(squadUpdate->UserInfo[i].AccountName);
				int role = int(squadUpdate->UserInfo[i].Role);
				APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Unofficial Extras event - account: {}, role: {}", account, role).c_str());

				PlayerInfo playerInfo;
				playerInfo.account = account;
				playerInfo.id = 0;
				playerInfo.source = "Unofficial Extras";
				playerInfo.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

				if (role == 5) {
					APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player left squad: {} (Unofficial Extras)", account).c_str());
					if (selfAccountName == account) {
						trackerManager.HandleSquadClear();
					} else {
						trackerManager.HandlePlayerLeave(playerInfo);
					}
				}
				if (role <= 2) {
					APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player joined squad: {} (Unofficial Extras)", account).c_str());
					trackerManager.HandlePlayerJoin(playerInfo);
				}
			}
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in Unofficial Extras event handler: {}", e.what()).c_str());
		}
	}

	void ArcSquadJoinEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			std::string accountName = StripAccount(evAgentUpdateData->account);
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player joined squad: {} (ArcDPS)", accountName).c_str());

			PlayerInfo playerInfo;
			playerInfo.account = accountName;
			playerInfo.character = evAgentUpdateData->character;
			playerInfo.id = evAgentUpdateData->id;
			playerInfo.instanceId = static_cast<uint32_t>(evAgentUpdateData->instanceId);
			playerInfo.source = "ArcDPS";
			playerInfo.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			trackerManager.HandlePlayerJoin(playerInfo);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS squad join handler: {}", e.what()).c_str());
		}
	}

	void ArcSquadLeaveEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player left squad: ID {} (ArcDPS)", evAgentUpdateData->id).c_str());

			PlayerInfo playerInfo;
			playerInfo.account = StripAccount(evAgentUpdateData->account);
			playerInfo.id = evAgentUpdateData->id;
			playerInfo.source = "ArcDPS";

			trackerManager.HandlePlayerLeave(playerInfo);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS squad leave handler: {}", e.what()).c_str());
		}
	}

	void ArcSelfLeaveEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Self left squad: {} (ArcDPS)", selfAccountName).c_str());
			trackerManager.HandleSquadClear();
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS self leave handler: {}", e.what()).c_str());
		}
	}

	void ArcSelfDetectedEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			std::string accountName = StripAccount(evAgentUpdateData->account);
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Self detected: {} (ID: {}, ArcDPS)", accountName, evAgentUpdateData->id).c_str());

			PlayerInfo playerInfo;
			playerInfo.account = accountName;
			playerInfo.character = evAgentUpdateData->character;
			playerInfo.id = evAgentUpdateData->id;
			playerInfo.instanceId = static_cast<uint32_t>(evAgentUpdateData->instanceId);
			playerInfo.source = "ArcDPS";
			playerInfo.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			trackerManager.HandleSelfDetected(playerInfo);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS self detection handler: {}", e.what()).c_str());
		}
	}
} // namespace LogProofs
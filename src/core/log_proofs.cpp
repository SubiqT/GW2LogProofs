#include <format>
#include <queue>
#include <vector>

#include "../arcdps/arcdps.h"

#include "log_proofs.h"
#include "shared.h"
#include "../utils/threadpool.hpp"
#include "../providers/common/provider_registry.h"
#include "settings.h"

namespace LogProofs {
	std::vector<Player> players;
	std::string selfAccountName;
	bool unofficalExtrasEnabled = false;

	Threadpool threadpool = Threadpool();
	std::mutex Mutex;

	long long GetPlayerIndex(std::string account) {
		try {
			for (Player& p : players)
				if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));
			;
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("tried to get index of player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
		}
		return -1;
	}

	long long GetPlayerIndex(uintptr_t id) {
		try {
			for (Player& p : players)
				if (p.id == id) return std::addressof(p) - std::addressof(players.at(0));
			;
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("tried to get index of player with id {} but an error occurred. exception details: {}", id, e.what()).c_str());
		}
		return -1;
	}



	void LoadPlayerData(const std::string& account, const std::string& providerName) {
		try {
			auto provider = ProviderRegistry::Instance().CreateProvider(providerName);
			if (!provider) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Provider {} not found", providerName).c_str());
				return;
			}
			
			PlayerProofData data = provider->LoadPlayerData(account);
			
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(account);
			if (index == -1) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Player {} not found when loading data", account).c_str());
				return;
			}
			
			players[index].proofData = std::make_unique<PlayerProofData>(data);
			players[index].state = ::LoadState::READY;
			players[index].providerName = providerName;
			
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Loaded {} data for player: {}", providerName, account).c_str());
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Error loading {} data for {}: {}", providerName, account, e.what()).c_str());
			
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(account);
			if (index != -1) {
				players[index].state = ::LoadState::FAILED;
			}
		}
	}

	void AddPlayer(std::string account) {
		if (!unofficalExtrasEnabled) return;
		try {
			std::scoped_lock lck(Mutex);
			for (const Player& p : players)
				if (p.account == account) return; // skip already added players
			players.emplace_back();
			players.back().id = 0;
			players.back().account = account;
			players.back().state = ::LoadState::LOADING;
			players.back().proofData = nullptr;
			players.back().providerName = "";
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("tried to add player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
			return;
		}
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player to log proofs table with account {}", account).c_str());
		std::string providerName = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
		threadpool.spawn([=]() { LoadPlayerData(account, providerName); return nullptr; });
	}

	void AddPlayer(uintptr_t id, std::string account) {
		try {
			std::scoped_lock lck(Mutex);
			for (const Player& p : players)
				if (p.account == account) return; // skip already added players
			players.emplace_back();
			players.back().id = id;
			players.back().account = account;
			players.back().state = ::LoadState::LOADING;
			players.back().proofData = nullptr;
			players.back().providerName = "";
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("tried to add player with id {} and account {} but an error occurred. exception details: {}", id, account, e.what()).c_str());
			return;
		}
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player to log proofs table with id {} and account {}", id, account).c_str());
		std::string providerName = (Settings::SelectedDataSource == WINGMAN) ? "Wingman" : "KPME";
		threadpool.spawn([=]() { LoadPlayerData(account, providerName); return nullptr; });
	}

	void RemovePlayer(std::string account) {
		try {
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(account);
			if (index == -1) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("tried to remove player with account {} but player was not found in players vector.", account).c_str());
				return;
			}
			players.erase(players.begin() + index);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("tried to remove player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
			return;
		}
		APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("removed player from log proofs table: {}", account).c_str());
	}

	void RemovePlayer(uintptr_t id) {
		try {
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(id);
			if (index == -1) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("tried to remove player with id {} but player was not found in players vector.", id).c_str());
				return;
			}
			players.erase(players.begin() + index);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("tried to remove player with id {} but an error occurred. exception details: {}", id, e.what()).c_str());
			return;
		}
		APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("removed player from log proofs table: {}", id).c_str());
	}

	void ClearPlayers() {
		try {
			std::scoped_lock lck(Mutex);
			long long index = GetPlayerIndex(selfAccountName);
			if (index == -1) {
				players.clear();
				APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "removed all players from log proofs table (self was not found).");
			} else {
				Player self;
				self.id = players[index].id;
				self.account = players[index].account;
				self.state = players[index].state;
				self.proofData = std::move(players[index].proofData);
				self.providerName = players[index].providerName;
				players.clear();
				players.emplace_back(std::move(self));
				APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "removed all players from log proofs table (except for self).");
			}
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("tried to remove all players but an error occurred. exception details: {}", e.what()).c_str());
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

	std::string StripAccount(std::string account) {
		if (!account.empty()) {
			if (account.at(0) == ':') {
				return account.erase(0, 1); // the game prefixes accounts with :
			}
		}
		return account;
	}

	/* Unofficial Extras */
	void UnExSquadEventHandler(void* eventArgs) {
		try {
			SquadUpdate* squadUpdate = (SquadUpdate*) eventArgs;
			unofficalExtrasEnabled = true;
			for (size_t i = 0; i < squadUpdate->UsersCount; i++) {
				if (!squadUpdate) return;
				if (!squadUpdate->UserInfo) return;
				if (!squadUpdate->UserInfo[i].AccountName) return;
				std::string account = StripAccount(squadUpdate->UserInfo[i].AccountName);
				int role = int(squadUpdate->UserInfo[i].Role);
				APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("unex: received event for account: {} - role: {}", account, role).c_str());
				if (role == 5) {
					APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("unex: {} has left the squad or party", account).c_str());
					if (selfAccountName == account) ClearPlayers();
					else
						RemovePlayer(account);
				}
				if (role <= 2) {
					APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("unex: detected player with account {} in squad or party", account).c_str());
					AddPlayer(account);
				}
			}
		} catch (const std::exception e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in UnExSquadEventHandler: \n{}", e.what()).c_str());
		}
	}

	/* Arcdps */
	void ArcSquadJoinEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			std::string accountName = StripAccount(evAgentUpdateData->account);
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: detected player with account {} in squad or party", accountName).c_str());
			AddPlayer(evAgentUpdateData->id, accountName);
		} catch (const std::exception e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSquadJoinEventHandler: \n{}", e.what()).c_str());
		}
	}

	void ArcSquadLeaveEventHandler(void* eventArgs) {
		try {
			if (unofficalExtrasEnabled) return;
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: {} has left the squad or party", evAgentUpdateData->id).c_str());
			RemovePlayer(evAgentUpdateData->id);
		} catch (const std::exception e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSquadLeaveEventHandler: \n{}", e.what()).c_str());
		}
	}

	void ArcSelfLeaveEventHandler(void* eventArgs) {
		try {
			if (unofficalExtrasEnabled) return;
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: {} has left the squad or party", selfAccountName).c_str());
			ClearPlayers();
		} catch (const std::exception e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSelfLeaveEventHandler: \n{}", e.what()).c_str());
		}
	}

	void ArcSelfDetectedEventHandler(void* eventArgs) {
		try {
			EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*) eventArgs;
			selfAccountName = StripAccount(evAgentUpdateData->account);
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: self detected with id {} account: {}", evAgentUpdateData->id, selfAccountName).c_str());
			AddPlayer(evAgentUpdateData->id, selfAccountName);
		} catch (const std::exception e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSelfDetectedEventHandler: \n{}", e.what()).c_str());
		}
	}
} // namespace LogProofs
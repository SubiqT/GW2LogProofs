#include <format>
#include <vector>
#include <queue>

#include "arcdps/arcdps.h"

#include "log_proofs.h"
#include "shared.h"
#include "threadpool.hpp"
#include "kp_loader.h"

namespace LogProofs {
	std::vector<Player> players;
	std::string selfAccountName;
    bool unofficalExtrasEnabled = false;

	Threadpool threadpool = Threadpool();
	std::mutex Mutex;

    long long GetPlayerIndex(std::string account) {
        try {
            for (Player& p : players) if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));;
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                "tried to get index of player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
        }
        return -1;
    }

    long long GetPlayerIndex(uintptr_t id) {
        try {
            for (Player& p : players) if (p.id == id) return std::addressof(p) - std::addressof(players.at(0));;
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                "tried to get index of player with id {} but an error occurred. exception details: {}", id, e.what()).c_str());
        }
        return -1;
    }

    void LoadWingmanKillProofs(std::string account) {
        try {
            Wingman::WingmanResponse res = Wingman::GetKp(account);
            std::scoped_lock lck(Mutex);
            long long index = GetPlayerIndex(account);
            if (index == -1) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to load wingman for player with account {} but player was not found in players vector.", account).c_str());
                return;
            }
            if (players[index].wingmanState != LOADING) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to load wingman for player with account {} but player was not in the loading state.", account).c_str());
                return;
            }
            players.at(index).wingman = res;
            players.at(index).wingmanState = READY;
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("loaded wingman for player: {}", account).c_str());
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to load wingman for player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
    }

    void LoadKpmeKillProofs(std::string account) {
        try {
            Kpme::KpmeResponse res = Kpme::GetKp(account);
            std::scoped_lock lck(Mutex);
            long long index = GetPlayerIndex(account);
            if (index == -1) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to load kpme for player with account {} but player was not found in players vector.", account).c_str());
                return;
            }
            if (players[index].kpmeState != LOADING) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to load kpme for player with account {} but player was not in the loading state.", account).c_str());
                return;
            }
            players.at(index).kpme = res;
            players.at(index).kpmeState = READY;
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("loaded kpme for player: {}", account).c_str());
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to load kpme for player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
    }

    void AddPlayer(std::string account) {
        if (!unofficalExtrasEnabled) return;
        try {
            std::scoped_lock lck(Mutex);
            for (Player p : players) if (p.account == account) return;  // skip already added players
            players.push_back({ 0, account, LOADING, LOADING });
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to add player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player to log proofs table with account {}", account).c_str());
        threadpool.spawn([=]() { LoadWingmanKillProofs(account); return nullptr; });
        threadpool.spawn([=]() { LoadKpmeKillProofs(account); return nullptr; });
    }

    void AddPlayer(uintptr_t id, std::string account) {
        try {
            std::scoped_lock lck(Mutex);
            for (Player p : players) if (p.account == account) return;  // skip already added players
            players.push_back({ id, account, LOADING, LOADING });
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to add player with id {} and account {} but an error occurred. exception details: {}", id, account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player to log proofs table with id {} and account {}", id, account).c_str());
        threadpool.spawn([=]() { LoadWingmanKillProofs(account); return nullptr; });
        threadpool.spawn([=]() { LoadKpmeKillProofs(account); return nullptr; });
    }

    void RemovePlayer(std::string account) {
        try {
            std::scoped_lock lck(Mutex);
            long long index = GetPlayerIndex(account);
            if (index == -1) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to remove player with account {} but player was not found in players vector.", account).c_str());
                return;
            }
            players.erase(players.begin() + index);
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to remove player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("removed player from log proofs table: {}", account).c_str());
    }

    void RemovePlayer(uintptr_t id) {
        try {
            std::scoped_lock lck(Mutex);
            long long index = GetPlayerIndex(id);
            if (index == -1) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to remove player with id {} but player was not found in players vector.", id).c_str());
                return;
            }
            players.erase(players.begin() + index);
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to remove player with id {} but an error occurred. exception details: {}", id, e.what()).c_str());
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
            }
            else {
                Player self = players[index];
                players.clear();
                players.push_back(self);
                APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "removed all players from log proofs table (except for self).");
            }
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to remove all players but an error occurred. exception details: {}", e.what()).c_str());
            return;
        }
    }

    std::string StripAccount(std::string account) {
        if (!account.empty()) {
            if (account.at(0) == ':') {
                return account.erase(0, 1);  // the game prefixes accounts with :
            }
        }
        return account;
    }

    /* Unofficial Extras */
    void UnExSquadEventHandler(void* eventArgs) {
        try {
            SquadUpdate* squadUpdate = (SquadUpdate*)eventArgs;
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
                    else RemovePlayer(account);
                }
                if (role <= 2) {
                    APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("unex: detected player with account {} in squad or party", account).c_str());
                    AddPlayer(account);
                }
            }
        }
        catch (const std::exception e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in UnExSquadEventHandler: \n{}", e.what()).c_str());
        }
    }

    /* Arcdps */
    void ArcSquadJoinEventHandler(void* eventArgs) {
        try {
            EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*)eventArgs;
            std::string accountName = StripAccount(evAgentUpdateData->account);
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: detected player with account {} in squad or party", accountName).c_str());
            AddPlayer(evAgentUpdateData->id, accountName);
        }
        catch (const std::exception e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSquadJoinEventHandler: \n{}", e.what()).c_str());
        }
    }

    void ArcSquadLeaveEventHandler(void* eventArgs) {
        try {
            if (unofficalExtrasEnabled) return;
            EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*)eventArgs;
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: {} has left the squad or party", evAgentUpdateData->id).c_str());
            RemovePlayer(evAgentUpdateData->id);
        }
        catch (const std::exception e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSquadLeaveEventHandler: \n{}", e.what()).c_str());
        }
    }

    void ArcSelfLeaveEventHandler(void* eventArgs) {
        try {
            if (unofficalExtrasEnabled) return;
            EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*)eventArgs;
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: {} has left the squad or party", selfAccountName).c_str());
            ClearPlayers();
        }
        catch (const std::exception e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSelfLeaveEventHandler: \n{}", e.what()).c_str());
        }
    }

    void ArcSelfDetectedEventHandler(void* eventArgs) {
        try {
            EvAgentUpdateData* evAgentUpdateData = (EvAgentUpdateData*)eventArgs;
            selfAccountName = StripAccount(evAgentUpdateData->account);
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("arc: self detected with id {} account: {}", evAgentUpdateData->id, selfAccountName).c_str());
            AddPlayer(evAgentUpdateData->id, selfAccountName);
        }
        catch (const std::exception e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An error occurred in ArcSelfDetectedEventHandler: \n{}", e.what()).c_str());
        }
    }
}
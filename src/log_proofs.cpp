#include <format>
#include <vector>
#include <queue>

#include "arcdps/arcdps.h"

#include "log_proofs.h"
#include "shared.h"
#include "threadpool.hpp"
#include "wingman.h"

namespace LogProofs {
	std::vector<Player> players;
	std::string selfName;

	Threadpool threadpool = Threadpool();
	std::mutex Mutex;

    long long GetPlayerIndex(std::string account) {
        try {
            std::scoped_lock lck(Mutex);
            for (Player& p : players) if (p.account == account) return std::addressof(p) - std::addressof(players.at(0));;
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                "tried to get index of player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
        }
        return -1;
    }

    void LoadKillProofs(std::string account) {
        Wingman::KillProofsResponse wingmanResponse = Wingman::GetKillProofs(account);

        std::map<std::string, int> kp;
        for (const auto& outer : wingmanResponse.kp) {
            for (const auto& inner : outer.second) {
                if (inner.first == "total") {
                    kp.insert({ outer.first, inner.second });  // { boss_id, total_kills }
                }
            }
        }
        long long index = GetPlayerIndex(account);
        if (index == -1) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                "tried to load kp for player with account {} but player was not found in players vector.", account).c_str());
            return;
        }
        try {
            std::scoped_lock lck(Mutex);
            if (players[index].state != LOADING) {
                APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                    "tried to load kp for player with account {} but player was not in the loading state.", account).c_str());
                return;
            }
            players.at(index).kp = kp;
            players.at(index).state = READY;
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to load kp for player with account {} but a subscript out of bounds error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("loaded kp for player: {}", account).c_str());
    }

    void AddPlayer(std::string account) {
        try {
            std::scoped_lock lck(Mutex);
            for (Player p : players) if (p.account == account) return;  // skip already added players
            players.push_back({ account, LOADING });
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to add player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player to log proofs table: {}", account).c_str());
        threadpool.spawn([=]() { LoadKillProofs(account); return nullptr; });
    }

    void RemovePlayer(std::string account) {
        long long index = GetPlayerIndex(account);
        if (index == -1) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format(
                "tried to remove player with account {} but player was not found in players vector.", account).c_str());
            return;
        }
        try {
            std::scoped_lock lck(Mutex);
            players.erase(players.begin() + index);
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format(
                "tried to remove player with account {} but an error occurred. exception details: {}", account, e.what()).c_str());
            return;
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("removed player from log proofs table: {}", account).c_str());
    }

    void ClearPlayers() {
        long long index = GetPlayerIndex(selfName);
        try {
            std::scoped_lock lck(Mutex);
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
        if (account.at(0) == ':') account.erase(0, 1);  // the game prefixes accounts with :
        return account;
    }

    /* Unofficial Extras */
    void SquadEventHandler(void* eventArgs) {
        SquadUpdate* squadUpdate = (SquadUpdate*)eventArgs;
        std::string account = StripAccount(squadUpdate->UserInfo->AccountName);
        int role = int(squadUpdate->UserInfo->Role);
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("received squad event for account: {} - role: {}", account, role).c_str());
        if (role == 5) {
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("{} has left the squad or party", account, role).c_str());
            if (selfName == account) ClearPlayers();
            else RemovePlayer(account);
        }
        if (role <= 2) {
            APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("{} has joined the squad or party", account, role).c_str());
            AddPlayer(account);
        }
    }

    /* Arcdps */
    void SelfAccountNameHandler(void* eventArgs) {
        const char* selfAccountName = (const char*)eventArgs;
        selfName = StripAccount(selfAccountName);
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("self account was detected: {}", selfName).c_str());
        AddPlayer(selfName);
    }
}
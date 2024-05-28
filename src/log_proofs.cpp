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
        std::scoped_lock lck(Mutex);
        if (players.size() > 0) {
            for (Player& p : players) if (p.account == account) return std::addressof(p) - std::addressof(players[0]);;
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
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("player with account {} was not found in players vector.", account).c_str());
            return;
        }
        {
            std::scoped_lock lck(Mutex);
            if (players[index].state != LOADING) {
                APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("player with account {} is not in the loading state.", account).c_str());
                return;
            }
            players[index].kp = kp;
            players[index].state = READY;
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("loaded kp for player: {}", account).c_str());
    }

    void AddPlayer(std::string account) {
        {
            std::scoped_lock lck(Mutex);
            for (Player p : players) if (p.account == account) return;  // skip already added players
            players.push_back({ account, LOADING });
        }
        threadpool.spawn([&]() { LoadKillProofs(account); return nullptr; });
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("added player: {}", account).c_str());
    }

    void RemovePlayer(std::string account) {
        long long index = GetPlayerIndex(account);
        if (index == -1) {
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("player with account {} was not found in players vector.", account).c_str());
            return;
        }
        {
            std::scoped_lock lck(Mutex);
            players.erase(players.begin() + index);
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("removed player: {}", account).c_str());
    }

    void ClearPlayers() {
        long long index = GetPlayerIndex(selfName);
        {
            std::scoped_lock lck(Mutex);
            if (index == -1) {
                players.clear();
            }
            else {
                Player self = players[index];
                players.clear();
                players.push_back(self);
            }
        }
        APIDefs->Log(ELogLevel_INFO, ADDON_NAME, "cleared players");
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
            if (selfName == account) {
                ClearPlayers();
                APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("{} has left the squad or party", account, role).c_str());
            }
            else {
                RemovePlayer(account);
                APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("{} has left the squad or party", account, role).c_str());
            }
        }
        if (role <= 2) {
            AddPlayer(account);
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("{} has joined the squad or party", account, role).c_str());
        }
    }

    /* Arcdps */
    void CombatEventHandler(void* eventArgs) {
        EvCombatData* cbtEvent = (EvCombatData*)eventArgs;
        if (!selfName.empty()) return;
        if (cbtEvent->ev) return;
        if (cbtEvent->src->elite) return;
        if (!cbtEvent->src->prof) return;
        if (cbtEvent->src->name == nullptr || cbtEvent->src->name[0] == '\0' || cbtEvent->dst->name == nullptr || cbtEvent->dst->name[0] == '\0') return;
        if (cbtEvent->dst->self) { // Should only occur on map change
            selfName = StripAccount(std::string(cbtEvent->dst->name));
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("detected self: {}", selfName).c_str());
            AddPlayer(selfName);
        }
    }
}
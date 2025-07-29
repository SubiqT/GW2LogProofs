#ifndef LOG_PROOFS_H
#define LOG_PROOFS_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "../unofficial_extras/Definitions.h"

#include "../providers/common/provider_registry.h"
#include "../utils/threadpool.hpp"
#include "player_data.h"

namespace LogProofs {
	// Legacy compatibility - will be removed after full migration
	enum KpState {
		LOADING = 0,
		READY = 1
	};

	// Use generic Player struct from player_data.h
	using Player = ::Player;

	extern std::vector<Player> players;
	extern std::string selfAccountName;
	extern std::mutex Mutex;
	extern Threadpool threadpool;

	void UnExSquadEventHandler(void* eventArgs);
	void ArcSquadJoinEventHandler(void* eventArgs);
	void ArcSquadLeaveEventHandler(void* eventArgs);
	void ArcSelfLeaveEventHandler(void* eventArgs);
	void ArcSelfDetectedEventHandler(void* eventArgs);

	// Generic provider-based loading
	void LoadPlayerData(const std::string& account, const std::string& providerName);
	void ReloadAllPlayersWithProvider(const std::string& providerName);
	void ReloadKpmePlayersForLinkedAccounts();
} // namespace LogProofs

/* Unofficial Extras */
struct SquadUpdate {
	UserInfo* UserInfo;
	uint64_t UsersCount;
};

#endif
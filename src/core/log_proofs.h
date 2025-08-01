#ifndef LOG_PROOFS_H
#define LOG_PROOFS_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "../unofficial_extras/Definitions.h"

#include "../providers/common/provider_registry.h"
#include "../utils/threadpool.hpp"
#include "lazy_load_manager.h"
#include "player_data.h"
#include "player_info.h"
#include "player_tracker_manager.h"



namespace LogProofs {
	// Legacy compatibility
	enum KpState {
		LOADING = 0,
		READY = 1
	};

	using Player = ::Player;

	extern std::vector<Player> players;
	extern std::string selfAccountName;
	extern bool unofficalExtrasEnabled;
	extern std::mutex Mutex;
	extern Threadpool threadpool;
	extern LazyLoadManager lazyLoadManager;
	extern PlayerTrackerManager trackerManager;

	// Event handlers
	void UnExSquadEventHandler(void* eventArgs);
	void ArcSquadJoinEventHandler(void* eventArgs);
	void ArcSquadLeaveEventHandler(void* eventArgs);
	void ArcSelfLeaveEventHandler(void* eventArgs);
	void ArcSelfDetectedEventHandler(void* eventArgs);

	// Generic provider-based loading
	void LoadPlayerData(const std::string& account, const std::string& providerName);
	void LoadPlayerDataLazy(const std::string& account, const std::string& providerName, const std::string& key);
	void ReloadAllPlayersWithProvider(const std::string& providerName);
	void ReloadKpmePlayersForLinkedAccounts();
	void OnWindowStateChanged(bool isOpen);

	// Tracker integration functions
	void AddPlayerFromTracker(const PlayerInfo& playerInfo);
	void RemovePlayerFromTracker(const PlayerInfo& playerInfo);
	void SetSelfFromTracker(const PlayerInfo& playerInfo);
	void ClearPlayers();
	void InitializeTrackerManager();
	void ShutdownTrackerManager();
} // namespace LogProofs

/* Unofficial Extras */
struct SquadUpdate {
	UserInfo* UserInfo;
	uint64_t UsersCount;
};

#endif
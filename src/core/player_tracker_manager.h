#ifndef PLAYER_TRACKER_MANAGER_H
#define PLAYER_TRACKER_MANAGER_H

#include "player_info.h"
#include "player_tracker.h"
#include <memory>
#include <mutex>
#include <vector>

class PlayerTrackerManager {
private:
	std::vector<std::unique_ptr<IPlayerTracker>> trackers;
	IPlayerTracker* activeTracker = nullptr;
	mutable std::mutex trackerMutex;

public:
	void RegisterTracker(std::unique_ptr<IPlayerTracker> tracker);
	void UpdateActiveTracker();
	IPlayerTracker* GetActiveTracker() const;
	void HandlePlayerJoin(const PlayerInfo& player);
	void HandlePlayerLeave(const PlayerInfo& player);
	void HandleSelfDetected(const PlayerInfo& self);
	void HandleSquadClear();
	void Initialize();
	void Shutdown();
};

#endif
#include "player_tracker_manager.h"
#include "shared.h"
#include <algorithm>
#include <format>

void PlayerTrackerManager::RegisterTracker(std::unique_ptr<IPlayerTracker> tracker) {
	std::scoped_lock lock(trackerMutex);
	trackers.push_back(std::move(tracker));
	UpdateActiveTracker();
}

void PlayerTrackerManager::UpdateActiveTracker() {
	IPlayerTracker* newActiveTracker = nullptr;
	int highestPriority = -1;

	for (const auto& tracker : trackers) {
		if (tracker->IsAvailable() && tracker->GetPriority() > highestPriority) {
			highestPriority = tracker->GetPriority();
			newActiveTracker = tracker.get();
		}
	}

	if (newActiveTracker != activeTracker) {
		if (activeTracker && newActiveTracker) {
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player tracker switched: {} -> {}", activeTracker->GetName(), newActiveTracker->GetName()).c_str());
		} else if (newActiveTracker) {
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player tracker activated: {}", newActiveTracker->GetName()).c_str());
		} else if (activeTracker) {
			APIDefs->Log(ELogLevel_INFO, ADDON_NAME, std::format("Player tracker deactivated: {}", activeTracker->GetName()).c_str());
		}
		activeTracker = newActiveTracker;
	}
}

IPlayerTracker* PlayerTrackerManager::GetActiveTracker() const {
	std::scoped_lock lock(trackerMutex);
	return activeTracker;
}

void PlayerTrackerManager::HandlePlayerJoin(const PlayerInfo& player) {
	std::scoped_lock lock(trackerMutex);
	UpdateActiveTracker();
	if (activeTracker) {
		activeTracker->OnPlayerJoin(player);
	}
}

void PlayerTrackerManager::HandlePlayerLeave(const PlayerInfo& player) {
	std::scoped_lock lock(trackerMutex);
	UpdateActiveTracker();
	if (activeTracker) {
		activeTracker->OnPlayerLeave(player);
	}
}

void PlayerTrackerManager::HandleSelfDetected(const PlayerInfo& self) {
	std::scoped_lock lock(trackerMutex);
	UpdateActiveTracker();
	if (activeTracker) {
		activeTracker->OnSelfDetected(self);
	}
}

void PlayerTrackerManager::HandleSquadClear() {
	std::scoped_lock lock(trackerMutex);
	UpdateActiveTracker();
	if (activeTracker) {
		activeTracker->OnSquadClear();
	}
}

void PlayerTrackerManager::Initialize() {
	std::scoped_lock lock(trackerMutex);
	for (const auto& tracker : trackers) {
		tracker->Initialize();
	}
	UpdateActiveTracker();
}

void PlayerTrackerManager::Shutdown() {
	std::scoped_lock lock(trackerMutex);
	for (const auto& tracker : trackers) {
		tracker->Shutdown();
	}
	activeTracker = nullptr;
}
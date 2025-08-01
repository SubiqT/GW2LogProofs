#include "arcdps_tracker.h"
#include "../core/player_manager.h"

extern bool unofficialExtrasEnabled;

bool ArcdpsTracker::IsAvailable() const {
	return !unofficialExtrasEnabled;
}

void ArcdpsTracker::Initialize() {}
void ArcdpsTracker::Shutdown() {}

void ArcdpsTracker::OnPlayerJoin(const PlayerInfo& player) {
	PlayerManager::AddPlayer(player);
}

void ArcdpsTracker::OnPlayerLeave(const PlayerInfo& player) {
	PlayerManager::RemovePlayer(player);
}

void ArcdpsTracker::OnSelfDetected(const PlayerInfo& self) {
	PlayerManager::SetSelf(self);
}

void ArcdpsTracker::OnSquadClear() {
	PlayerManager::ClearPlayers();
}
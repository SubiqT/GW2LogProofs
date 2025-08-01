#include "unofficial_extras_tracker.h"
#include "../core/player_manager.h"

extern bool unofficialExtrasEnabled;

bool UnofficialExtrasTracker::IsAvailable() const {
	return unofficialExtrasEnabled;
}

void UnofficialExtrasTracker::Initialize() {}
void UnofficialExtrasTracker::Shutdown() {}

void UnofficialExtrasTracker::OnPlayerJoin(const PlayerInfo& player) {
	PlayerManager::AddPlayer(player);
}

void UnofficialExtrasTracker::OnPlayerLeave(const PlayerInfo& player) {
	PlayerManager::RemovePlayer(player);
}

void UnofficialExtrasTracker::OnSelfDetected(const PlayerInfo& self) {
	PlayerManager::SetSelf(self);
}

void UnofficialExtrasTracker::OnSquadClear() {
	PlayerManager::ClearPlayers();
}
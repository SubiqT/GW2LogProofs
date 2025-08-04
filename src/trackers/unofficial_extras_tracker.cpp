#include "unofficial_extras_tracker.h"
#include "../core/event_handlers.h"
#include "../core/player_manager.h"
#include "../core/shared.h"

extern bool unofficialExtrasEnabled;

bool UnofficialExtrasTracker::IsAvailable() const {
	return unofficialExtrasEnabled;
}

void UnofficialExtrasTracker::Initialize() {
	APIDefs->Events.Subscribe("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", UnExSquadEventHandler);
}

void UnofficialExtrasTracker::Shutdown() {
	APIDefs->Events.Unsubscribe("EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", UnExSquadEventHandler);
}

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
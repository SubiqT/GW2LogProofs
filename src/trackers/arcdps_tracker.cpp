#include "arcdps_tracker.h"
#include "../core/event_handlers.h"
#include "../core/player_manager.h"
#include "../core/shared.h"

extern bool unofficialExtrasEnabled;

bool ArcdpsTracker::IsAvailable() const {
	return !unofficialExtrasEnabled;
}

void ArcdpsTracker::Initialize() {
	APIDefs->Events.Subscribe("EV_ARCDPS_SQUAD_JOIN", ArcSquadJoinEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SQUAD_LEAVE", ArcSquadLeaveEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SELF_JOIN", ArcSelfDetectedEventHandler);
	APIDefs->Events.Subscribe("EV_ARCDPS_SELF_LEAVE", ArcSelfLeaveEventHandler);
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SELF_JOIN", nullptr);
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SQUAD_JOIN", nullptr);
}

void ArcdpsTracker::Shutdown() {
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SELF_LEAVE", ArcSelfLeaveEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SELF_JOIN", ArcSelfDetectedEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SQUAD_LEAVE", ArcSquadLeaveEventHandler);
	APIDefs->Events.Unsubscribe("EV_ARCDPS_SQUAD_JOIN", ArcSquadJoinEventHandler);
}

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
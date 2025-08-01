#include "arcdps_tracker.h"
#include "../core/log_proofs.h"

bool ArcdpsTracker::IsAvailable() const {
    return !LogProofs::unofficalExtrasEnabled;
}

void ArcdpsTracker::Initialize() {
}

void ArcdpsTracker::Shutdown() {
}

void ArcdpsTracker::OnPlayerJoin(const PlayerInfo& player) {
    LogProofs::AddPlayerFromTracker(player);
}

void ArcdpsTracker::OnPlayerLeave(const PlayerInfo& player) {
    LogProofs::RemovePlayerFromTracker(player);
}

void ArcdpsTracker::OnSelfDetected(const PlayerInfo& self) {
    LogProofs::SetSelfFromTracker(self);
}

void ArcdpsTracker::OnSquadClear() {
    LogProofs::ClearPlayers();
}
#include "unofficial_extras_tracker.h"
#include "../log_proofs.h"

bool UnofficialExtrasTracker::IsAvailable() const {
	return LogProofs::unofficalExtrasEnabled;
}

void UnofficialExtrasTracker::Initialize() {
}

void UnofficialExtrasTracker::Shutdown() {
}

void UnofficialExtrasTracker::OnPlayerJoin(const PlayerInfo& player) {
	LogProofs::AddPlayerFromTracker(player);
}

void UnofficialExtrasTracker::OnPlayerLeave(const PlayerInfo& player) {
	LogProofs::RemovePlayerFromTracker(player);
}

void UnofficialExtrasTracker::OnSelfDetected(const PlayerInfo& self) {
	LogProofs::SetSelfFromTracker(self);
}

void UnofficialExtrasTracker::OnSquadClear() {
	LogProofs::ClearPlayers();
}
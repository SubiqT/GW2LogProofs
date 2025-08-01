#include "realtime_api_tracker.h"
#include "../core/log_proofs.h"
#include "../core/shared.h"
#include <chrono>
#include <format>

RealtimeApiTracker* RealtimeApiTracker::instance = nullptr;

bool RealtimeApiTracker::IsAvailable() const {
	// Check if RTAPI is loaded and functional
	if (!rtapi) {
		// Try to get RTAPI data link again
		rtapi = (RealTimeData*) APIDefs->DataLink.Get(DL_RTAPI);
	}
	return rtapi && rtapi->GameBuild != 0;
}

void RealtimeApiTracker::Initialize() {
	instance = this;
	rtapi = (RealTimeData*) APIDefs->DataLink.Get(DL_RTAPI);

	// Subscribe to RTAPI events
	APIDefs->Events.Subscribe(EV_RTAPI_GROUP_MEMBER_JOINED, OnGroupMemberJoinedStatic);
	APIDefs->Events.Subscribe(EV_RTAPI_GROUP_MEMBER_LEFT, OnGroupMemberLeftStatic);
	APIDefs->Events.Subscribe(EV_RTAPI_GROUP_MEMBER_UPDATED, OnGroupMemberUpdatedStatic);
}

void RealtimeApiTracker::Shutdown() {
	APIDefs->Events.Unsubscribe(EV_RTAPI_GROUP_MEMBER_JOINED, OnGroupMemberJoinedStatic);
	APIDefs->Events.Unsubscribe(EV_RTAPI_GROUP_MEMBER_LEFT, OnGroupMemberLeftStatic);
	APIDefs->Events.Unsubscribe(EV_RTAPI_GROUP_MEMBER_UPDATED, OnGroupMemberUpdatedStatic);
	rtapi = nullptr;
	instance = nullptr;
}

void RealtimeApiTracker::OnPlayerJoin(const PlayerInfo& player) {
	LogProofs::AddPlayerFromTracker(player);
}

void RealtimeApiTracker::OnPlayerLeave(const PlayerInfo& player) {
	LogProofs::RemovePlayerFromTracker(player);
}

void RealtimeApiTracker::OnSelfDetected(const PlayerInfo& self) {
	LogProofs::SetSelfFromTracker(self);
}

void RealtimeApiTracker::OnSquadClear() {
	LogProofs::ClearPlayers();
}

// Static event handlers
void RealtimeApiTracker::OnGroupMemberJoinedStatic(void* eventArgs) {
	if (instance) instance->OnGroupMemberJoined(eventArgs);
}

void RealtimeApiTracker::OnGroupMemberLeftStatic(void* eventArgs) {
	if (instance) instance->OnGroupMemberLeft(eventArgs);
}

void RealtimeApiTracker::OnGroupMemberUpdatedStatic(void* eventArgs) {
	if (instance) instance->OnGroupMemberUpdated(eventArgs);
}

// Instance event handlers
void RealtimeApiTracker::OnGroupMemberJoined(void* eventArgs) {
	if (!IsAvailable()) return;

	GroupMember* member = (GroupMember*) eventArgs;
	if (!member) return;

	PlayerInfo playerInfo;
	playerInfo.account = LogProofs::StripAccount(member->AccountName);
	playerInfo.character = member->CharacterName;
	playerInfo.id = 0;
	playerInfo.instanceId = 0;
	playerInfo.source = "RealTime API";
	playerInfo.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
								   std::chrono::system_clock::now().time_since_epoch()
	)
								   .count();

	if (member->IsSelf) {
		LogProofs::SetSelfFromTracker(playerInfo);
	} else {
		LogProofs::AddPlayerFromTracker(playerInfo);
	}
}

void RealtimeApiTracker::OnGroupMemberLeft(void* eventArgs) {
	if (!IsAvailable()) return;

	GroupMember* member = (GroupMember*) eventArgs;
	if (!member) return;

	PlayerInfo playerInfo;
	playerInfo.account = LogProofs::StripAccount(member->AccountName);
	playerInfo.source = "RealTime API";

	if (member->IsSelf) {
		LogProofs::ClearPlayers();
	} else {
		LogProofs::RemovePlayerFromTracker(playerInfo);
	}
}

void RealtimeApiTracker::OnGroupMemberUpdated(void* eventArgs) {
	OnGroupMemberJoined(eventArgs);
}
#include "realtime_api_tracker.h"
#include "../core/player_manager.h"
#include "../core/shared.h"
#include <chrono>
#include <format>

static std::string StripAccount(std::string account) {
	if (!account.empty() && account.at(0) == ':') {
		return account.erase(0, 1);
	}
	return account;
}

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
	PlayerManager::AddPlayer(player);
}

void RealtimeApiTracker::OnPlayerLeave(const PlayerInfo& player) {
	PlayerManager::RemovePlayer(player);
}

void RealtimeApiTracker::OnSelfDetected(const PlayerInfo& self) {
	PlayerManager::SetSelf(self);
}

void RealtimeApiTracker::OnSquadClear() {
	PlayerManager::ClearPlayers();
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
	playerInfo.account = StripAccount(member->AccountName);
	playerInfo.character = member->CharacterName;
	playerInfo.id = 0;
	playerInfo.instanceId = 0;
	playerInfo.source = "RealTime API";
	playerInfo.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
								   std::chrono::system_clock::now().time_since_epoch()
	)
								   .count();

	if (member->IsSelf) {
		PlayerManager::SetSelf(playerInfo);
	} else {
		PlayerManager::AddPlayer(playerInfo);
	}
}

void RealtimeApiTracker::OnGroupMemberLeft(void* eventArgs) {
	if (!IsAvailable()) return;

	GroupMember* member = (GroupMember*) eventArgs;
	if (!member) return;

	PlayerInfo playerInfo;
	playerInfo.account = StripAccount(member->AccountName);
	playerInfo.source = "RealTime API";

	if (member->IsSelf) {
		PlayerManager::ClearPlayers();
	} else {
		PlayerManager::RemovePlayer(playerInfo);
	}
}

void RealtimeApiTracker::OnGroupMemberUpdated(void* eventArgs) {
	OnGroupMemberJoined(eventArgs);
}
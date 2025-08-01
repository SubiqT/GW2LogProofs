#ifndef EVENT_HANDLERS_H
#define EVENT_HANDLERS_H

#include "../unofficial_extras/Definitions.h"
#include "player_info.h"
#include "player_tracker_manager.h"

struct SquadUpdate {
	UserInfo* UserInfo;
	uint64_t UsersCount;
};

extern PlayerTrackerManager trackerManager;

void InitializeTrackerManager();
void ShutdownTrackerManager();
void UnExSquadEventHandler(void* eventArgs);
void ArcSquadJoinEventHandler(void* eventArgs);
void ArcSquadLeaveEventHandler(void* eventArgs);
void ArcSelfLeaveEventHandler(void* eventArgs);
void ArcSelfDetectedEventHandler(void* eventArgs);

#endif
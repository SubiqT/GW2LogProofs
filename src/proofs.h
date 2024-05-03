#ifndef TIMER_H
#define TIMER_H

#include "unofficial_extras/Definitions.h"
#include "shared.h"

Player GetProof(const char* account);

void AddPlayer(const char* account);
void RemovePlayer(const char* account);

void SquadEventHandler(const UserInfo* updatedUsers, size_t updatedUsersCount);

#endif
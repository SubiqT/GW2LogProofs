#ifndef TIMER_H
#define TIMER_H

#include "unofficial_extras/Definitions.h"
#include "shared.h"

extern bool shouldClearAllPlayers;
extern std::string shouldRemovePlayer;
extern std::string shouldAddPlayer;

void UpdatePlayers(void);

void SquadEventHandler(void* eventArgs);

#endif
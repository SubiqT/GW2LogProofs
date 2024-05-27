#ifndef TIMER_H
#define TIMER_H

#include "threadpool.hpp"

extern Threadpool threadpool;

void SquadEventHandler(void* eventArgs);
void CombatEventHandler(void* eventArgs);

void UpdatePlayers(void);

#endif
#ifndef TIMER_H
#define TIMER_H

extern bool shouldClearAllPlayers;
extern std::string shouldRemovePlayer;
extern std::string shouldAddPlayer;

void UpdatePlayers(void);

void SquadEventHandler(void* eventArgs);
void CombatEventHandler(void* eventArgs);

#endif
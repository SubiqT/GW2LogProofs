#ifndef PLAYER_TRACKER_H
#define PLAYER_TRACKER_H

#include "player_info.h"
#include <string>

class IPlayerTracker {
public:
	virtual ~IPlayerTracker() = default;
	virtual std::string GetName() const = 0;
	virtual int GetPriority() const = 0;
	virtual bool IsAvailable() const = 0;
	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void OnPlayerJoin(const PlayerInfo& player) = 0;
	virtual void OnPlayerLeave(const PlayerInfo& player) = 0;
	virtual void OnSelfDetected(const PlayerInfo& self) = 0;
	virtual void OnSquadClear() = 0;
};

#endif
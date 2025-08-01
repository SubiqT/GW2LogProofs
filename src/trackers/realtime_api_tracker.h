#ifndef REALTIME_API_TRACKER_H
#define REALTIME_API_TRACKER_H

#include "../core/player_tracker.h"
#include "../realtime_api/RTAPI.h"

class RealtimeApiTracker : public IPlayerTracker {
private:
	mutable RealTimeData* rtapi = nullptr;
	static RealtimeApiTracker* instance;

public:
	std::string GetName() const override { return "RealTime API"; }
	int GetPriority() const override { return 0; } // Highest priority
	bool IsAvailable() const override;
	void Initialize() override;
	void Shutdown() override;
	void OnPlayerJoin(const PlayerInfo& player) override;
	void OnPlayerLeave(const PlayerInfo& player) override;
	void OnSelfDetected(const PlayerInfo& self) override;
	void OnSquadClear() override;

	// Static event handlers for RTAPI events
	static void OnGroupMemberJoinedStatic(void* eventArgs);
	static void OnGroupMemberLeftStatic(void* eventArgs);
	static void OnGroupMemberUpdatedStatic(void* eventArgs);

	// Instance event handlers
	void OnGroupMemberJoined(void* eventArgs);
	void OnGroupMemberLeft(void* eventArgs);
	void OnGroupMemberUpdated(void* eventArgs);
};

#endif
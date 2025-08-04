#ifndef ARCDPS_TRACKER_H
#define ARCDPS_TRACKER_H

#include "../core/player_tracker.h"

class ArcdpsTracker : public IPlayerTracker {
public:
    std::string GetName() const override { return "ArcDPS"; }
    int GetPriority() const override { return 2; }
    bool IsAvailable() const override;
    void Initialize() override;
    void Shutdown() override;
    void OnPlayerJoin(const PlayerInfo& player) override;
    void OnPlayerLeave(const PlayerInfo& player) override;
    void OnSelfDetected(const PlayerInfo& self) override;
    void OnSquadClear() override;
};

#endif
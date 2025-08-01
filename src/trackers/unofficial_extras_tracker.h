#ifndef UNOFFICIAL_EXTRAS_TRACKER_H
#define UNOFFICIAL_EXTRAS_TRACKER_H

#include "../core/player_tracker.h"

class UnofficialExtrasTracker : public IPlayerTracker {
public:
    std::string GetName() const override { return "Unofficial Extras"; }
    int GetPriority() const override { return 1; }
    bool IsAvailable() const override;
    void Initialize() override;
    void Shutdown() override;
    void OnPlayerJoin(const PlayerInfo& player) override;
    void OnPlayerLeave(const PlayerInfo& player) override;
    void OnSelfDetected(const PlayerInfo& self) override;
    void OnSquadClear() override;
};

#endif
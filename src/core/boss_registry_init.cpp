#include "boss_registry.h"
#include "../providers/wingman/wingman_boss_provider.h"
#include "../providers/kpme/kpme_boss_provider.h"

void InitializeBossRegistry();

void InitializeBossRegistry() {
    auto& registry = BossRegistry::Instance();
    registry.RegisterProvider("Wingman", std::make_unique<WingmanBossProvider>());
    registry.RegisterProvider("KPME", std::make_unique<KpmeBossProvider>());
}
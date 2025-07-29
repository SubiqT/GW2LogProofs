#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "../core/bosses.h"
#include "../core/player_data.h"

class UIRenderer {
public:
	static void DrawPlayerAccountName(const Player& player);
	static void DrawPlayerProofValue(const Player& player, const std::string& proofId);
	static void DrawGenericTab(const char* tabName, const std::vector<std::string>& proofIds);
	static void DrawBossTab(const char* tabName, const std::vector<Boss>& bosses, bool isLegendary = false);
};

#endif
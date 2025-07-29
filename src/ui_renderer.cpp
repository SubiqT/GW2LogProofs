#include "ui_renderer.h"
#include "imgui/imgui.h"
#include "imgui_extensions.h"
#include "settings.h"
#include <Windows.h>

void UIRenderer::DrawPlayerAccountName(const Player& player) {
    if (player.proofData && !player.proofData->profileUrl.empty()) {
        if (ImGui::TextURL(player.account.c_str())) {
            ShellExecuteA(0, 0, player.proofData->profileUrl.c_str(), 0, 0, SW_SHOW);
        }
    } else {
        ImGui::Text(player.account.c_str());
    }
}

void UIRenderer::DrawPlayerProofValue(const Player& player, const std::string& proofId) {
    if (player.state == LoadState::READY && player.proofData) {
        auto it = player.proofData->proofs.find(proofId);
        if (it != player.proofData->proofs.end()) {
            ImGui::Text("%i", it->second.amount);
        } else {
            ImGui::Text("-");
        }
    } else if (player.state == LoadState::LOADING) {
        ImGui::Text("...");
    } else {
        ImGui::Text("-");
    }
}

void UIRenderer::DrawGenericTab(const char* tabName, const std::vector<std::string>& proofIds) {
    // Implementation will be added when needed for specific proof types
}

void UIRenderer::DrawBossTab(const char* tabName, const std::vector<Boss>& bosses, bool isLegendary) {
    // Implementation will be added when needed for boss-specific rendering
}
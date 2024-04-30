#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"


BOOL showProofsWindow = true;
std::vector<Player> players;

void RenderProofs() {
	if (!showProofsWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		if (players.size() > 0) {
			for (Player player : players) {
				ImGui::Text(player.account.c_str());
				for (const auto& [boss_id, boss_kp] : player.kp) {
					for (const auto& [spec, kills] : boss_kp) {
						if (spec == "total") {
							ImGui::Text("%s: %i", boss_id.c_str(), kills);
						}
					}
				}
			}
		}
		
		if (ImGui::Button("Refresh")) {
			players.push_back(GetProof("Subi.8014"));
		}
	}
	ImGui::End();
}

void RenderSettings() {
	if (showProofsWindow) {
		showProofsWindow = !ImGui::Button("Hide Proofs Window");
	} else {
		showProofsWindow = ImGui::Button("Show Proofs Window");
	}
}

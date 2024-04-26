#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

BOOL showProofsWindow = true;

void RenderProofs() {
	if (!showProofsWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		unsigned int kills = 0;
		ImGui::Text("Kills: %u",
			kills
		);
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

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"


BOOL showProofsWindow = true;
char* player;

void RenderProofs() {
	if (!showProofsWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		ImGui::Text(player ? player : "");
		if (ImGui::Button("Refresh")) {
			player = GetProof("Subi.8014");
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

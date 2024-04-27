#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

BOOL showProofsWindow = true;
const char* proofsTable;

void RenderProofs() {
	if (!showProofsWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		ImGui::Text(proofsTable ? proofsTable : "Not loaded.");
	}
	ImGui::End();
}

void RenderSettings() {
	if (showProofsWindow) {
		showProofsWindow = !ImGui::Button("Hide Proofs Window");
	} else {
		showProofsWindow = ImGui::Button("Show Proofs Window");
	}
	if (ImGui::Button("Refresh")) {
		proofsTable = GetProof();
	}

}

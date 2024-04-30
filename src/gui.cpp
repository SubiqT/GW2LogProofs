#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"


BOOL showProofsWindow = true;
std::vector<Player> players;

const char* GetBossName(std::string boss_id) {
	Boss boss = Boss(std::stoi(boss_id));
	switch (boss) {
		case ValeGuardian:
			return "Vale Guardian";
		case Gorseval:
			return "Gorseval";
		case Sabetha:
			return "Sabetha";
		case Slothasor:
			return "Slothasor";
		case BanditTrio:
			return "Bandit Trio";
		case Matthias:
			return "Matthias";
		case Escort:
			return "Escort";
		case KeepConstruct:
			return "Keep Construct";
		case TwistedCastle:
			return "Twisted Castle";
		case Xera:
			return "Xera";
		case Cairn:
			return "Cairn";
		case MursaatOverseer:
			return "Mursaat Overseer";
		case Samarog:
			return "Samarog";
		case Deimos:
			return "Deimos";
		case SoullessHorror:
			return "Soulles Horror";
		case RiverOfSouls:
			return "River of Souls";
		case BrokenKing:
			return "Broken King";
		case EaterOfSouls:
			return "Eater of Souls";
		case Eyes:
			return "Eyes";
		case Dhuum:
			return "Dhuum";
		case ConjuredAmalgamate:
			return "Conjured Amalgamate";
		case TwinLargos:
			return "Twin Largos";
		case Qadim:
			return "Qadim";
		case Adina:
			return "Adina";
		case Sabir:
			return "Sabir";
		case QadimThePeerless:
			return "Qadim the Peerless";
		default:
			return "Unknown";
	}
}

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
							const char* boss_name = GetBossName(boss_id);
							if (boss_name != "Unknown") {
								ImGui::Text("%s: %i", GetBossName(boss_id), kills);
							}
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

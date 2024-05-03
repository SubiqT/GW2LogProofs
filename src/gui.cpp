#include <format>

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

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
	if (!Config.showWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		if (players.size() > 0) {
			if (ImGui::BeginTable("table", 26, ImGuiTableFlags_BordersInnerV)) {
				// Header
				ImGui::TableNextColumn();
				ImGui::Text("Account");
				ImGui::TableNextColumn();
				ImGui::Text("VG");
				ImGui::TableNextColumn();
				ImGui::Text("Gors");
				ImGui::TableNextColumn();
				ImGui::Text("Sab");
				ImGui::TableNextColumn();
				ImGui::Text("Sloth");
				ImGui::TableNextColumn();
				ImGui::Text("Trio");
				ImGui::TableNextColumn();
				ImGui::Text("Matt");
				ImGui::TableNextColumn();
				ImGui::Text("Escort");
				ImGui::TableNextColumn();
				ImGui::Text("KC");
				ImGui::TableNextColumn();
				ImGui::Text("Xera");
				ImGui::TableNextColumn();
				ImGui::Text("Carin");
				ImGui::TableNextColumn();
				ImGui::Text("MO");
				ImGui::TableNextColumn();
				ImGui::Text("Sama");
				ImGui::TableNextColumn();
				ImGui::Text("Deimos");
				ImGui::TableNextColumn();
				ImGui::Text("SH");
				ImGui::TableNextColumn();
				ImGui::Text("River");
				ImGui::TableNextColumn();
				ImGui::Text("BK");
				ImGui::TableNextColumn();
				ImGui::Text("Eater");
				ImGui::TableNextColumn();
				ImGui::Text("Eyes");
				ImGui::TableNextColumn();
				ImGui::Text("Dhuum");
				ImGui::TableNextColumn();
				ImGui::Text("CA");
				ImGui::TableNextColumn();
				ImGui::Text("Largos");
				ImGui::TableNextColumn();
				ImGui::Text("Qadim");
				ImGui::TableNextColumn();
				ImGui::Text("Adina");
				ImGui::TableNextColumn();
				ImGui::Text("Sabir");
				ImGui::TableNextColumn();
				ImGui::Text("QtP");

				// Records
				for (Player player : players) {
					ImGui::TableNextColumn();
					ImGui::Text(player.account.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(ValeGuardian))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Gorseval))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Sabetha))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Slothasor))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(BanditTrio))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Matthias))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Escort))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(KeepConstruct))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Xera))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Cairn))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(MursaatOverseer))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Samarog))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Deimos))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(SoullessHorror))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(RiverOfSouls))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(BrokenKing))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(EaterOfSouls))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Eyes))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Dhuum))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(ConjuredAmalgamate))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(TwinLargos))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Qadim))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Adina))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(Sabir))][std::string("total")]);
					ImGui::TableNextColumn();
					ImGui::Text("%i", player.kp[std::format("{}", int(QadimThePeerless))][std::string("total")]);
				}
				ImGui::EndTable();
			}
		}
		
		if (ImGui::Button("Refresh")) {
			std::string account = "Subi.8014";
			for (Player& player : players) {
				if (player.account == account) {
					long long index = std::addressof(player) - std::addressof(players[0]);
					players.erase(players.begin() + index);
				}
			}
			players.push_back(GetProof("Subi.8014"));
		}
	}
	ImGui::End();
}

void RenderSettings() {
	if (ImGui::Button(Config.showWindow ? "Hide Window" : "Show Window")) {
		Config.showWindow = !Config.showWindow;
	}
}

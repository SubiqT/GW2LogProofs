#include <format>

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

enum Boss {
	ValeGuardian = 15438,
	Gorseval = 15429,
	Sabetha = 15375,
	Slothasor = 16123,
	BanditTrio = 16088,
	Matthias = 16115,
	Escort = 16253,
	KeepConstruct = 16235,
	TwistedCastle = 16247,
	Xera = 16246,
	Cairn = 17194,
	MursaatOverseer = 17172,
	Samarog = 17188,
	Deimos = 17154,
	SoullessHorror = 19767,
	RiverOfSouls = 19828,
	BrokenKing = 19691,
	EaterOfSouls = 19536,
	Eyes = 19651,
	Dhuum = 19450,
	ConjuredAmalgamate = 43974,
	TwinLargos = 21105,
	Qadim = 20934,
	Adina = 22006,
	Sabir = 21964,
	QadimThePeerless = 22000
};

enum RaidWing {
	SpiritVale,
	SalvationPass,
	StrongholdOfTheFaithful,
	BastionOfThePenitent,
	HallOfChains,
	MythwrightGambit,
	TheKeyOfAhdashim
};

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

void RenderRaidsHeader(RaidWing wing) {
	ImGui::TableNextColumn();
	ImGui::Text("Account");
	switch (wing) {
		case SpiritVale:
			ImGui::TableNextColumn();
			ImGui::Text("VG");
			ImGui::TableNextColumn();
			ImGui::Text("Gors");
			ImGui::TableNextColumn();
			ImGui::Text("Sab");
			return;
		case SalvationPass:
			ImGui::TableNextColumn();
			ImGui::Text("Sloth");
			ImGui::TableNextColumn();
			ImGui::Text("Trio");
			ImGui::TableNextColumn();
			ImGui::Text("Matt");
			return;
		case StrongholdOfTheFaithful:
			ImGui::TableNextColumn();
			ImGui::Text("Escort");
			ImGui::TableNextColumn();
			ImGui::Text("KC");
			ImGui::TableNextColumn();
			ImGui::Text("TC");
			ImGui::TableNextColumn();
			ImGui::Text("Xera");
			return;
		case BastionOfThePenitent:
			ImGui::TableNextColumn();
			ImGui::Text("Carin");
			ImGui::TableNextColumn();
			ImGui::Text("MO");
			ImGui::TableNextColumn();
			ImGui::Text("Sama");
			ImGui::TableNextColumn();
			ImGui::Text("Deimos");
			return;
		case HallOfChains:
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
			return;
		case MythwrightGambit:
			ImGui::TableNextColumn();
			ImGui::Text("CA");
			ImGui::TableNextColumn();
			ImGui::Text("Largos");
			ImGui::TableNextColumn();
			ImGui::Text("Qadim");
			return;
		case TheKeyOfAhdashim:
			ImGui::TableNextColumn();
			ImGui::Text("Adina");
			ImGui::TableNextColumn();
			ImGui::Text("Sabir");
			ImGui::TableNextColumn();
			ImGui::Text("QtP");
			return;
	}
}

void RenderRaidsRow(Player player, RaidWing wing) {
	ImGui::TableNextColumn();
	ImGui::Text(player.account.c_str());
	switch (wing) {
		case SpiritVale:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(ValeGuardian))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Gorseval))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Sabetha))][std::string("total")]);
			return;
		case SalvationPass:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Slothasor))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(BanditTrio))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Matthias))][std::string("total")]);
			return;
		case StrongholdOfTheFaithful:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Escort))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(KeepConstruct))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(TwistedCastle))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Xera))][std::string("total")]);
			return;
		case BastionOfThePenitent:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Cairn))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(MursaatOverseer))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Samarog))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Deimos))][std::string("total")]);
			return;
		case HallOfChains:
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
			return;
		case MythwrightGambit:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(ConjuredAmalgamate))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(TwinLargos))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Qadim))][std::string("total")]);
			return;
		case TheKeyOfAhdashim:
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Adina))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(Sabir))][std::string("total")]);
			ImGui::TableNextColumn();
			ImGui::Text("%i", player.kp[std::format("{}", int(QadimThePeerless))][std::string("total")]);
			return;
	}
}

void RenderWindow() {
	if (!Config.showWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		if (players.size() > 0 || !selfName.empty()) {
			if (ImGui::BeginTabBar("##Wings", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Spirit Vale")) {
					if (ImGui::BeginTable("table", 4, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(SpiritVale);
						if (!selfName.empty()) {
							RenderRaidsRow(self, SpiritVale);
						}
						for (Player player : players) {
							RenderRaidsRow(player, SpiritVale);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Salvation Pass")) {
					if (ImGui::BeginTable("table", 4, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(SalvationPass);
						if (!selfName.empty()) {
							RenderRaidsRow(self, SalvationPass);
						}
						for (Player player : players) {
							RenderRaidsRow(player, SalvationPass);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Stronghold of the Faithful")) {
					if (ImGui::BeginTable("table", 5, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(StrongholdOfTheFaithful);
						if (!selfName.empty()) {
							RenderRaidsRow(self, StrongholdOfTheFaithful);
						}
						for (Player player : players) {
							RenderRaidsRow(player, StrongholdOfTheFaithful);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Bastion of the Penitent")) {
					if (ImGui::BeginTable("table", 5, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(BastionOfThePenitent);
						if (!selfName.empty()) {
							RenderRaidsRow(self, BastionOfThePenitent);
						}
						for (Player player : players) {
							RenderRaidsRow(player, BastionOfThePenitent);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Hall of Chains")) {
					if (ImGui::BeginTable("table", 7, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(HallOfChains);
						if (!selfName.empty()) {
							RenderRaidsRow(self, HallOfChains);
						}
						for (Player player : players) {
							RenderRaidsRow(player, HallOfChains);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Mythwright Gambit")) {
					if (ImGui::BeginTable("table", 4, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(MythwrightGambit);
						if (!selfName.empty()) {
							RenderRaidsRow(self, MythwrightGambit);
						}
						for (Player player : players) {
							RenderRaidsRow(player, MythwrightGambit);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("The Key  of Ahdashim")) {
					if (ImGui::BeginTable("table", 4, ImGuiTableFlags_BordersInnerV)) {
						RenderRaidsHeader(TheKeyOfAhdashim);
						if (!selfName.empty()) {
							RenderRaidsRow(self, TheKeyOfAhdashim);
						}
						for (Player player : players) {
							RenderRaidsRow(player, TheKeyOfAhdashim);
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
	}
	ImGui::End();
}

void RenderSettings() {
	if (ImGui::Button(Config.showWindow ? "Hide Window" : "Show Window")) {
		Config.showWindow = !Config.showWindow;
	}
}

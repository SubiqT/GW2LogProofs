#include <format>

#include "imgui/imgui.h"
#include "shared.h"
#include "proofs.h"

std::map<int, Texture*> bossTextures = {};
static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingFixedFit;

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

const char* GetBossName(Boss boss) {
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

Texture* GetBossTexture(Boss boss) {
	int boss_id = int(boss);
	if (bossTextures[boss_id] != nullptr) {
		return bossTextures[boss_id];
	}
	switch (boss_id) {
		case ValeGuardian:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("vale_guardian", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Vale_Guardian_Trophy.png");
			return bossTextures[boss_id];
		case Gorseval:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("gorseval", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Gorseval_Trophy.png");
			return bossTextures[boss_id];
		case Sabetha:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("sabetha", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Sabetha_Trophy.png");
			return bossTextures[boss_id];
		case Slothasor:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("slothasor", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Slothasor_Trophy.png");
			return bossTextures[boss_id];
		case BanditTrio:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("bandit_trio", "https://gw2wingman.nevermindcreations.de", "/static/Mini_Narella.png");
			return bossTextures[boss_id];
		case Matthias:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("matthias", "https://gw2wingman.nevermindcreations.de", "/static/Silver_White_Mantle_Abomination_Trophy.png");
			return bossTextures[boss_id];
		case Escort:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("escort", "https://gw2wingman.nevermindcreations.de", "/static/Bloodstone_Turret_Fragment.png");
			return bossTextures[boss_id];
		case KeepConstruct:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("keep_construct", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Keep_Construct_Trophy.png");
			return bossTextures[boss_id];
		case TwistedCastle:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("twisted_castle", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Siege_the_Stronghold_Trophy.png");
			return bossTextures[boss_id];
		case Xera:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("xera", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Xera_Trophy.png");
			return bossTextures[boss_id];
		case Cairn:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("cairn", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cairn_the_Indomitable_Trophy.png");
			return bossTextures[boss_id];
		case MursaatOverseer:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("mursaat_overseer", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Mursaat_Overseer_Trophy.png");
			return bossTextures[boss_id];
		case Samarog:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("samarog", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Samarog_Trophy.png");
			return bossTextures[boss_id];
		case Deimos:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("deimos", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Deimos_Trophy.png");
			return bossTextures[boss_id];
		case SoullessHorror:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("soulless_horror", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Desmina_Trophy.png");
			return bossTextures[boss_id];
		case RiverOfSouls:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("river_of_souls", "https://gw2wingman.nevermindcreations.de", "/static/Silver_River_of_Souls_Trophy.png");
			return bossTextures[boss_id];
		case BrokenKing:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("broken_king", "https://gw2wingman.nevermindcreations.de", "/static/Icon_BrokenKing.png");
			return bossTextures[boss_id];
		case EaterOfSouls:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("eater_of_souls", "https://gw2wingman.nevermindcreations.de", "/static/Icon_EaterOfSouls.png");
			return bossTextures[boss_id];
		case Eyes:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("eyes", "https://gw2wingman.nevermindcreations.de", "/static/Icon_Eyes.png");
			return bossTextures[boss_id];
		case Dhuum:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("dhuum", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Dhuum_Trophy.png");
			return bossTextures[boss_id];
		case ConjuredAmalgamate:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("conjured_amalgamate", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Conjured_Amalgamate_Trophy.png");
			return bossTextures[boss_id];
		case TwinLargos:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("twin_largos", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Twin_Largos_Trophy.png");
			return bossTextures[boss_id];
		case Qadim:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("qadim", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Qadim_Trophy.png");
			return bossTextures[boss_id];
		case Adina:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("adina", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cardinal_Adina_Trophy.png");
			return bossTextures[boss_id];
		case Sabir:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("sabir", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Cardinal_Sabir_Trophy.png");
			return bossTextures[boss_id];
		case QadimThePeerless:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("qadim_the_pearless", "https://gw2wingman.nevermindcreations.de", "/static/Silver_Ether_Djinn_Trophy.png");
			return bossTextures[boss_id];
		default:
			bossTextures[boss_id] = APIDefs->GetTextureOrCreateFromURL("any", "https://wiki.guildwars2.com", "/images/a/a7/Mini_Eye_of_Janthir.png");
			return bossTextures[boss_id];
	}
}

void RenderBossHeader(Boss boss) {
	ImGui::TableNextColumn();
	Texture* texture = GetBossTexture(boss);
	texture != nullptr
		? ImGui::Image((void*)texture->Resource, ImVec2(32.f, 32.f))
		: ImGui::Text(GetBossName(boss));
}

void RenderRaidsHeaderPerWing(RaidWing wing) {
	ImGui::TableNextColumn();
	switch (wing) {
		case SpiritVale:
			RenderBossHeader(ValeGuardian);
			RenderBossHeader(Gorseval);
			RenderBossHeader(Sabetha);
			return;
		case SalvationPass:
			RenderBossHeader(Slothasor);
			RenderBossHeader(BanditTrio);
			RenderBossHeader(Matthias);
			return;
		case StrongholdOfTheFaithful:
			RenderBossHeader(Escort);
			RenderBossHeader(KeepConstruct);
			RenderBossHeader(TwistedCastle);
			RenderBossHeader(Xera);
			return;
		case BastionOfThePenitent:
			RenderBossHeader(Cairn);
			RenderBossHeader(MursaatOverseer);
			RenderBossHeader(Samarog);
			RenderBossHeader(Deimos);
			return;
		case HallOfChains:
			RenderBossHeader(SoullessHorror);
			RenderBossHeader(RiverOfSouls);
			RenderBossHeader(BrokenKing);
			RenderBossHeader(EaterOfSouls);
			RenderBossHeader(Eyes);
			RenderBossHeader(Dhuum);
			return;
		case MythwrightGambit:
			RenderBossHeader(ConjuredAmalgamate);
			RenderBossHeader(TwinLargos);
			RenderBossHeader(Qadim);
			return;
		case TheKeyOfAhdashim:
			RenderBossHeader(Adina);
			RenderBossHeader(Sabir);
			RenderBossHeader(QadimThePeerless);
			return;
	}
}

void RenderRaidsHeaderAllWings() {
	ImGui::TableNextColumn();
	RenderBossHeader(ValeGuardian);
	RenderBossHeader(Gorseval);
	RenderBossHeader(Sabetha);

	RenderBossHeader(Slothasor);
	RenderBossHeader(BanditTrio);
	RenderBossHeader(Matthias);

	RenderBossHeader(Escort);
	RenderBossHeader(KeepConstruct);
	RenderBossHeader(TwistedCastle);
	RenderBossHeader(Xera);

	RenderBossHeader(Cairn);
	RenderBossHeader(MursaatOverseer);
	RenderBossHeader(Samarog);
	RenderBossHeader(Deimos);

	RenderBossHeader(SoullessHorror);
	RenderBossHeader(RiverOfSouls);
	RenderBossHeader(BrokenKing);
	RenderBossHeader(EaterOfSouls);
	RenderBossHeader(Eyes);
	RenderBossHeader(Dhuum);

	RenderBossHeader(ConjuredAmalgamate);
	RenderBossHeader(TwinLargos);
	RenderBossHeader(Qadim);

	RenderBossHeader(Adina);
	RenderBossHeader(Sabir);
	RenderBossHeader(QadimThePeerless);
}

void RenderBossRow(Player* player, Boss boss) {
	ImGui::TableNextColumn();
	ImGui::Text("%i", player->kp[std::format("{}", int(boss))][std::string("total")]);
}

void RenderRaidsRowPerWing(Player* player, RaidWing wing) {
	ImGui::TableNextColumn();
	ImGui::Text(player->account.c_str());
	switch (wing) {
	case SpiritVale:
		RenderBossRow(player, ValeGuardian);
		RenderBossRow(player, Gorseval);
		RenderBossRow(player, Sabetha);
		return;
	case SalvationPass:
		RenderBossRow(player, Slothasor);
		RenderBossRow(player, BanditTrio);
		RenderBossRow(player, Matthias);
		return;
	case StrongholdOfTheFaithful:
		RenderBossRow(player, Escort);
		RenderBossRow(player, KeepConstruct);
		RenderBossRow(player, TwistedCastle);
		RenderBossRow(player, Xera);
		return;
	case BastionOfThePenitent:
		RenderBossRow(player, Cairn);
		RenderBossRow(player, MursaatOverseer);
		RenderBossRow(player, Samarog);
		RenderBossRow(player, Deimos);
		return;
	case HallOfChains:
		RenderBossRow(player, SoullessHorror);
		RenderBossRow(player, RiverOfSouls);
		RenderBossRow(player, BrokenKing);
		RenderBossRow(player, EaterOfSouls);
		RenderBossRow(player, Eyes);
		RenderBossRow(player, Dhuum);
		return;
	case MythwrightGambit:
		RenderBossRow(player, ConjuredAmalgamate);
		RenderBossRow(player, TwinLargos);
		RenderBossRow(player, Qadim);
		return;
	case TheKeyOfAhdashim:
		RenderBossRow(player, Adina);
		RenderBossRow(player, Sabir);
		RenderBossRow(player, QadimThePeerless);
		return;
	}
}

void RenderRaidsRowAllWings(Player* player) {
	ImGui::TableNextColumn();
	ImGui::Text(player->account.c_str());
	RenderBossRow(player, ValeGuardian);
	RenderBossRow(player, Gorseval);
	RenderBossRow(player, Sabetha);

	RenderBossRow(player, Slothasor);
	RenderBossRow(player, BanditTrio);
	RenderBossRow(player, Matthias);

	RenderBossRow(player, Escort);
	RenderBossRow(player, KeepConstruct);
	RenderBossRow(player, TwistedCastle);
	RenderBossRow(player, Xera);

	RenderBossRow(player, Cairn);
	RenderBossRow(player, MursaatOverseer);
	RenderBossRow(player, Samarog);
	RenderBossRow(player, Deimos);

	RenderBossRow(player, SoullessHorror);
	RenderBossRow(player, RiverOfSouls);
	RenderBossRow(player, BrokenKing);
	RenderBossRow(player, EaterOfSouls);
	RenderBossRow(player, Eyes);
	RenderBossRow(player, Dhuum);

	RenderBossRow(player, ConjuredAmalgamate);
	RenderBossRow(player, TwinLargos);
	RenderBossRow(player, Qadim);

	RenderBossRow(player, Adina);
	RenderBossRow(player, Sabir);
	RenderBossRow(player, QadimThePeerless);
}

void RenderWindow() {
	if (!Config.showWindow) {
		return;
	}

	if (ImGui::Begin("Log Proofs")) {
		if (players.size() > 0 || !selfName.empty()) {
			if (!Config.splitPerWing) {
				if (ImGui::BeginTabBar("##GameModes", ImGuiTabBarFlags_None)) {
					if (ImGui::BeginTabItem("Raids")) {
						if (ImGui::BeginTable("raidsTable", 27, tableFlags)) {
							RenderRaidsHeaderAllWings();
							if (!selfName.empty()) {
								RenderRaidsRowAllWings(&self);
							}
							for (Player player : players) {
								RenderRaidsRowAllWings(&player);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			} else {
				if (ImGui::BeginTabBar("##Wings", ImGuiTabBarFlags_None)) {
					if (ImGui::BeginTabItem("Spirit Vale")) {
						if (ImGui::BeginTable("tableW1", 4, tableFlags)) {
							RenderRaidsHeaderPerWing(SpiritVale);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, SpiritVale);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, SpiritVale);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Salvation Pass")) {
						if (ImGui::BeginTable("tableW2", 4, tableFlags)) {
							RenderRaidsHeaderPerWing(SalvationPass);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, SalvationPass);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, SalvationPass);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Stronghold of the Faithful")) {
						if (ImGui::BeginTable("tableW3", 5, tableFlags)) {
							RenderRaidsHeaderPerWing(StrongholdOfTheFaithful);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, StrongholdOfTheFaithful);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, StrongholdOfTheFaithful);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Bastion of the Penitent")) {
						if (ImGui::BeginTable("tableW4", 5, tableFlags)) {
							RenderRaidsHeaderPerWing(BastionOfThePenitent);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, BastionOfThePenitent);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, BastionOfThePenitent);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Hall of Chains")) {
						if (ImGui::BeginTable("tableW6", 7, tableFlags)) {
							RenderRaidsHeaderPerWing(HallOfChains);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, HallOfChains);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, HallOfChains);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Mythwright Gambit")) {
						if (ImGui::BeginTable("tableW7", 4, tableFlags)) {
							RenderRaidsHeaderPerWing(MythwrightGambit);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, MythwrightGambit);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, MythwrightGambit);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("The Key  of Ahdashim")) {
						if (ImGui::BeginTable("table", 4, tableFlags)) {
							RenderRaidsHeaderPerWing(TheKeyOfAhdashim);
							if (!selfName.empty()) {
								RenderRaidsRowPerWing(&self, TheKeyOfAhdashim);
							}
							for (Player player : players) {
								RenderRaidsRowPerWing(&player, TheKeyOfAhdashim);
							}
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
		}
	}
	ImGui::End();
}

void RenderSettings() {
	if (ImGui::Button(Config.showWindow ? "Hide Window" : "Show Window")) {
		Config.showWindow = !Config.showWindow;
	}
	if (ImGui::Button(Config.splitPerWing ? "Show All Wings" : "Split Per Wing")) {
		Config.splitPerWing = !Config.splitPerWing;
	}
}

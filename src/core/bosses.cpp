#include "bosses.h"

#include <algorithm>

using namespace WingmanModes;

namespace {
	constexpr BossInfo bosses[] = {
			/* Raids */
			{.id = 15438, .name = "Vale Guardian", .texture = {"TEX_VALE_GUARDIAN", IDB_VALE_GUARDIAN}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Vale Guardian Fragment", .kpmeCoffer = "Vale Guardian Coffer"},
			{.id = 15429, .name = "Gorseval", .texture = {"TEX_GORSEVAL", IDB_GORSEVAL}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Gorseval Tentacle Piece", .kpmeCoffer = "Gorseval's Coffer"},
			{.id = 15375, .name = "Sabetha", .texture = {"TEX_SABETHA", IDB_SABETHA}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Sabetha Flamethrower Fragment Piece", .kpmeCoffer = "Sabetha's Coffer"},
			{.id = 16123, .name = "Slothasor", .texture = {"TEX_SLOTHASOR", IDB_SLOTHASOR}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Slothasor Mushroom", .kpmeCoffer = "Slothasor's Coffer"},
			{.id = 16088, .name = "Bandit Trio", .texture = {"TEX_NARELLA", IDB_NARELLA}, .content = Content::RAID, .wingmanModes = NORMAL},
			{.id = 16115, .name = "Matthias", .texture = {"TEX_MATTHIAS", IDB_MATTHIAS}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "White Mantle Abomination Crystal", .kpmeCoffer = "Matthias's Coffer"},
			{.id = 16253, .name = "Escort", .texture = {"TEX_ESCORT", IDB_ESCORT}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Turret Fragment", .kpmeCoffer = "McLeod's Coffer"},
			{.id = 16235, .name = "Keep Construct", .texture = {"TEX_KEEP_CONSTRUCT", IDB_KEEP_CONSTRUCT}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Keep Construct Rubble", .kpmeCoffer = "Keep Construct's Coffer"},
			{.id = 16247, .name = "Twisted Castle", .texture = {"TEX_TWISTED_CASTLE", IDB_TWISTED_CASTLE}, .content = Content::RAID, .wingmanModes = NORMAL},
			{.id = 16246, .name = "Xera", .texture = {"TEX_XERA", IDB_XERA}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Ribbon Scrap", .kpmeCoffer = "Xera's Coffer"},
			{.id = 17194, .name = "Cairn", .texture = {"TEX_CAIRN", IDB_CAIRN}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Cairn Fragment", .kpmeCoffer = "Cairn's Coffer"},
			{.id = 17172, .name = "Mursaat Overseer", .texture = {"TEX_MURSAAT_OVERSEER", IDB_MURSAAT_OVERSEER}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Recreation Room Floor Fragment", .kpmeCoffer = "Mursaat Overseer's Coffer"},
			{.id = 17188, .name = "Samarog", .texture = {"TEX_SAMAROG", IDB_SAMAROG}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Impaled Prisoner Token", .kpmeCoffer = "Samarog's Coffer"},
			{.id = 17154, .name = "Deimos", .texture = {"TEX_DEIMOS", IDB_DEIMOS}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Fragment of Saul's Burden", .kpmeCoffer = "Deimos's Coffer"},
			{.id = 19767, .name = "Soulless Horror", .texture = {"TEX_SOULLESS_HORROR", IDB_SOULLESS_HORROR}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Desmina's Token", .kpmeCoffer = "Desmina's Coffer"},
			{.id = 19828, .name = "River of Souls", .texture = {"TEX_RIVER_OF_SOULS", IDB_RIVER_OF_SOULS}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "River of Souls Token", .kpmeCoffer = "River of Souls Coffer"},
			{.id = 19691, .name = "Broken King", .texture = {"TEX_BROKEN_KING", IDB_BROKEN_KING}, .content = Content::RAID, .wingmanModes = NORMAL, .kpmeToken = "Statue Token", .kpmeCoffer = "Statue of Grenth Coffer"},
			{.id = 19536, .name = "Eater of Souls", .texture = {"TEX_EATER_OF_SOULS", IDB_EATER_OF_SOULS}, .content = Content::RAID, .wingmanModes = NORMAL},
			{.id = 19651, .name = "Eyes", .texture = {"TEX_EYES_OF_FATE", IDB_EYES_OF_FATE}, .content = Content::RAID, .wingmanModes = NORMAL},
			{.id = 19450, .name = "Dhuum", .texture = {"TEX_DHUUM", IDB_DHUUM}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Dhuum's Token", .kpmeCoffer = "Dhuum's Coffer"},
			{.id = 43974, .name = "Conjured Amalgamate", .texture = {"TEX_CONJURED_AMALGAMATE", IDB_CONJURED_AMALGAMATE}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Conjured Amalgamate Token", .kpmeCoffer = "Conjured Amalgamate's Coffer"},
			{.id = 21105, .name = "Twin Largos", .texture = {"TEX_KENUTT", IDB_KENUTT}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Twin Largos Token", .kpmeCoffer = "Twin Largos' Coffer"},
			{.id = 20934, .name = "Qadim", .texture = {"TEX_QADIM", IDB_QADIM}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Qadim's Token", .kpmeCoffer = "Qadim's Coffer"},
			{.id = 22006, .name = "Adina", .texture = {"TEX_ADINA", IDB_ADINA}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Cardinal Adina's Token", .kpmeCoffer = "Cardinal Adina's Coffer"},
			{.id = 21964, .name = "Sabir", .texture = {"TEX_SABIR", IDB_SABIR}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Cardinal Sabir's Token", .kpmeCoffer = "Cardinal Sabir's Coffer"},
			{.id = 22000, .name = "Qadim the Peerless", .texture = {"TEX_QADIM_THE_PEERLESS", IDB_QADIM_THE_PEERLESS}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Ether Djinn's Token", .kpmeCoffer = "Qadim the Peerless's Coffer"},
			{.id = 26725, .name = "Greer", .texture = {"TEX_GREER", IDB_GREER}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Greer's Token", .kpmeCoffer = "Greer's Coffer", .kpmeCmCoffer = "Greer's Magnificent Coffer"},
			{.id = 26774, .name = "Decima", .texture = {"TEX_DECIMA", IDB_DECIMA}, .content = Content::RAID, .wingmanModes = NORMAL | CM, .kpmeToken = "Decima's Token", .kpmeCoffer = "Decima's Coffer", .kpmeCmCoffer = "Decima's Magnificent Coffer"},
			{.id = 26712, .name = "Ura", .texture = {"TEX_URA", IDB_URA}, .content = Content::RAID, .wingmanModes = NORMAL | CM | LCM, .kpmeToken = "Ura's Token", .kpmeCoffer = "Ura's Coffer", .kpmeCmCoffer = "Ura's Magnificent Coffer"},
			/* Fractals */
			{.id = 17021, .name = "MAMA", .texture = {"TEX_MAMA", IDB_MAMA}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 17028, .name = "Siax", .texture = {"TEX_SIAX", IDB_SIAX}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 16948, .name = "Ensolyss", .texture = {"TEX_ENSOLYSS", IDB_ENSOLYSS}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 17632, .name = "Skorvald", .texture = {"TEX_SKORVALD", IDB_SKORVALD}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 17949, .name = "Artsariiv", .texture = {"TEX_ARTSARIIV", IDB_ARTSARIIV}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 17759, .name = "Arkk", .texture = {"TEX_ARKK", IDB_ARKK}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 23254, .name = "Elemental Ai", .texture = {"TEX_ELEMENTAL_AI_LIGHT", IDB_ELEMENTAL_AI_LIGHT}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 232542, .name = "Dark Ai", .texture = {"TEX_ELEMENTAL_AI_DARK", IDB_ELEMENTAL_AI_DARK}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 232543, .name = "Full Ai", .texture = {"TEX_ELEMENTAL_AI_LIGHT_AND_DARK", IDB_ELEMENTAL_AI_LIGHT_AND_DARK}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 25577, .name = "Kanaxai", .texture = {"TEX_KANAXAI", IDB_KANAXAI}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 26231, .name = "Eparch", .texture = {"TEX_EPARCH", IDB_EPARCH}, .content = Content::FRACTAL, .wingmanModes = CM},
			{.id = 27010, .name = "Kinfall", .texture = {"TEX_KINFALL", IDB_KINFALL}, .content = Content::FRACTAL, .wingmanModes = CM},
			/* Strikes */
			{.id = 22154, .name = "Icebrood Construct", .texture = {"TEX_ICEBROOD_CONSTRUCT", IDB_ICEBROOD_CONSTRUCT}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 22343, .name = "Kodans / The Voice and The Claw", .texture = {"TEX_KODANS", IDB_KODANS}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 22492, .name = "Fraenir of Jormag", .texture = {"TEX_FRAENIR_OF_JORMAG", IDB_FRAENIR_OF_JORMAG}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 22521, .name = "Boneskinner", .texture = {"TEX_BONESKINNER", IDB_BONESKINNER}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 22711, .name = "Whisper of Jormag", .texture = {"TEX_WHISPER_OF_JORMAG", IDB_WHISPER_OF_JORMAG}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 21333, .name = "Freezie", .texture = {"TEX_FREEZIE", IDB_FREEZIE}, .content = Content::STRIKE, .wingmanModes = NORMAL},
			{.id = 24033, .name = "Captain Mai Trin", .texture = {"TEX_CAPTAIN_MAI_TRIN", IDB_CAPTAIN_MAI_TRIN}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Mai Trin's Coffer", .kpmeCmCoffer = "Mai Trin's Magnificent Coffer"},
			{.id = 23957, .name = "Ankka", .texture = {"TEX_ANKKA", IDB_ANKKA}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Ankka's Coffer", .kpmeCmCoffer = "Ankka's Magnificent Coffer"},
			{.id = 24485, .name = "Minister Li", .texture = {"TEX_MINISTER_LI", IDB_MINISTER_LI}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Minister Li's Coffer", .kpmeCmCoffer = "Minister Li's Magnificent Coffer"},
			{.id = 24375, .name = "Harvest Temple / Void Amalgamate", .texture = {"TEX_HARVEST_TEMPLE", IDB_HARVEST_TEMPLE}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Void's Coffer", .kpmeCmCoffer = "Void's Magnificent Coffer"},
			{.id = 25413, .name = "Old Lion's Court", .texture = {"TEX_WATCHKNIGHT_MARK_II", IDB_WATCHKNIGHT_MARK_II}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Assault Knights' Coffer", .kpmeCmCoffer = "Assault Knights' Magnificent Coffer"},
			{.id = 25705, .name = "Dagda", .texture = {"TEX_DAGDA", IDB_DAGDA}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeCoffer = "Dagda's Coffer", .kpmeCmCoffer = "Dagda's Magnificent Coffer"},
			{.id = 25989, .name = "Cerus", .texture = {"TEX_CERUS", IDB_CERUS}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM | LCM, .kpmeCoffer = "Cerus's Coffer", .kpmeCmCoffer = "Cerus's Magnificent Coffer"},
			{.id = 27124, .name = "Kela", .texture = {"TEX_KELA", IDB_KELA}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM, .kpmeToken = "Sandcastle", .kpmeCoffer = "Kela's Coffer", .kpmeCmCoffer = "Kela's Magnificent Coffer"},
			{.id = 28106, .name = "Vloxx", .texture = {"TEX_VLOXX", IDB_VLOXX}, .content = Content::STRIKE, .wingmanModes = NORMAL | CM | LCM, .kpmeCoffer = "Vloxx's Coffer", .kpmeCmCoffer = "Vloxx's Magnificent Coffer"},
	};
} // namespace

BossType BossTypeFromString(const std::string& s) {
	if (s == "CM") return BossType::CM;
	if (s == "LCM") return BossType::LCM;
	return BossType::NORMAL;
}

bool BossInfo::HasWingmanMode(BossType type) const {
	switch (type) {
		case BossType::CM:
			return wingmanModes & WingmanModes::CM;
		case BossType::LCM:
			return wingmanModes & WingmanModes::LCM;
		default:
			return wingmanModes & WingmanModes::NORMAL;
	}
}

std::span<const BossInfo> AllBosses() {
	return bosses;
}

const BossInfo* FindBoss(int id) {
	auto it = std::find_if(std::begin(bosses), std::end(bosses), [id](const BossInfo& boss) { return boss.id == id; });
	return it != std::end(bosses) ? &*it : nullptr;
}

std::string GetBossName(const BossInfo& boss, BossType type) {
	switch (type) {
		case BossType::CM:
			return std::string(boss.name) + " CM";
		case BossType::LCM:
			return std::string(boss.name) + " LCM";
		default:
			return boss.name;
	}
}

std::string GetBossName(const BossInfo& boss, BossType type, ProofKind kind) {
	std::string name = GetBossName(boss, type);
	switch (kind) {
		case ProofKind::TOKEN:
			return name + " Token";
		case ProofKind::COFFER:
			return name + " Coffer";
		default:
			return name;
	}
}

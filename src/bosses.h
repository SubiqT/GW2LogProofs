#ifndef BOSSES_H
#define BOSSES_H

#include <vector>

#include "nexus/Nexus.h"

enum Boss {
	/* Raids */
	ValeGuardian = 15438,
	Gorseval = 15429,
	Sabetha = 15375,
	Slothasor = 16123,
	BanditTrio = 16088,
	Matthias = 16115,
	Escort = 16253,
	KeepConstruct = 16235,
	KeepConstructCM = -16235,
	TwistedCastle = 16247,
	Xera = 16246,
	Cairn = 17194,
	CairnCM = -17194,
	MursaatOverseer = 17172,
	MursaatOverseerCM = -17172,
	Samarog = 17188,
	SamarogCM = -17188,
	Deimos = 17154,
	DeimosCM = -17154,
	SoullessHorror = 19767,
	SoullessHorrorCM = -19767,
	RiverOfSouls = 19828,
	BrokenKing = 19691,
	EaterOfSouls = 19536,
	Eyes = 19651,
	Dhuum = 19450,
	DhuumCM = -19450,
	ConjuredAmalgamate = 43974,
	ConjuredAmalgamateCM = -43974,
	TwinLargos = 21105,
	TwinLargosCM = -21105,
	Qadim = 20934,
	QadimCM = -20934,
	Adina = 22006,
	AdinaCM = -22006,
	Sabir = 21964,
	SabirCM = -21964,
	QadimThePeerless = 22000,
	QadimThePeerlessCM = -22000,
	Greer = 26725,
	Decima = 26774,
	Ura = 26712,
	/* Fractals  */
	MAMA = 17021,
	MAMACM = -17021,
	SiaxTheCorrupted = 17028,
	SiaxTheCorruptedCM = -17028,
	EnsolyssOfTheEndlessTorment = 16948,
	EnsolyssOfTheEndlessTormentCM = -16948,
	SkorvaldTheShattered = 17632,
	SkorvaldTheShatteredCM = -17632,
	Artsariiv = 17949,
	ArtsariivCM = -17949,
	Arkk = 17759,
	ArkkCM = -17759,
	ElementalAiKeeperOfThePeak = 23254,
	ElementalAiKeeperOfThePeakCM = -23254,
	DarkAiKeeperOfThePeak = 232542,
	DarkAiKeeperOfThePeakCM = -232542,
	ElementalAndDarkAiKeeperOfThePeak = 232543,
	ElementalAndDarkAiKeeperOfThePeakCM = -232543,
	KanaxaiScytheOfHouseAurkus = 25577,
	KanaxaiScytheOfHouseAurkusCM = -25577,
	CerusAndDeimos = 26257,
	CerusAndDeimosCM = -26257,
	Eparch = 26231,
	EparchCM = -26231,
	/* Strikes */
	IcebroodConstruct = 22154,
	TheVoiceAndTheClaw = 22343,
	FraenirOfJormag = 22492,
	Boneskinner = 22521,
	WhisperOfJormag = 22711,
	Freezie = 21333,
	CaptainMaiTrin = 24033,
	CaptainMaiTrinCM = -24033,
	Ankka = 23957,
	AnkkaCM = -23957,
	MinisterLi = 24485,
	MinisterLiCM = -24485,
	VoidAmalgamate = 24375,
	VoidAmalgamateCM = -24375,
	OldLionsCourt = 25413,
	OldLionsCourtCM = -25413,
	Dagda = 25705,
	DagdaCM = -25705,
	Cerus = 25989,
	CerusCM = -25989
};

extern std::vector<Boss> sortedRaidBosses;
extern std::vector<Boss> sortedRaidCmBosses;
extern std::vector<Boss> sortedFractalCMBosses;
extern std::vector<Boss> sortedStrikeBosses;
extern std::vector<Boss> sortedStrikeCMBosses;
extern std::vector<Boss> sortedStrikeLMBosses;

extern std::vector<std::string> sortedKpmeSummary;
extern std::vector<Boss> sortedKpmeRaidBosses;
extern std::vector<Boss> sortedKpmeStrikeBosses;
extern std::vector<Boss> sortedKpmeStrikeCMBosses;

const char* GetBossName(Boss boss);
Texture* GetBossTexture(Boss boss);
std::string GetKpMeBossToken(Boss boss);
std::string GetKpMeBossCoffer(Boss boss);
Texture* GetCurrencyTexture(std::string name);

#endif
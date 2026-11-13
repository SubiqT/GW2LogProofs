#ifndef BOSS_GROUP_H
#define BOSS_GROUP_H

#include "bosses.h"
#include <string>
#include <vector>

struct BossEntry {
	const BossInfo* boss;
	BossType type;
	ProofKind kind;
	std::string proofId;
};

struct BossGroup {
	std::string name;
	std::string tableName;
	std::vector<BossEntry> bosses;
	std::vector<std::string> currencies; // Currency names double as their proof ids.
};

#endif

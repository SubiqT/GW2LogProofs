#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "../providers/common/data_provider.h"
#include <memory>

enum class LoadState {
	LOADING = 0,
	READY = 1,
	FAILED = 2
};

struct Player {
	uintptr_t id;
	std::string account;
	LoadState state;
	std::unique_ptr<PlayerProofData> proofData;
	std::string providerName;
};

#endif
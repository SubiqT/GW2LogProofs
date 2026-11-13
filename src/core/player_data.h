#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <string>

enum class LoadState {
	LOADING = 0,
	READY = 1,
	FAILED = 2
};

struct Player {
	std::string account;
};

#endif
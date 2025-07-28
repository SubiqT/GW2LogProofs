#ifndef LOG_PROOFS_H
#define LOG_PROOFS_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "unofficial_extras/Definitions.h"

#include "kp_loader.h"
#include "threadpool.hpp"

namespace LogProofs {
	enum KpState {
		LOADING = 0,
		READY = 1
	};

	struct Player {
		uintptr_t id;
		std::string account;
		KpState wingmanState;
		KpState kpmeState;
		Wingman::WingmanResponse wingman;
		Kpme::KpmeResponse kpme;
	};

	extern std::vector<Player> players;
	extern std::string selfAccountName;
	extern std::mutex Mutex;
	extern Threadpool threadpool;

	void UnExSquadEventHandler(void* eventArgs);
	void ArcSquadJoinEventHandler(void* eventArgs);
	void ArcSquadLeaveEventHandler(void* eventArgs);
	void ArcSelfLeaveEventHandler(void* eventArgs);
	void ArcSelfDetectedEventHandler(void* eventArgs);
} // namespace LogProofs

/* Unofficial Extras */
struct SquadUpdate {
	UserInfo* UserInfo;
	uint64_t UsersCount;
};

#endif
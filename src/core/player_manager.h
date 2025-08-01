#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "lazy_load_manager.h"
#include "player_data.h"
#include "player_info.h"
#include <mutex>
#include <string>
#include <vector>

namespace PlayerManager {
	extern std::vector<Player> players;
	extern std::string selfAccountName;
	extern std::mutex playerMutex;
	extern LazyLoadManager lazyLoadManager;

	void AddPlayer(const PlayerInfo& playerInfo);
	void RemovePlayer(const PlayerInfo& playerInfo);
	void SetSelf(const PlayerInfo& playerInfo);
	void ClearPlayers();
	void OnWindowStateChanged(bool isOpen);
} // namespace PlayerManager

#endif
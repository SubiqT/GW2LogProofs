#include "event_handlers.h"
#include "../arcdps/arcdps.h"
#include "../unofficial_extras/Definitions.h"
#include "player_manager.h"
#include "player_tracker_manager.h"
#include "shared.h"
#include <chrono>
#include <format>

bool unofficialExtrasEnabled = false;
PlayerTrackerManager trackerManager;

static std::string StripAccount(std::string account) {
	if (!account.empty() && account.at(0) == ':') {
		return account.erase(0, 1);
	}
	return account;
}

static PlayerInfo CreatePlayerInfo(const std::string& account, const std::string& source, const std::string& character = "", uintptr_t id = 0, uintptr_t instanceId = 0) {
	return {account, character, static_cast<uint32_t>(id), static_cast<uint32_t>(instanceId), source,
			static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
										  std::chrono::system_clock::now().time_since_epoch()
			)
										  .count())};
}

void UnExSquadEventHandler(void* eventArgs) {
	try {
		SquadUpdate* squadUpdate = (SquadUpdate*) eventArgs;
		unofficialExtrasEnabled = true;
		for (size_t i = 0; i < squadUpdate->UsersCount; i++) {
			if (!squadUpdate || !squadUpdate->UserInfo || !squadUpdate->UserInfo[i].AccountName) return;
			std::string account = StripAccount(squadUpdate->UserInfo[i].AccountName);
			int role = int(squadUpdate->UserInfo[i].Role);

			PlayerInfo playerInfo = CreatePlayerInfo(account, "Unofficial Extras");
			if (role == 5) {
				if (PlayerManager::selfAccountName == account) {
					trackerManager.HandleSquadClear();
				} else {
					trackerManager.HandlePlayerLeave(playerInfo);
				}
			} else if (role <= 2) {
				trackerManager.HandlePlayerJoin(playerInfo);
			}
		}
	} catch (const std::exception& e) {
		APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in Unofficial Extras event handler: {}", e.what()).c_str());
	}
}

void ArcSquadJoinEventHandler(void* eventArgs) {
	try {
		EvAgentUpdateData* data = (EvAgentUpdateData*) eventArgs;
		PlayerInfo playerInfo = CreatePlayerInfo(StripAccount(data->account), "ArcDPS", data->character, data->id, data->instanceId);
		trackerManager.HandlePlayerJoin(playerInfo);
	} catch (const std::exception& e) {
		APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS squad join handler: {}", e.what()).c_str());
	}
}

void ArcSquadLeaveEventHandler(void* eventArgs) {
	try {
		EvAgentUpdateData* data = (EvAgentUpdateData*) eventArgs;
		PlayerInfo playerInfo = CreatePlayerInfo(StripAccount(data->account), "ArcDPS", data->character, data->id, data->instanceId);
		trackerManager.HandlePlayerLeave(playerInfo);
	} catch (const std::exception& e) {
		APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS squad leave handler: {}", e.what()).c_str());
	}
}

void ArcSelfLeaveEventHandler(void* eventArgs) {
	try {
		trackerManager.HandleSquadClear();
	} catch (const std::exception& e) {
		APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS self leave handler: {}", e.what()).c_str());
	}
}

void ArcSelfDetectedEventHandler(void* eventArgs) {
	try {
		EvAgentUpdateData* data = (EvAgentUpdateData*) eventArgs;
		PlayerInfo playerInfo = CreatePlayerInfo(StripAccount(data->account), "ArcDPS", data->character, data->id, data->instanceId);
		trackerManager.HandleSelfDetected(playerInfo);
	} catch (const std::exception& e) {
		APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Critical error in ArcDPS self detection handler: {}", e.what()).c_str());
	}
}

void InitializeTrackerManager() {
	trackerManager.Initialize();
}

void ShutdownTrackerManager() {
	trackerManager.Shutdown();
}
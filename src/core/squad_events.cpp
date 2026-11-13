#include "squad_events.h"
#include "../arcdps/arcdps.h"
#include "../realtime_api/RTAPI.h"
#include "../unofficial_extras/Definitions.h"
#include "player_manager.h"
#include "shared.h"
#include <format>

namespace {
	struct SquadUpdate {
		UserInfo* UserInfo;
		uint64_t UsersCount;
	};

	std::string StripAccount(const char* account) {
		std::string_view name = account ? account : "";
		if (name.starts_with(':')) name.remove_prefix(1);
		return std::string(name);
	}

	// Nexus invokes these from C, so exceptions must not escape.
	template <void (*Handle)(void*), bool RequiresPayload = true>
	void Guarded(void* eventArgs) {
		if (RequiresPayload && !eventArgs) return;
		try {
			Handle(eventArgs);
		} catch (const std::exception& e) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Squad event handler failed: {}", e.what()).c_str());
		}
	}

	void OnUnofficialExtrasSquadUpdate(void* eventArgs) {
		auto* update = static_cast<SquadUpdate*>(eventArgs);
		if (!update->UserInfo) return;
		for (uint64_t i = 0; i < update->UsersCount; i++) {
			const UserInfo& user = update->UserInfo[i];
			if (!user.AccountName) continue;
			std::string account = StripAccount(user.AccountName);
			if (user.Role == UserRole::None) {
				if (PlayerManager::IsSelf(account)) {
					PlayerManager::ClearPlayers();
				} else {
					PlayerManager::RemovePlayer(account);
				}
			} else if (user.Role == UserRole::SquadLeader || user.Role == UserRole::Lieutenant || user.Role == UserRole::Member) {
				PlayerManager::AddPlayer(account);
			}
		}
	}

	void OnArcSquadJoin(void* eventArgs) {
		PlayerManager::AddPlayer(StripAccount(static_cast<EvAgentUpdateData*>(eventArgs)->account));
	}

	void OnArcSquadLeave(void* eventArgs) {
		PlayerManager::RemovePlayer(StripAccount(static_cast<EvAgentUpdateData*>(eventArgs)->account));
	}

	void OnArcSelfJoin(void* eventArgs) {
		PlayerManager::SetSelf(StripAccount(static_cast<EvAgentUpdateData*>(eventArgs)->account));
	}

	void OnArcSelfLeave(void*) {
		PlayerManager::ClearPlayers();
	}

	void OnGroupMemberJoinedOrUpdated(void* eventArgs) {
		auto* member = static_cast<GroupMember*>(eventArgs);
		std::string account = StripAccount(member->AccountName);
		if (member->IsSelf) {
			PlayerManager::SetSelf(account);
		} else {
			PlayerManager::AddPlayer(account);
		}
	}

	void OnGroupMemberLeft(void* eventArgs) {
		auto* member = static_cast<GroupMember*>(eventArgs);
		if (member->IsSelf) {
			PlayerManager::ClearPlayers();
		} else {
			PlayerManager::RemovePlayer(StripAccount(member->AccountName));
		}
	}

	struct Subscription {
		const char* event;
		EVENT_CONSUME handler;
	};

	const Subscription subscriptions[] = {
			{"EV_UNOFFICIAL_EXTRAS_SQUAD_UPDATE", Guarded<OnUnofficialExtrasSquadUpdate>},
			{"EV_ARCDPS_SQUAD_JOIN",              Guarded<OnArcSquadJoin>               },
			{"EV_ARCDPS_SQUAD_LEAVE",             Guarded<OnArcSquadLeave>              },
			{"EV_ARCDPS_SELF_JOIN",               Guarded<OnArcSelfJoin>                },
			{"EV_ARCDPS_SELF_LEAVE",              Guarded<OnArcSelfLeave, false>        },
			{EV_RTAPI_GROUP_MEMBER_JOINED,        Guarded<OnGroupMemberJoinedOrUpdated> },
			{EV_RTAPI_GROUP_MEMBER_UPDATED,       Guarded<OnGroupMemberJoinedOrUpdated> },
			{EV_RTAPI_GROUP_MEMBER_LEFT,          Guarded<OnGroupMemberLeft>            },
	};
} // namespace

void SubscribeSquadEvents() {
	for (const auto& subscription : subscriptions) {
		APIDefs->Events.Subscribe(subscription.event, subscription.handler);
	}
	// Ask ArcDPS to resend the current squad so players who joined before the addon loaded are tracked.
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SELF_JOIN", nullptr);
	APIDefs->Events.Raise("EV_REPLAY_ARCDPS_SQUAD_JOIN", nullptr);
}

void UnsubscribeSquadEvents() {
	for (const auto& subscription : subscriptions) {
		APIDefs->Events.Unsubscribe(subscription.event, subscription.handler);
	}
}

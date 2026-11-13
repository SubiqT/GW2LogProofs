#include "squad_events.h"
#include "../arcdps/arcdps.h"
#include "../realtime_api/RTAPI.h"
#include "../unofficial_extras/Definitions.h"
#include "player_manager.h"
#include "shared.h"
#include <cstring>
#include <format>
#include <string_view>

namespace {
	struct SquadUpdate {
		UserInfo* UserInfo;
		uint64_t UsersCount;
	};

	std::string StripAccount(std::string_view name) {
		if (name.starts_with(':')) name.remove_prefix(1);
		return std::string(name);
	}

	std::string StripAccount(const char* account) {
		return StripAccount(std::string_view(account ? account : ""));
	}

	template <size_t N>
	std::string_view FixedString(const char (&text)[N]) {
		return {text, strnlen(text, N)};
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
				// Unofficial Extras numbers squad subgroups from 0.
				uint8_t subgroup = user.GroupType == ChannelType::Squad ? user.Subgroup + 1 : 0;
				PlayerManager::UpdatePlayer({account, std::nullopt, subgroup, user.Role == UserRole::SquadLeader});
			}
		}
	}

	void OnArcSquadJoin(void* eventArgs) {
		auto* agent = static_cast<EvAgentUpdateData*>(eventArgs);
		PlayerManager::UpdatePlayer({StripAccount(FixedString(agent->account)), std::string(FixedString(agent->character))});
	}

	void OnArcSquadLeave(void* eventArgs) {
		PlayerManager::RemovePlayer(StripAccount(FixedString(static_cast<EvAgentUpdateData*>(eventArgs)->account)));
	}

	void OnArcSelfJoin(void* eventArgs) {
		auto* agent = static_cast<EvAgentUpdateData*>(eventArgs);
		PlayerManager::SetSelf({StripAccount(FixedString(agent->account)), std::string(FixedString(agent->character))});
	}

	void OnArcSelfLeave(void*) {
		PlayerManager::ClearPlayers();
	}

	void OnGroupMemberJoinedOrUpdated(void* eventArgs) {
		auto* member = static_cast<GroupMember*>(eventArgs);
		// RealTime API already numbers squad subgroups from 1 and reports 0 for parties.
		SquadMemberUpdate update {StripAccount(FixedString(member->AccountName)), std::string(FixedString(member->CharacterName)), static_cast<uint8_t>(member->Subgroup), member->IsCommander != 0};
		if (member->IsSelf) {
			PlayerManager::SetSelf(update);
		} else {
			PlayerManager::UpdatePlayer(update);
		}
	}

	void OnGroupMemberLeft(void* eventArgs) {
		auto* member = static_cast<GroupMember*>(eventArgs);
		if (member->IsSelf) {
			PlayerManager::ClearPlayers();
		} else {
			PlayerManager::RemovePlayer(StripAccount(FixedString(member->AccountName)));
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

#include "player_manager.h"
#include "../providers/common/provider_registry.h"
#include "settings.h"
#include "shared.h"
#include <algorithm>
#include <cctype>
#include <format>
#include <mutex>

namespace PlayerManager {
	namespace {
		std::mutex mutex;
		std::vector<SquadMember> members;
		std::string selfAccount;
		constexpr size_t MAX_ID_LENGTH = 64;

		void LeaveSquad(SquadMember& member) {
			member.inSquad = false;
			member.subgroup = 0;
			member.commander = false;
		}

		void UpdateLocked(const SquadMemberUpdate& update) {
			auto it = std::find_if(members.begin(), members.end(), [&](const SquadMember& m) { return m.account == update.account; });
			if (it == members.end()) {
				members.push_back({update.account});
				it = std::prev(members.end());
				APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player added to tracking: {}", update.account).c_str());
			}
			it->inSquad = true;
			if (update.character && !update.character->empty()) it->character = *update.character;
			if (update.subgroup) it->subgroup = *update.subgroup;
			if (update.commander) it->commander = *update.commander;
		}
	} // namespace

	LazyLoadManager lazyLoadManager(
			[](const std::string& account, const std::string& provider, FetchCallback callback) {
				IProvider* source = ProviderRegistry::Instance().Find(provider);
				if (!source) {
					callback({FetchStatus::FAILED});
					return;
				}
				source->FetchAsync(account, std::move(callback));
			},
			[] { return LoadPolicy {std::chrono::minutes(Settings::CacheTimeoutMinutes), Settings::MaxRetryAttempts}; }
	);

	void UpdatePlayer(const SquadMemberUpdate& update) {
		if (update.account.empty()) return;
		std::scoped_lock lock(mutex);
		UpdateLocked(update);
	}

	void RemovePlayer(const std::string& account) {
		std::scoped_lock lock(mutex);
		auto it = std::find_if(members.begin(), members.end(), [&](const SquadMember& m) { return m.account == account; });
		if (it == members.end()) return;
		if (it->addedManually) {
			LeaveSquad(*it);
			return;
		}
		members.erase(it);
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Player removed from tracking: {}", account).c_str());
	}

	void SetSelf(const SquadMemberUpdate& update) {
		if (update.account.empty()) return;
		std::scoped_lock lock(mutex);
		selfAccount = update.account;
		UpdateLocked(update);
	}

	bool IsSelf(const std::string& account) {
		std::scoped_lock lock(mutex);
		return account == selfAccount;
	}

	void ClearPlayers() {
		std::scoped_lock lock(mutex);
		std::erase_if(members, [](const SquadMember& m) { return m.account != selfAccount && !m.addedManually; });
		for (auto& member : members) {
			if (member.account != selfAccount) LeaveSquad(member);
			member.subgroup = 0;
			member.commander = false;
		}
	}

	bool AddManualPlayer(std::string_view id) {
		while (!id.empty() && std::isspace(static_cast<unsigned char>(id.front()))) id.remove_prefix(1);
		while (!id.empty() && std::isspace(static_cast<unsigned char>(id.back()))) id.remove_suffix(1);
		if (id.empty() || id.size() > MAX_ID_LENGTH) return false;
		if (std::any_of(id.begin(), id.end(), [](unsigned char c) { return std::iscntrl(c); })) return false;

		std::scoped_lock lock(mutex);
		auto it = std::find_if(members.begin(), members.end(), [&](const SquadMember& m) { return m.account == id || m.character == id; });
		if (it == members.end()) {
			members.push_back({std::string(id)});
			it = std::prev(members.end());
		}
		it->addedManually = true;
		return true;
	}

	void ClearManualPlayers() {
		std::scoped_lock lock(mutex);
		std::erase_if(members, [](const SquadMember& m) { return m.addedManually && !m.inSquad; });
		for (auto& member : members) member.addedManually = false;
	}

	bool HasManualPlayers() {
		std::scoped_lock lock(mutex);
		return std::any_of(members.begin(), members.end(), [](const SquadMember& m) { return m.addedManually; });
	}

	std::vector<SquadMember> Members() {
		std::scoped_lock lock(mutex);
		return members;
	}
} // namespace PlayerManager

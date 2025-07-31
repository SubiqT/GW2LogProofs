#include "tab_config.h"
#include "../providers/common/provider_registry.h"
#include "boss_registry.h"
#include "settings.h"
#include "shared.h"
#include <unordered_set>

using json = nlohmann::json;

// JSON serialization implementations
void to_json(json& j, const CustomTab& tab) {
	j = json {
			{"Id",          tab.id         },
			{"DisplayName", tab.displayName},
			{"ProofIds",    tab.proofIds   },
			{"Visible",     tab.visible    },
			{"Order",       tab.order      }
	};
}

void from_json(const json& j, CustomTab& tab) {
	j.at("Id").get_to(tab.id);
	j.at("DisplayName").get_to(tab.displayName);
	j.at("ProofIds").get_to(tab.proofIds);
	j.at("Visible").get_to(tab.visible);
	j.at("Order").get_to(tab.order);
}

void to_json(json& j, const ProviderTabConfig& config) {
	j = json {
			{"ProviderId",    config.providerId   },
			{"UseCustomTabs", config.useCustomTabs},
			{"CustomTabs",    config.tabs         }
	};
}

void from_json(const json& j, ProviderTabConfig& config) {
	j.at("ProviderId").get_to(config.providerId);
	j.at("UseCustomTabs").get_to(config.useCustomTabs);
	j.at("CustomTabs").get_to(config.tabs);
}

TabConfigManager& TabConfigManager::Instance() {
	static TabConfigManager instance;
	return instance;
}

void TabConfigManager::CreateTab(const std::string& providerId, const CustomTab& tab) {
	// Validate tab data
	if (tab.id.empty() || tab.displayName.empty()) {
		return; // Invalid tab data
	}

	// Ensure provider config exists
	if (providerConfigs_.find(providerId) == providerConfigs_.end()) {
		ProviderTabConfig newConfig;
		newConfig.providerId = providerId;
		newConfig.useCustomTabs = false;
		providerConfigs_[providerId] = newConfig;
	}

	auto& config = providerConfigs_[providerId];

	// Check for duplicate tab IDs
	for (const auto& existingTab : config.tabs) {
		if (existingTab.id == tab.id) {
			return; // Tab ID already exists
		}
	}

	config.tabs.push_back(tab);
	SaveAndPersist();
}

void TabConfigManager::UpdateTab(const std::string& providerId, const std::string& tabId, const CustomTab& tab) {
	// Validate input
	if (tabId.empty() || tab.displayName.empty()) {
		return;
	}

	auto it = providerConfigs_.find(providerId);
	if (it == providerConfigs_.end()) {
		return; // Provider not found
	}

	auto& config = it->second;
	for (auto& existingTab : config.tabs) {
		if (existingTab.id == tabId) {
			// Preserve the original ID
			CustomTab updatedTab = tab;
			updatedTab.id = tabId;
			existingTab = updatedTab;
			SaveAndPersist();
			return;
		}
	}
}

void TabConfigManager::DeleteTab(const std::string& providerId, const std::string& tabId) {
	if (tabId.empty()) {
		return;
	}

	auto it = providerConfigs_.find(providerId);
	if (it == providerConfigs_.end()) {
		return; // Provider not found
	}

	auto& config = it->second;
	auto originalSize = config.tabs.size();
	config.tabs.erase(
			std::remove_if(config.tabs.begin(), config.tabs.end(), [&tabId](const CustomTab& tab) { return tab.id == tabId; }),
			config.tabs.end()
	);

	// Only save if something was actually deleted
	if (config.tabs.size() != originalSize) {
		SaveAndPersist();
	}
}

void TabConfigManager::ReorderTabs(const std::string& providerId, const std::vector<std::string>& tabOrder) {
	auto& config = providerConfigs_[providerId];
	for (size_t i = 0; i < tabOrder.size(); ++i) {
		for (auto& tab : config.tabs) {
			if (tab.id == tabOrder[i]) {
				tab.order = static_cast<int>(i);
				break;
			}
		}
	}
	std::sort(config.tabs.begin(), config.tabs.end(), [](const CustomTab& a, const CustomTab& b) { return a.order < b.order; });
	SaveAndPersist();
}

ProviderTabConfig TabConfigManager::GetProviderConfig(const std::string& providerId) const {
	auto it = providerConfigs_.find(providerId);
	if (it != providerConfigs_.end()) {
		return it->second;
	}
	// Return default configuration for unknown providers
	ProviderTabConfig defaultConfig;
	defaultConfig.providerId = providerId;
	defaultConfig.useCustomTabs = false;
	defaultConfig.tabs.clear();
	return defaultConfig;
}

void TabConfigManager::SetProviderConfig(const std::string& providerId, const ProviderTabConfig& config) {
	providerConfigs_[providerId] = config;
	providerConfigs_[providerId].providerId = providerId; // Ensure provider ID is set
														  // Don't save immediately - let the caller handle saving to avoid mutex deadlock
}

std::vector<ProofOption> TabConfigManager::GetAvailableProofs(const std::string& providerId) const {
	auto* provider = BossRegistry::Instance().GetProvider(providerId);
	if (provider) {
		return provider->GetAvailableProofs();
	}
	return {};
}

void TabConfigManager::LoadFromSettings() {
	// Note: Called from Settings::Load() which already holds mutex
	if (Settings::Settings.contains("WindowLogProofs") && Settings::Settings["WindowLogProofs"].contains("ProviderConfigs")) {

		auto providerConfigs = Settings::Settings["WindowLogProofs"]["ProviderConfigs"];
		for (auto& [providerId, configJson] : providerConfigs.items()) {
			ProviderTabConfig config;
			from_json(configJson, config);
			providerConfigs_[providerId] = config;
		}
	}
}

void TabConfigManager::SaveToSettings() {
	std::scoped_lock lck(Settings::Mutex);
	json providerConfigsJson;
	for (const auto& [providerId, config] : providerConfigs_) {
		to_json(providerConfigsJson[providerId], config);
	}

	Settings::Settings["WindowLogProofs"]["ProviderConfigs"] = providerConfigsJson;
}

void TabConfigManager::SaveAndPersist() {
	SaveToSettings();
	Settings::Save(SettingsPath);
}

void TabConfigManager::ValidateAndCleanupConfigs() {
	for (auto& [providerId, config] : providerConfigs_) {
		ValidateTabConfig(config);
	}
}

void TabConfigManager::ValidateTabConfig(ProviderTabConfig& config) const {
	// Remove tabs with empty IDs or display names
	config.tabs.erase(
			std::remove_if(config.tabs.begin(), config.tabs.end(), [](const CustomTab& tab) {
				return tab.id.empty() || tab.displayName.empty();
			}),
			config.tabs.end()
	);

	// Remove duplicate tab IDs
	std::unordered_set<std::string> seenIds;
	config.tabs.erase(
			std::remove_if(config.tabs.begin(), config.tabs.end(), [&seenIds](const CustomTab& tab) {
				if (seenIds.count(tab.id)) {
					return true; // Duplicate
				}
				seenIds.insert(tab.id);
				return false;
			}),
			config.tabs.end()
	);

	// Skip proof ID validation during load - providers may not be ready yet
	// Validation will happen when tabs are actually used
}
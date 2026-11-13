#include "tab_config.h"
#include "../providers/common/provider_registry.h"
#include "settings.h"
#include "shared.h"
#include <format>

using json = nlohmann::json;

// JSON serialization implementations
void to_json(json& j, const ProofSelection& proof) {
	j = json {
			{"ProofId",  proof.proofId },
			{"BossType", proof.bossType}
	};
}

void from_json(const json& j, ProofSelection& proof) {
	j.at("ProofId").get_to(proof.proofId);
	j.at("BossType").get_to(proof.bossType);
}

void to_json(json& j, const CustomTab& tab) {
	j = json {
			{"Id",          tab.id         },
			{"DisplayName", tab.displayName},
			{"Proofs",      tab.proofs     },
			{"Visible",     tab.visible    },
			{"Order",       tab.order      }
	};
}

void from_json(const json& j, CustomTab& tab) {
	j.at("Id").get_to(tab.id);
	j.at("DisplayName").get_to(tab.displayName);
	j.at("Proofs").get_to(tab.proofs);
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
	config.tabs.clear();
	for (const auto& tabJson : j.at("CustomTabs")) {
		try {
			auto tab = tabJson.get<CustomTab>();
			if (!tab.id.empty() && !tab.displayName.empty()) config.tabs.push_back(std::move(tab));
		} catch (const json::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Skipping invalid custom tab: {}", e.what()).c_str());
		}
	}
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
	providerConfigs_[providerId].providerId = providerId;
}

std::vector<ProofOption> TabConfigManager::GetAvailableProofs(const std::string& providerId) const {
	auto* provider = ProviderRegistry::Instance().Find(providerId);
	if (provider) {
		return provider->GetAvailableProofs();
	}
	return {};
}

void TabConfigManager::Load(const json& providerConfigs) {
	if (!providerConfigs.is_object()) return;
	for (const auto& [providerId, configJson] : providerConfigs.items()) {
		try {
			providerConfigs_[providerId] = configJson.get<ProviderTabConfig>();
		} catch (const json::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Ignoring invalid tab configuration for {}: {}", providerId, e.what()).c_str());
		}
	}
}

json TabConfigManager::ToJson() const {
	json providerConfigs = json::object();
	for (const auto& [providerId, config] : providerConfigs_) {
		providerConfigs[providerId] = config;
	}
	return providerConfigs;
}

void TabConfigManager::SaveAndPersist() {
	Settings::Save();
}

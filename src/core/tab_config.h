#ifndef TAB_CONFIG_H
#define TAB_CONFIG_H

#include "../nlohmann/json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

struct CustomTab {
	std::string id;
	std::string displayName;
	std::vector<std::string> proofIds;
	bool visible = true;
	int order = 0;
};

struct ProviderTabConfig {
	std::string providerId;
	std::vector<CustomTab> tabs;
	bool useCustomTabs = false;
};

struct ProofOption {
	std::string id;
	std::string displayName;
	std::string category;
	std::string subcategory;
};

class TabConfigManager {
public:
	static TabConfigManager& Instance();

	void CreateTab(const std::string& providerId, const CustomTab& tab);
	void UpdateTab(const std::string& providerId, const std::string& tabId, const CustomTab& tab);
	void DeleteTab(const std::string& providerId, const std::string& tabId);
	void ReorderTabs(const std::string& providerId, const std::vector<std::string>& tabOrder);

	ProviderTabConfig GetProviderConfig(const std::string& providerId) const;
	void SetProviderConfig(const std::string& providerId, const ProviderTabConfig& config);

	std::vector<ProofOption> GetAvailableProofs(const std::string& providerId) const;

	void LoadFromSettings();
	void SaveToSettings();
	void SaveAndPersist();
	void ValidateAndCleanupConfigs();

	std::unordered_map<std::string, ProviderTabConfig> providerConfigs_;

private:
	void ValidateTabConfig(ProviderTabConfig& config) const;
};

// JSON serialization
void to_json(nlohmann::json& j, const CustomTab& tab);
void from_json(const nlohmann::json& j, CustomTab& tab);
void to_json(nlohmann::json& j, const ProviderTabConfig& config);
void from_json(const nlohmann::json& j, ProviderTabConfig& config);

#endif
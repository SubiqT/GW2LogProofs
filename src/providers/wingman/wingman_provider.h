#ifndef WINGMAN_PROVIDER_H
#define WINGMAN_PROVIDER_H

#include "../common/data_provider.h"
#include "wingman_client.h"

class WingmanProvider : public IDataProvider {
public:
	std::string GetProviderName() const override { return "Wingman"; }

	void LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) override;
	bool SupportsLinkedAccounts() const override { return false; }
	std::vector<std::string> GetSupportedProofTypes() const override;

private:
	Wingman::WingmanClient client_;
	PlayerProofData ConvertWingmanResponse(const Wingman::WingmanResponse& response);
};

#endif
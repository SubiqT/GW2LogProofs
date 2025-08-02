#ifndef KPME_PROVIDER_H
#define KPME_PROVIDER_H

#include "../common/data_provider.h"
#include "kpme_client.h"

class KpmeProvider : public IDataProvider {
public:
	std::string GetProviderName() const override { return "KPME"; }
	PlayerProofData LoadPlayerData(const std::string& account) override;
	void LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) override;
	bool SupportsLinkedAccounts() const override { return true; }
	std::vector<std::string> GetSupportedProofTypes() const override;

private:
	Kpme::KpmeClient client_;
	PlayerProofData ConvertKpmeResponse(const Kpme::KpmeResponse& response);
};

#endif
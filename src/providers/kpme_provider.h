#ifndef KPME_PROVIDER_H
#define KPME_PROVIDER_H

#include "../data_provider.h"
#include "../kp_loader.h"

class KpmeProvider : public IDataProvider {
public:
    std::string GetProviderName() const override { return "KPME"; }
    PlayerProofData LoadPlayerData(const std::string& account) override;
    bool SupportsLinkedAccounts() const override { return true; }
    std::vector<std::string> GetSupportedProofTypes() const override;

private:
    PlayerProofData ConvertKpmeResponse(const Kpme::KpmeResponse& response);
};

#endif
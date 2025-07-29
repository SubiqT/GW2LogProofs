#ifndef WINGMAN_PROVIDER_H
#define WINGMAN_PROVIDER_H

#include "../data_provider.h"
#include "../kp_loader.h"

class WingmanProvider : public IDataProvider {
public:
    std::string GetProviderName() const override { return "Wingman"; }
    PlayerProofData LoadPlayerData(const std::string& account) override;
    bool SupportsLinkedAccounts() const override { return false; }
    std::vector<std::string> GetSupportedProofTypes() const override;

private:
    PlayerProofData ConvertWingmanResponse(const Wingman::WingmanResponse& response);
};

#endif
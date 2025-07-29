#include "wingman_provider.h"

PlayerProofData WingmanProvider::LoadPlayerData(const std::string& account) {
    Wingman::WingmanResponse response = Wingman::GetKp(account);
    return ConvertWingmanResponse(response);
}

std::vector<std::string> WingmanProvider::GetSupportedProofTypes() const {
    return {"KILL_PROOF"};
}

PlayerProofData WingmanProvider::ConvertWingmanResponse(const Wingman::WingmanResponse& response) {
    PlayerProofData data;
    data.accountName = response.account;
    data.profileId = response.account;
    data.profileUrl = "https://gw2wingman.nevermindcreations.de/search/" + response.account;
    data.hasLinkedAccounts = false;
    
    for (const auto& kp : response.kp) {
        ProofData proof;
        proof.identifier = kp.first;
        proof.amount = kp.second;
        proof.type = ProofType::KILL_PROOF;
        proof.displayName = kp.first;
        proof.url = "";
        data.proofs[kp.first] = proof;
    }
    
    return data;
}
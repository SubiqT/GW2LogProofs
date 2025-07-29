#include "kpme_provider.h"
#include "../settings.h"

PlayerProofData KpmeProvider::LoadPlayerData(const std::string& account) {
    Kpme::KpmeResponse response = Kpme::GetKp(account);
    return ConvertKpmeResponse(response);
}

std::vector<std::string> KpmeProvider::GetSupportedProofTypes() const {
    return {"KILL_PROOF", "TOKEN", "COFFER", "TITLE"};
}

PlayerProofData KpmeProvider::ConvertKpmeResponse(const Kpme::KpmeResponse& response) {
    PlayerProofData data;
    data.accountName = response.id;
    data.profileId = response.id;
    data.profileUrl = "https://killproof.me/proof/" + response.id;
    data.hasLinkedAccounts = !response.shared.killproofs.empty() || !response.shared.tokens.empty() || 
                            !response.shared.coffers.empty() || !response.shared.titles.empty();
    
    // Add killproofs (self + shared if enabled)
    for (const auto& kp : response.self.killproofs) {
        ProofData proof;
        proof.identifier = kp.first;
        proof.amount = kp.second;
        if (Settings::IncludeLinkedAccounts && response.shared.killproofs.count(kp.first)) {
            proof.amount += response.shared.killproofs.at(kp.first);
        }
        proof.type = ProofType::KILL_PROOF;
        proof.displayName = kp.first;
        proof.url = "";
        data.proofs[kp.first] = proof;
    }
    
    // Add shared-only killproofs if linked accounts enabled
    if (Settings::IncludeLinkedAccounts) {
        for (const auto& kp : response.shared.killproofs) {
            if (!response.self.killproofs.count(kp.first)) {
                ProofData proof;
                proof.identifier = kp.first;
                proof.amount = kp.second;
                proof.type = ProofType::KILL_PROOF;
                proof.displayName = kp.first;
                proof.url = "";
                data.proofs[kp.first] = proof;
            }
        }
    }
    
    // Add tokens (self + shared if enabled)
    for (const auto& token : response.self.tokens) {
        ProofData proof;
        proof.identifier = token.first;
        proof.amount = token.second;
        if (Settings::IncludeLinkedAccounts && response.shared.tokens.count(token.first)) {
            proof.amount += response.shared.tokens.at(token.first);
        }
        proof.type = ProofType::TOKEN;
        proof.displayName = token.first;
        proof.url = "";
        data.proofs[token.first] = proof;
    }
    
    // Add coffers (self + shared if enabled)
    for (const auto& coffer : response.self.coffers) {
        ProofData proof;
        proof.identifier = coffer.first;
        proof.amount = coffer.second;
        if (Settings::IncludeLinkedAccounts && response.shared.coffers.count(coffer.first)) {
            proof.amount += response.shared.coffers.at(coffer.first);
        }
        proof.type = ProofType::COFFER;
        proof.displayName = coffer.first;
        proof.url = "";
        data.proofs[coffer.first] = proof;
    }
    
    return data;
}
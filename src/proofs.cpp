#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <format>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "shared.h"

bool shouldClearAllPlayers = false;
std::string shouldRemovePlayer;
std::string shouldAddPlayer;

void from_json(const json& j, Player& p) {
    j.at("account").get_to(p.account);
    j.at("chars").get_to<std::vector<std::string>>(p.characters);
    j.at("groups").get_to<std::vector<std::string>>(p.groups);
    j.at("kp").get_to<std::map<std::string, std::map<std::string, int>>>(p.kp);
}

std::string DownloadProof(const char* account) {
    std::string urlString = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
    const char* url = urlString.c_str();
    std::wstring wUrl(url, url + strlen(url));

    IStream* stream;
    HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
    if (result != 0)
    {
        return "An error occured.";
    }

    const unsigned long chunkSize = 128;
    char buffer[chunkSize];
    unsigned long bytesRead;
    std::stringstream strStream;

    stream->Read(buffer, chunkSize, &bytesRead);
    while (bytesRead > 0)
    {
        strStream.write(buffer, (long long)bytesRead);
        stream->Read(buffer, chunkSize, &bytesRead);
    }
    stream->Release();
    std::string proof = strStream.str();
    return proof.c_str();
}

Player GetProof(const char* account) {
    std::string proof = DownloadProof(account);
    json j = json::parse(proof);
    Player p = j.template get<Player>();
    return p;
}

std::string StripAccount(std::string account) {
    if (account.at(0) == ':') {
        account.erase(0, 1);
    }
    return account;
}

void UpdatePlayers() {
    if (shouldClearAllPlayers) {
        players.clear();
        shouldClearAllPlayers = false;
    }
    if (!shouldRemovePlayer.empty()) {
        for (Player& player : players) {
            if (player.account == shouldRemovePlayer) {
                long long index = std::addressof(player) - std::addressof(players[0]);
                players.erase(players.begin() + index);
            }
        }
        shouldRemovePlayer.clear();
    }
    if (!shouldAddPlayer.empty()) {
        try {
            Player newPlayer = GetProof(shouldAddPlayer.c_str());
            if (shouldAddPlayer == selfName) {
                self = newPlayer;
            }
            else {
                players.push_back(newPlayer);
            }
        } catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("An unexpected exception occurred when trying to update proofs for {}. Exception details: {}", shouldAddPlayer, e.what()).c_str());
        }
        shouldAddPlayer.clear();
    }
}

void SquadEventHandler(void* eventArgs) {
    SquadUpdate* squadUpdate = (SquadUpdate*)eventArgs;
    std::string account = StripAccount(squadUpdate->UserInfo->AccountName);
    int role = int(squadUpdate->UserInfo->Role);
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("updated  user: {} -  {}", account, role).c_str());
    if (selfName == account) {
        if (role == 5) {
            shouldClearAllPlayers = true;
        }
    }
    else {
        shouldRemovePlayer = account;
        if (role <= 2) {
            shouldAddPlayer = account;
        }
    }
    
}

void CombatEventHandler(void* eventArgs) {
    EvCombatData* cbtEvent = (EvCombatData*)eventArgs;
    if (!selfName.empty()) {
        return;
    }
    if (cbtEvent->ev) {
        return;
    }
    if (cbtEvent->src->elite) {
        return;
    }
    if (!cbtEvent->src->prof) {
        return;
    }
    if (cbtEvent->src->name == nullptr || cbtEvent->src->name[0] == '\0' || cbtEvent->dst->name == nullptr || cbtEvent->dst->name[0] == '\0') {
        return;
    }
    if (cbtEvent->dst->self) { // Should only occur on map change
        selfName = StripAccount(std::string(cbtEvent->dst->name));
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("self: {}", selfName).c_str());
        self = GetProof(selfName.c_str());
    }
}
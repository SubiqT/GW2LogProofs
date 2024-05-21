#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <format>
#include <thread>
#include <queue>
#include <nlohmann/json.hpp>
#include "unofficial_extras/Definitions.h"

using json = nlohmann::json;

#include "shared.h"

bool shouldClearAllPlayers = false;
std::queue<std::string> removePlayerQueue;
std::queue<Player> addPlayerQueue;
std::mutex mtx;

struct SquadUpdate
{
    UserInfo* UserInfo;
    uint64_t UsersCount;
};

void from_json(const json& j, Player& p) {
    j.at("account").get_to(p.account);
    j.at("chars").get_to<std::vector<std::string>>(p.characters);
    j.at("groups").get_to<std::vector<std::string>>(p.groups);
    j.at("kp").get_to<std::map<std::string, std::map<std::string, int>>>(p.kp);
}

std::string DownloadPlayerKP(const char* account) {
    std::string urlString = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
    const char* url = urlString.c_str();
    std::wstring wUrl(url, url + strlen(url));

    IStream* stream;
    HRESULT result = URLOpenBlockingStream(0, wUrl.c_str(), &stream, 0, 0);
    if (result != 0) {
        return "An error occured.";
    }

    const unsigned long chunkSize = 128;
    char buffer[chunkSize];
    unsigned long bytesRead;
    std::stringstream strStream;

    stream->Read(buffer, chunkSize, &bytesRead);
    while (bytesRead > 0) {
        strStream.write(buffer, (long long)bytesRead);
        stream->Read(buffer, chunkSize, &bytesRead);
    }
    stream->Release();
    std::string proof = strStream.str();
    return proof.c_str();
}

Player GetPlayer(const char* account) {
    std::string proof = DownloadPlayerKP(account);
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

void PushAccountToAddPlayerQueue(std::string account) {
    std::thread([&] {
        for (Player player : players) { // Already added
            if (player.account == account) {
                return;
            }
        }
        Player addPlayer;
        try {
            addPlayer = GetPlayer(account.c_str());
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("An unexpected exception occurred when trying to update proofs for {}. Exception details: {}", account, e.what()).c_str());
        }
        std::scoped_lock lck(mtx);
        addPlayerQueue.push(addPlayer);
    }).join();
}

void SquadEventHandler(void* eventArgs) {
    SquadUpdate* squadUpdate = (SquadUpdate*)eventArgs;
    std::string account = StripAccount(squadUpdate->UserInfo->AccountName);
    int role = int(squadUpdate->UserInfo->Role);
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("squad event: {} -  {}", account, role).c_str());
    if (role == 5) { // Left squad
        if (selfName == account) {
            shouldClearAllPlayers = true;
            return;
        }
        else {
            std::scoped_lock lck(mtx);
            removePlayerQueue.push(account);
        }
    }
    if (role <= 2) { // In squad
        PushAccountToAddPlayerQueue(account);
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
        PushAccountToAddPlayerQueue(selfName);
    }
}

void UpdatePlayers() {
    std::scoped_lock lck(mtx);
    if (shouldClearAllPlayers) {
        if (selfName.empty()) {
            players.clear();
        }
        else {
            Player selfPlayer;
            for (Player& player : players) {
                if (player.account == selfName) {
                    selfPlayer = player;
                    break;
                }
            }
            if (!selfPlayer.account.empty()) {
                players.clear();
                players.push_back(selfPlayer);
            }
            else {
                players.clear();
            }
        }
        shouldClearAllPlayers = false;
    }
    if (!removePlayerQueue.empty()) {
        std::string removePlayer = removePlayerQueue.front();
        for (Player& player : players) {
            if (player.account == removePlayer) {
                long long index = std::addressof(player) - std::addressof(players[0]);
                players.erase(players.begin() + index);
            }
        }
        removePlayerQueue.pop();
    }
    if (!addPlayerQueue.empty()) {
        Player addPlayer = addPlayerQueue.front();
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("added player: {}", addPlayer.account).c_str());
        players.push_back(addPlayer);
        addPlayerQueue.pop();
    }
}
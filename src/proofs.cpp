#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <format>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "unofficial_extras/Definitions.h"
#include "shared.h"

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

void RemovePlayer(const char* account) {
    for (Player& player : players) {
        if (player.account == account) {
            long long index = std::addressof(player) - std::addressof(players[0]);
            players.erase(players.begin() + index);
        }
    }
}

void AddPlayer(const char* account) {
    RemovePlayer(account);
    players.push_back(GetProof(account));
}

void SquadEventHandler(const UserInfo* updatedUsers, size_t updatedUsersCount) {
    APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", std::format("Updated Users: {} -  {}", updatedUsers->AccountName, int(updatedUsers->Role)).c_str());
    if (int(updatedUsers->Role) > 2) {
        return;
    }
    std::string account = updatedUsers->AccountName;
    if (account.at(0) == ':')
        AddPlayer(account.erase(0, 1).c_str());
}

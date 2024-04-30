#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <sstream>
#include <format>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
    APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", std::format("Response: {}", proof).c_str());
    return proof.c_str();
}

Player GetProof(const char* account) {
    std::string proof = DownloadProof(account);
    json j = json::parse(proof);
    APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", std::format("Dump: {}", j.dump(4)).c_str());
    Player p = j.template get<Player>();
    char* result = p.account.data();
    APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", result);
    //for (const auto& [key1, value1] : p.kp) {
    //    for (const auto& [key2, value2] : value1) {
    //        APIDefs->Log(ELogLevel_DEBUG, "Log Proofs", std::format("{}:{}:{}", key1, key2, value2).c_str());
    //    }
    //}
    return p;
}
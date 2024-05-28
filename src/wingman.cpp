#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <string>
#include <map>

#include "wingman.h"
#include "httpclient.hpp"
#include "shared.h"

namespace Wingman {

    void from_json(const json& j, KillProofsResponse& r) {
        j.at("account").get_to(r.account);
        j.at("kp").get_to<std::map<std::string, std::map<std::string, int>>>(r.kp);
    }

    KillProofsResponse GetKillProofs(std::string account) {
        std::string url = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
        const char* cUrl = url.c_str();
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, cUrl);
        std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
        std::string response = HTTPClient::GetRequest(wUrl.c_str());
        json j = json::parse(response);
        return j.template get<KillProofsResponse>();
    }
}

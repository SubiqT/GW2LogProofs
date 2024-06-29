#ifndef KP_LOADER_H
#define KP_LOADER_H

#include <string>
#include <map>
#include <vector>

namespace Wingman {
    struct WingmanResponse {
        std::string account;
        std::map<std::string, int> kp;
        // Not used fields:
        //std::vector<std::string> characters;
        //std::vector<std::string> groups;
    };

	WingmanResponse GetKp(std::string account);
}

namespace Kpme {
    struct KpSummary {
        std::map<std::string, int> killproofs;
        std::map<std::string, int> tokens;
        std::map<std::string, int> coffers;
        std::map<std::string, std::string> titles;
    };

    struct KpmeResponse {
        std::string id;
        KpSummary self;
        KpSummary shared;
    };

    KpmeResponse GetKp(std::string account);
}


#endif
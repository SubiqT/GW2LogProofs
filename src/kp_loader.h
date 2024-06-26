#ifndef KP_LOADER_H
#define KP_LOADER_H

#include <string>
#include <map>
#include <vector>

namespace Wingman {
    struct WingmanResponse {
        std::string account;
        std::map<std::string, std::map<std::string, int>> kp;
        // Not used fields:
        //std::vector<std::string> characters;
        //std::vector<std::string> groups;
    };

	WingmanResponse GetKillProofs(std::string account);
}

namespace Kpme {
    struct Kp {
        int id;
        std::string name;
        int amount;
    };

    struct Title {
        int id;
        std::string name;
        std::string mode;
    };

    struct AccountKp {
        std::string account;
        std::vector<Kp> killproofs;
        std::vector<Kp> tokens;
        std::vector<Kp> coffers;
        std::vector<Title> titles;
    };

    struct TotalKp {
        std::vector<Kp> killproofs;
        std::vector<Kp> tokens;
        std::vector<Kp> coffers;
        std::vector<Title> titles;
    };

    struct KpmeResponse {
        std::string kpId;
        std::vector<AccountKp> linked;
        TotalKp linkedTotals;
    };

    KpmeResponse GetKillProofs(std::string account);
}


#endif
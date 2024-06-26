#ifndef KPME_H
#define KPME_H

#include <string>
#include <vector>
#include <map>

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
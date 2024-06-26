#ifndef WINGMAN_H
#define WINGMAN_H

#include <string>

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

#endif
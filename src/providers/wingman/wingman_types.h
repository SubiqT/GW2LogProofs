#ifndef WINGMAN_TYPES_H
#define WINGMAN_TYPES_H

#include <map>
#include <string>

namespace Wingman {
    struct WingmanResponse {
        std::string account;
        std::map<std::string, int> kp;
    };
}

#endif
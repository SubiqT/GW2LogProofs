#ifndef JSON_FETCH_H
#define JSON_FETCH_H

#include "../../nlohmann/json.hpp"
#include "provider.h"
#include <functional>
#include <string>

using ProofsParser = std::function<std::shared_ptr<const PlayerProofs>(const nlohmann::json&)>;

// Fetches url and parses its JSON body, classifying transport, HTTP and parse errors as FetchStatus values.
void FetchJson(const std::string& url, ProofsParser parse, FetchCallback callback);

#endif

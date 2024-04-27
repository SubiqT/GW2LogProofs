#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib/httplib.h"
#include <format>

#include "shared.h"

const std::string wingmanUrl  = "gw2wingman.nevermindcreations.de";

int GetProof(const char* account) {
	std::string path = std::format("/api/kp?account={}", account);
	httplib::SSLClient cli(wingmanUrl);
	auto res = cli.Get(path);
	cli.stop();
	return res->status;
}

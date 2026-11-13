#include "json_fetch.h"
#include "../../core/shared.h"
#include "../../utils/httpclient.h"
#include <format>

namespace {
	constexpr unsigned long HTTP_REQUEST_TIMEOUT = 408;
	constexpr unsigned long HTTP_TOO_MANY_REQUESTS = 429;

	bool IsRetryable(unsigned long status) {
		return status == HTTP_REQUEST_TIMEOUT || status == HTTP_TOO_MANY_REQUESTS || status >= 500;
	}

	void LogFailure(const std::string& url, const std::string& reason) {
		APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Request to {} failed: {}", url, reason).c_str());
	}

	FetchResult Classify(const std::string& url, const HTTPClient::Response& response, const ProofsParser& parse) {
		if (!response.received) {
			LogFailure(url, "no response");
			return {FetchStatus::FAILED};
		}
		if (response.status < 200 || response.status >= 300) {
			if (IsRetryable(response.status)) {
				LogFailure(url, std::format("HTTP {}", response.status));
				return {FetchStatus::FAILED};
			}
			return {FetchStatus::NOT_FOUND};
		}

		auto json = nlohmann::json::parse(response.body, nullptr, false);
		if (json.is_discarded() || !json.is_object()) {
			LogFailure(url, "response is not a JSON object");
			return {FetchStatus::FAILED};
		}
		// Both services answer unknown accounts with an object holding only an error message.
		if (json.contains("error")) {
			return {FetchStatus::NOT_FOUND};
		}
		try {
			return {FetchStatus::FOUND, parse(json)};
		} catch (const nlohmann::json::exception& e) {
			LogFailure(url, e.what());
			return {FetchStatus::FAILED};
		}
	}
} // namespace

void FetchJson(const std::string& url, ProofsParser parse, FetchCallback callback) {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Requesting {}", url).c_str());
	HTTPClient::GetAsync(url, [url, parse = std::move(parse), callback = std::move(callback)](const HTTPClient::Response& response) {
		callback(Classify(url, response, parse));
	});
}

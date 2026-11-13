#include "httpclient.h"
#include "../core/shared.h"

#pragma comment(lib, "winhttp.lib")
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <format>
#include <mutex>
#include <unordered_set>
#include <vector>
#include <winhttp.h>

namespace {
	constexpr size_t MAX_BODY_BYTES = 4 * 1024 * 1024;
	constexpr auto SHUTDOWN_TIMEOUT = std::chrono::seconds(5);

	struct Request {
		std::function<void(const HTTPClient::Response&)> callback;
		HTTPClient::Response response;
		HINTERNET connect = nullptr;
		HINTERNET handle = nullptr;
		std::vector<char> buffer;
		std::atomic<bool> completed = false;
		bool handleClosed = false; // Guarded by mutex.
	};

	std::mutex mutex;
	std::condition_variable allReleased;
	HINTERNET session = nullptr;
	std::unordered_set<Request*> inFlight;
	bool shuttingDown = false;

	// WinHttpCloseHandle can deliver HANDLE_CLOSING synchronously, so it must never be called with mutex held.
	void CloseRequestHandle(Request* request) {
		{
			std::scoped_lock lock(mutex);
			if (request->handleClosed) return;
			request->handleClosed = true;
		}
		WinHttpCloseHandle(request->handle);
	}

	void Complete(Request* request, bool received) {
		if (request->completed.exchange(true)) return;
		request->response.received = received;

		bool deliver;
		{
			std::scoped_lock lock(mutex);
			deliver = !shuttingDown;
		}
		if (deliver) request->callback(request->response);
		CloseRequestHandle(request);
	}

	void Release(Request* request) {
		WinHttpCloseHandle(request->connect);
		{
			std::scoped_lock lock(mutex);
			inFlight.erase(request);
		}
		delete request;
		allReleased.notify_all();
	}

	void QueryNextChunk(Request* request) {
		if (!WinHttpQueryDataAvailable(request->handle, nullptr)) Complete(request, false);
	}

	void CALLBACK OnStatus(HINTERNET handle, DWORD_PTR context, DWORD status, LPVOID info, DWORD infoLength) {
		auto* request = reinterpret_cast<Request*>(context);
		if (!request || handle != request->handle) return;

		switch (status) {
			case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
				if (!WinHttpReceiveResponse(request->handle, nullptr)) Complete(request, false);
				break;
			case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE: {
				DWORD code = 0;
				DWORD size = sizeof(code);
				if (!WinHttpQueryHeaders(request->handle, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &code, &size, WINHTTP_NO_HEADER_INDEX)) {
					Complete(request, false);
					break;
				}
				request->response.status = code;
				QueryNextChunk(request);
				break;
			}
			case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE: {
				DWORD available = *static_cast<DWORD*>(info);
				if (available == 0) {
					Complete(request, true);
				} else if (request->response.body.size() + available > MAX_BODY_BYTES) {
					Complete(request, false);
				} else {
					request->buffer.resize(available);
					if (!WinHttpReadData(request->handle, request->buffer.data(), available, nullptr)) Complete(request, false);
				}
				break;
			}
			case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
				if (infoLength == 0) {
					Complete(request, true);
				} else {
					request->response.body.append(request->buffer.data(), infoLength);
					QueryNextChunk(request);
				}
				break;
			case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
				Complete(request, false);
				break;
			case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
				Release(request);
				break;
		}
	}

	std::wstring Widen(const std::string& value) {
		int length = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
		std::wstring wide(length, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), wide.data(), length);
		return wide;
	}
} // namespace

namespace HTTPClient {
	void Initialize() {
		std::scoped_lock lock(mutex);
		shuttingDown = false;
		if (session) return;
		session = WinHttpOpen(L"GW2LogProofs/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC);
		if (session && WinHttpSetStatusCallback(session, OnStatus, WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS | WINHTTP_CALLBACK_FLAG_HANDLES, 0) == WINHTTP_INVALID_STATUS_CALLBACK) {
			WinHttpCloseHandle(session);
			session = nullptr;
		}
		if (!session) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("Failed to open WinHTTP session (error {})", GetLastError()).c_str());
		}
	}

	void GetAsync(const std::string& url, std::function<void(const Response&)> callback) {
		HINTERNET currentSession;
		{
			std::scoped_lock lock(mutex);
			currentSession = shuttingDown ? nullptr : session;
		}
		if (!currentSession) {
			callback({});
			return;
		}

		std::wstring wideUrl = Widen(url);
		URL_COMPONENTS parts = {};
		parts.dwStructSize = sizeof(parts);
		parts.dwSchemeLength = static_cast<DWORD>(-1);
		parts.dwHostNameLength = static_cast<DWORD>(-1);
		parts.dwUrlPathLength = static_cast<DWORD>(-1);
		if (!WinHttpCrackUrl(wideUrl.c_str(), 0, 0, &parts)) {
			callback({});
			return;
		}
		std::wstring host(parts.lpszHostName, parts.dwHostNameLength);
		std::wstring path(parts.lpszUrlPath, parts.dwUrlPathLength);

		auto* request = new Request {std::move(callback)};
		request->connect = WinHttpConnect(currentSession, host.c_str(), parts.nPort, 0);
		if (request->connect) {
			DWORD flags = parts.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0;
			request->handle = WinHttpOpenRequest(request->connect, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
		}
		DWORD_PTR context = reinterpret_cast<DWORD_PTR>(request);
		if (!request->handle || !WinHttpSetOption(request->handle, WINHTTP_OPTION_CONTEXT_VALUE, &context, sizeof(context))) {
			if (request->handle) WinHttpCloseHandle(request->handle);
			if (request->connect) WinHttpCloseHandle(request->connect);
			auto failed = std::move(request->callback);
			delete request;
			failed({});
			return;
		}

		{
			std::scoped_lock lock(mutex);
			inFlight.insert(request);
		}
		if (!WinHttpSendRequest(request->handle, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, context)) {
			Complete(request, false);
		}
	}

	void Shutdown() {
		std::vector<Request*> pending;
		{
			std::scoped_lock lock(mutex);
			if (!session) return;
			shuttingDown = true;
			pending.assign(inFlight.begin(), inFlight.end());
		}
		for (auto* request : pending) {
			CloseRequestHandle(request);
		}

		std::unique_lock lock(mutex);
		if (!allReleased.wait_for(lock, SHUTDOWN_TIMEOUT, [] { return inFlight.empty(); })) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("{} HTTP requests still pending at shutdown", inFlight.size()).c_str());
		}
		WinHttpSetStatusCallback(session, nullptr, 0, 0);
		WinHttpCloseHandle(session);
		session = nullptr;
	}

	std::string EncodeUrlComponent(std::string_view value) {
		static constexpr char hex[] = "0123456789ABCDEF";
		std::string encoded;
		for (unsigned char c : value) {
			bool unreserved = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '.' || c == '_' || c == '~';
			if (unreserved) {
				encoded += static_cast<char>(c);
			} else {
				encoded += '%';
				encoded += hex[c >> 4];
				encoded += hex[c & 0xF];
			}
		}
		return encoded;
	}
} // namespace HTTPClient

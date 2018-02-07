#pragma once

#include "network_error.h"

namespace wyre {

class WSASession {
private:
	WSADATA _wsaData;

public:
	WSASession() {
		int err = WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0;
		if (err != ERROR_SUCCESS) {
			throw network_error("WSAStartup failed", err);
		}
	}
	WSASession(WSASession &other) = delete;
	~WSASession() {
		WSACleanup();
	}
	const WSADATA & wsaData() const { return _wsaData; }
};

} // namespace wyre

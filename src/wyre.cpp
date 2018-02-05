#include "wyre.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include "SafeResource.h"


namespace wyre {

namespace {
void _closesocket(SOCKET s) {
	closesocket(s);
}
}

constexpr char * DEFAULT_PORT = "34215";

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

using SafeSocket = SafeResource<SOCKET, INVALID_SOCKET, _closesocket>;


std::pair<std::string, std::string> parseServerSpec(const std::string & serverSpec) {
	size_t colon = serverSpec.find(':');
	if (colon == serverSpec.npos) {
		return std::make_pair(serverSpec, DEFAULT_PORT);
	}
	const char *data = serverSpec.data();
	char *end;
	int port = strtol(data + colon + 1, &end, 10);
	if (port == 0 || *end != '\0') {
		throw network_error("Invalid port specification", ERROR_INVALID_DATA);
	}
	return std::make_pair(serverSpec.substr(0, colon), serverSpec.substr(colon + 1));
}

SafeSocket connectSocket(const std::string & address, const std::string & port) {
	printf("Connecting to %s:%s\n", address.c_str(), port.c_str());
	addrinfo *result = nullptr;
	addrinfo hints;
	int err;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	err = getaddrinfo(address.c_str(), port.c_str(), &hints, &result);
	if (err) { throw network_error("getaddrinfo", err); }

	SafeSocket sock;
	for (addrinfo *ptr = result; ptr; ptr = ptr->ai_next) {
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (!sock) {
			throw network_error("socket", WSAGetLastError());
		}
		err = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (err == SOCKET_ERROR) {
			sock.close();
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (!sock) {
		throw network_error("Unable to connect to server", WSAGetLastError());
	}
	return sock;
}

void run(std::vector<std::string>& args) {
	auto server = parseServerSpec(args[0]);

	WSASession w;
	auto sock = connectSocket(server.first, server.second);

	int err, sent;
	char buf[] = "Hello, world!";

	sent = send(sock, buf, (int)(sizeof(buf) - 1), 0);
	if (sent == SOCKET_ERROR) {
		throw network_error("send", WSAGetLastError());
	}
	printf("Sent %d bytes\n", sent);

	err = shutdown(sock, SD_SEND);
	if (err == SOCKET_ERROR) {
		throw network_error("shutdown", WSAGetLastError());
	}
}

void push(std::vector<std::string>& args) {

}


} // namespace wyre

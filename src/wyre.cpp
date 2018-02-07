#include "wyre.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "SafeResource.h"
#include "wsasession.h"
#include "socket.h"
#include "network_error.h"
#include "ChildProcess.h"

namespace wyre {

constexpr uint16_t DEFAULT_PORT = 34215;

using SafeSocket = SafeResource<SOCKET, INVALID_SOCKET, _closesocket>;

std::pair<std::string, uint16_t> parseServerSpec(const std::string & serverSpec) {
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
	return std::make_pair(serverSpec.substr(0, colon), port);
}

void run(std::vector<std::string>& args) {
	auto server = parseServerSpec(args[0]);
	args.erase(args.begin());

	WSASession w;
	wyre::socket sock;
	sock.connect(server.first, server.second);

	ChildProcess p;
	p.withArgv(args).useStdout().run();
	auto f = p.stdout_();
	if (!f) { throw std::runtime_error("stdout not defined"); }
	constexpr size_t BUF_SIZE = 16384;
	auto buf = std::make_unique<char[]>(BUF_SIZE);
	while (!std::feof(f)) {
		auto written = std::fread(buf.get(), 1, BUF_SIZE, f);
		if (written >= 0) {
			sock.write(buf.get(), written);
		}
	}
	sock.shutdown(SD_SEND);
	sock.close();
}

void push(std::vector<std::string>& args) {
	auto server = parseServerSpec(args[0]);

	WSASession w;
	wyre::socket sock;
	sock.connect(server.first, server.second);

	{
		std::ifstream file(args[1]);
		if (!file.is_open()) {
			throw std::runtime_error("Could not open file " + args[1]);
		}
		sock << file.rdbuf();
	}

	sock.shutdown(SD_SEND);
	sock.close();
}


} // namespace wyre

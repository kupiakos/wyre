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
#include "proto_util.h"

#include "wyre.pb.h"
#include "SHA1.h"

namespace wyre {

constexpr size_t BUF_SIZE = 16384;
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
	using wyre::proto::DataChunk;
	auto server = parseServerSpec(args[0]);
	args.erase(args.begin());

	WSASession w;
	wyre::socket sock;
	std::cout << "Connecting to " << server.first << ", port "
		<< server.second << std::endl;
	sock.connect(server.first, server.second);

	ChildProcess p;
	p.withArgv(args).useStdout().run();
	auto processOut = p.stdout_();
	if (!processOut) { throw std::runtime_error("stdout not defined"); }
	
	DataChunk d;
	d.set_description(p.cmdLine());
	d.set_source(DataChunk::COMMAND);
	d.set_finished(false);
	d.set_data("", 0);

	SHA1 sha;
	auto data = std::make_unique<char[]>(BUF_SIZE);
	std::cout << "Running " << p.cmdLine() << std::endl;

	do {
		size_t nRead = 0;
		if (!std::feof(processOut)) {
			nRead = std::fread(data.get(), 1, BUF_SIZE, processOut);
		}
		if (nRead > 0) {
			sha.update(data.get(), nRead);
			d.set_data(data.get(), nRead);
		} else if (nRead < 0) {
			throw std::runtime_error("Error reading processOut");
		} else {
			continue;
		}
		if (std::feof(processOut)) {
			d.set_finished(true);
			d.set_finalhash(sha.hexdigest());
		}
		if (!wyre::proto::writeLengthDelimited(sock, d)) {
			throw std::runtime_error("Could not serialize");
		}
	} while (!std::feof(processOut));

	sock.shutdown(SD_SEND);
	sock.close();
}

void push(std::vector<std::string>& args) {
	auto server = parseServerSpec(args[0]);

	WSASession w;
	wyre::socket sock;
	sock.connect(server.first, server.second);

	{
		std::ifstream file(args[1], std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("Could not open file " + args[1]);
		}
		sock << file.rdbuf();
	}

	sock.shutdown(SD_SEND);
	sock.close();
}


} // namespace wyre

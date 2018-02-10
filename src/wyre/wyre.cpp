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
#include <filesystem>

#include "wyre.h"
#include "SafeResource.h"
#include "wsasession.h"
#include "socket.h"
#include "network_error.h"
#include "ChildProcess.h"
#include "proto_util.h"
#include "network_util.h"
#include "winunicode.h"

#include "SHA1.h"

namespace wyre {

using wyre::proto::DataChunk;
namespace fs = std::experimental::filesystem;
constexpr size_t BUF_SIZE = 16384;

inline FILE * utf8Open(const char * filename, const char * mode) {
#ifdef _WIN32
	FILE *f;
	auto err =_wfopen_s(
		&f,
		unicode::utf8ToUtf16(filename).c_str(),
		unicode::utf8ToUtf16(mode).c_str());
	if (err) {
		throw std::system_error(
			std::make_error_code(static_cast<std::errc>(err)), filename);
	}
	return f;
#else
	return std::open(filename, mode);
#endif
}

void WyreClient::sendFile(DataChunk & d, FILE *f) {
	if (!f) { return; }

	SHA1 sha;
	auto data = std::make_unique<char[]>(BUF_SIZE);

	d.set_finished(false);

	do {
		size_t nRead = 0;
		if (!std::feof(f)) {
			nRead = std::fread(data.get(), 1, BUF_SIZE, f);
		}
		if (nRead > 0) {
			sha.update(data.get(), nRead);
			d.set_data(data.get(), nRead);
		} else if (nRead < 0) {
			throw std::runtime_error("Error reading");
		} else {
			continue;
		}
		if (std::feof(f)) {
			d.set_finished(true);
			d.set_finalhash(sha.hexdigest());
		}
		if (!wyre::proto::writeLengthDelimited(_sock, d)) {
			throw std::runtime_error("Could not serialize");
		}
	} while (!std::feof(f));
}

void WyreClient::connect(const std::string & hostname, uint16_t port) {
	std::cout << "Connecting to " << hostname << ", port "
		<< port << std::endl;
	_sock.connect(hostname, port);
}

void WyreClient::run(std::vector<std::string>& args) {
	ChildProcess p;
	p.withArgv(args).useStdout().run();
	auto processOut = p.stdout_();
	if (!processOut) { throw std::runtime_error("stdout not defined"); }
	
	DataChunk d;
	d.set_description(p.cmdLine());
	d.set_source(DataChunk::COMMAND);

	std::cout << "Running " << p.cmdLine() << std::endl;
	sendFile(d, processOut);

	_sock.shutdown(SD_SEND);
	_sock.close();
}

void WyreClient::push(std::vector<std::string>& args) {
	DataChunk d;
	d.set_source(DataChunk::FILE);

	for (const auto & fname : args) {
		SafeFile f = utf8Open(fname.c_str(), "rb");
		if (!f) {
			throw std::runtime_error("Could not open file " + fname);
		}
		d.set_description(fs::canonical(fname).u8string());
		sendFile(d, f);
	}

	_sock.shutdown(SD_SEND);
	_sock.close();
}


} // namespace wyre

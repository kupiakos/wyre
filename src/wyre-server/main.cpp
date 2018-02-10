
#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <filesystem>

#include "socket.h"
#include "SHA1.h"
#include "ChildProcess.h"
#include "SafeResource.h"
#include "wyre.h"
#include "ConsumerThreadPool.h"
#include "network_error.h"
#include "wsasession.h"
#include "time_util.h"
#include "wincmdline.h"
#include "proto_util.h"

#pragma warning (disable : 4146)

#include "wyre.pb.h"

int wyre_main(std::vector<std::string> argv);

void printUsage() {
	fprintf(stderr, "%s",
		"wyre-server <interface:port>\n\n"
	);
}

#if _WIN32
#ifndef UNICODE
#error "Unicode support must be enabled"
#endif
#include "winunicode.h"
#include <io.h>

int wmain(int argc, wchar_t * wargv[]) {
	std::vector<std::string> argv;
	try {
		// Deal with Windows UTF-16 stupidness
		for (int i = 0; i < argc; ++i) {
			argv.push_back(wyre::unicode::utf16ToUtf8(wargv[i]));
		}
	}
	catch (const wyre::unicode::unicode_error &e) {
		fprintf(stderr, "Unicode Error occurred with error code 0x%x: %s\n",
			e.errorCode(), e.what());
		return e.errorCode();
	}
	catch (const std::exception &e) {
		fprintf(stderr, "Unhandled exception: %s\n", e.what());
		return ERROR_UNHANDLED_EXCEPTION;
	}
	return wyre_main(std::move(argv));
}
#else
int main(int argc, char ** argv) {
	return wyre_main(std::vector<std::string>(argv, argv + argc));
}
#endif

constexpr uint16_t DEFAULT_PORT = 34215;
std::pair<std::string, uint16_t> parseServerSpec(const std::string & serverSpec) {
	size_t colon = serverSpec.find(':');
	if (colon == serverSpec.npos) {
		return std::make_pair(serverSpec, DEFAULT_PORT);
	}
	const char *data = serverSpec.data();
	char *end;
	int port = strtol(data + colon + 1, &end, 10);
	if (port == 0 || *end != '\0') {
		throw std::runtime_error("Invalid port specification");
	}
	return std::make_pair(serverSpec.substr(0, colon), port);
}

namespace fs = std::experimental::filesystem::v1;

int wyre_main(std::vector<std::string> argv) {
	// TODO: Refactor heavily
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argv.size() < 2 ||
		argv[1] == "-h" ||
		argv[1] == "--help") {
		printUsage();
		return 0;
	}
	argv.erase(argv.begin());

	int err = 0;

	try {
		auto spec = parseServerSpec(argv[0]);
		wyre::WSASession w;
		wyre::socket sock;
		
		sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
		sock.bind(spec.first, spec.second);
		sock.listen();

		std::mutex reportMut;
		std::ofstream reportFile("report.md", std::ios::app);
		std::ofstream shaFile("hashes.sha1.txt", std::ios::app);
		const fs::path destDir = "out";
		if (!fs::is_directory(destDir)) {
			fs::create_directory(destDir);
		}

		ConsumerThreadPool<wyre::client_response> threadPool(2, [&](wyre::client_response && r) {
			wyre::socket sock(std::move(r.socket()));
			using wyre::proto::DataChunk;
			DataChunk chunk;
			if (!wyre::proto::readLengthDelimited(sock, chunk)) {
				throw std::runtime_error("Could not parse data chunk");
			}
			// Verify fields
			const auto & description = chunk.description();
			if (description.empty()) {
				throw std::runtime_error("Chunk description empty");
			}

			// Figure out where to save it
			fs::path dest;
			switch (chunk.source()) {
			case DataChunk::FILE: {
				auto slash = description.find_last_of("/\\");
				if (slash == std::string::npos) {
					slash = 0;
				}
				dest = destDir / (description.substr(slash));
				break;
			}
			case DataChunk::COMMAND:
				dest = destDir / wyre::win32::escapeFile(
					wyre::unicode::utf8ToUtf16(description));
				break;
			default:
				throw std::runtime_error("Unrecognized chunk source");
			}

			for (int i = 1; fs::exists(dest); ++i) {
				// technically a race condition
				dest.replace_extension(std::to_string(i));
			}
			std::cout << "[" << r.address() << "] " << description
				<< " -> " << dest << std::endl;
			{
				std::unique_lock<std::mutex> lock(reportMut);
				reportFile << wyre::dateTimeToString(
					wyre::now(), "###### %F %T %z%n");
				if (chunk.source() == DataChunk::FILE) {
					reportFile
						<< "Saved file '" << description << "'\n\n"
						<< "Impact: filesystem, cache\n\n";
				} else {
					reportFile
						<< "Ran command '" << description << "'\n\n"
						<< "Impact: new process, TODO\n\n";
				}
				reportFile
					<< "Saved at " << dest << ".\n"
					<< std::endl;
			}
			SHA1 sha;
			{
				std::ofstream destFile(dest, std::ios::out | std::ios::binary);
				do {
					destFile << chunk.data();
					sha.update(chunk.data());
					if (!sock.eof() && !wyre::proto::readLengthDelimited(sock, chunk)) {
						throw std::runtime_error("Could not parse data chunk");
					}
				} while (!chunk.finished() && !sock.eof());
			}
			auto digest = sha.hexdigest();
			if (!chunk.finalhash().empty()) {
				if (chunk.finalhash() != digest) {
					throw std::runtime_error("Checksum failure!");
				}
			}
			{
				std::unique_lock<std::mutex> lock(reportMut);
				shaFile << digest << "  " << dest.generic_u8string() << std::endl;
			}
		});

		std::cout << "Listening on " << spec.first << ":" <<
			spec.second << std::endl;
		while (true) {
			auto response = sock.accept();
			threadPool.push(std::move(response));
		}
	}
	catch (const std::system_error &e) {
		fprintf(stderr, "%s", e.what());
		return e.code().value();
	}
	catch (const std::exception &e) {
		fprintf(stderr, "%s", e.what());
	}
	google::protobuf::ShutdownProtobufLibrary();
	return err;
}


#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <filesystem>
#include <ctime>
#include <sstream>
#include <locale>

#include "socket.h"
#include "SHA1.h"
#include "ChildProcess.h"
#include "SafeResource.h"
#include "wyre.h"
#include "ConsumerThreadPool.h"
#include "network_error.h"
#include "wsasession.h"

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


std::ostream& formatDateTime(std::ostream& out, const tm& t, const char* fmt) {
	const auto& dateWriter = std::use_facet<std::time_put<char>>(out.getloc());
	int n = std::strlen(fmt);
	if (dateWriter.put(out, out, ' ', &t, fmt, fmt + n).failed()) {
		throw std::runtime_error("failure to format date time");
	}
	return out;
}

std::string dateTimeToString(const tm& t, const char* format) {
	std::ostringstream s;
	formatDateTime(s, t, format);
	return s.str();
}

tm now() {
	time_t now = time(0);
	tm result;
	localtime_s(&result, &now);
	return result;
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

		std::mutex mut;
		std::ofstream reportFile("report.md", std::ios::app);
		const fs::path destDir = fs::current_path() / "out";
		if (!fs::is_directory(destDir)) {
			fs::create_directory(destDir);
		}

		ConsumerThreadPool<wyre::client_response> threadPool(2, [&](wyre::client_response && r) {
			wyre::socket sock(std::move(r.socket()));
			using wyre::proto::DataChunk;
			DataChunk chunk;
			if (!chunk.ParseFromIstream(&sock)) {
				std::cerr << "Could not parse data chunk" << std::endl;
				return;
			}
			fs::path dest;
			if (chunk.source() == DataChunk::FILE) {
				dest = destDir /
					(chunk.description().substr(0, chunk.description().find(' ')) + ".0");
			} else {
				dest = destDir /
					(chunk.description().substr(chunk.description().find_last_of("/\\", 0)) + ".0");
			}
			int i = 1;
			while (fs::exists(dest)) {
				dest.replace_extension(std::to_string(i++));
			}
			{
				std::unique_lock<std::mutex> lock(mut);
				std::cout << chunk.description() << std::endl;
				reportFile << dateTimeToString(now(), "##### %Y-%m-%d %H:%M:%S") << '\n';
				if (chunk.source() == DataChunk::FILE) {
					reportFile
						<< "Saved file '" << chunk.description() << "'\n\n"
						<< "Impact: filesystem, cache\n\n";
				} else {
					reportFile
						<< "Ran command '" << chunk.description() << "'\n\n"
						<< "Impact: new process, TODO\n\n";
				}
				reportFile
					<< "Saved at " << dest << '\n'
					<< std::endl;
			}
			SHA1 sha;
			{
				std::ofstream destFile(dest, std::ios::out | std::ios::binary);
				do {
					destFile << chunk.data();
					sha.update(chunk.data());
					if (!chunk.ParseFromIstream(&sock)) {
						std::cerr << "Could not parse data chunk" << std::endl;
					}
				} while (!chunk.finished());
			}
			std::cout << dest << ": " << sha.hexdigest() << std::endl;
		});

		while (true) {
			auto response = sock.accept();
			std::cout << "Connection from " << response.address()
				<< ":" << response.port() << std::endl;
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

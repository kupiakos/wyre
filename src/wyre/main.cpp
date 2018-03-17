
#include <vector>
#include <string>
#include <fstream>

#include "SHA1.h"
#include "ChildProcess.h"
#include "SafeResource.h"
#include "wyre.h"
#include "network_util.h"

#pragma warning (disable : 4146)

#include "wyre.pb.h"
#include <grpc++/grpc++.h>
#include "wyre.grpc.pb.h"

constexpr const char * DEFAULT_PORT = "34215";

int wyre_main(std::vector<std::string> argv);

void printUsage() {
	fprintf(stderr, "%s",
		"wyre server[:port] <command>\n\n"
		"Common commands:\n\n"
		"  run args...  - run a command and upload\n"
		"  push file    - upload a file to the server\n"
	);
}

#if _WIN32
#ifndef UNICODE
#error "Unicode support must be enabled"
#endif
#include "win32/winunicode.h"
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

int wyre_main(std::vector<std::string> argv) {
	// TODO: Refactor heavily

	if (argv.size() <= 3 ||
		argv[1] == "-h" ||
		argv[1] == "--help") {
		printUsage();
		return 0;
	}
	auto serverAddr = std::move(argv[1]);
	auto cmd = std::move(argv[2]);
	argv.erase(argv.begin(), argv.begin() + 3);

	int err = 0;

	try {
		if (serverAddr.find_first_of(':') == serverAddr.npos) {
			serverAddr.push_back(':');
			serverAddr.append(DEFAULT_PORT);
		}
		wyre::WyreClient client(
			grpc::CreateChannel(serverAddr, grpc::InsecureChannelCredentials()));
		if (cmd == "run") {
			// TODO: move to parent
			if (argv.size() < 1) {
				printUsage();
			} else {
				client.run(argv);
			}
		} else if (cmd == "push") {
			if (argv.size() != 1) {
				printUsage();
			} else {
				client.push(argv);
			}
		} else {
			printUsage();
		}
	} catch (const std::system_error &e) {
		fprintf(stderr, "%s", e.what());
		return e.code().value();
	} catch (const std::exception &e) {
		fprintf(stderr, "%s", e.what());
	}
	return err;
}



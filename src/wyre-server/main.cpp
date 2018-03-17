
#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <filesystem>

#include "grpc++/grpc++.h"

#include "SHA1.h"
#include "ChildProcess.h"
#include "SafeResource.h"
#include "time_util.h"
#include "proto_util.h"
#include "network_util.h"
#include "WyreServer.h"


#pragma warning (disable : 4146)

#include "wyre.pb.h"

int wyre_main(std::vector<std::string> argv);

constexpr char const * DEFAULT_PORT = "34215";

void printUsage() {
	fprintf(stderr, "%s",
		"wyre-server <interface:port>\n\n"
	);
}

#if _WIN32
#ifndef UNICODE
#error "Unicode support must be enabled"
#endif

#include "win32/wincmdline.h"
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

	if (argv.size() < 2 ||
		argv[1] == "-h" ||
		argv[1] == "--help") {
		printUsage();
		return 0;
	}
	argv.erase(argv.begin());

	int err = 0;

	try {
		std::ofstream reportFile("report.md", std::ios::app);
		std::ofstream shaFile("hashes.sha1.txt", std::ios::app | std::ios::binary);
		wyre::WyreServer service(&reportFile, &shaFile, "out");
		grpc::ServerBuilder builder;
		auto serverAddr = argv[0];
		if (serverAddr.find_first_of(':') == serverAddr.npos) {
			serverAddr.push_back(':');
			serverAddr.append(DEFAULT_PORT);
		}
		builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
		builder.RegisterService(&service);
		std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
		std::cout << "Server listening on " << serverAddr << std::endl;
		server->Wait();
	}
	catch (const std::system_error &e) {
		fprintf(stderr, "%s", e.what());
		return e.code().value();
	}
	catch (const std::exception &e) {
		fprintf(stderr, "%s", e.what());
	}
	return err;
}

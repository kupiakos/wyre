
#include <vector>
#include <string>
#include <fstream>

#include "SHA1.h"
#include "ChildProcess.h"
#include "SafeResource.h"
#include "wyre.h"

#pragma warning (disable : 4146)

//#include "wyre.pb.h"

int wyre_main(std::vector<std::string> argv);

void printUsage() {
	fprintf(stderr, "%s",
		"wyre <command>\n\n"
		"Common commands:\n\n"
		"  run server args [args]* - run a command and upload\n"
		"  push server file        - upload a file to the server\n"
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

int wyre_main(std::vector<std::string> argv) {
	// TODO: Refactor heavily
	//GOOGLE_PROTOBUF_VERIFY_VERSION;
	wprintf(L"Hello, world!");

	if (argv.size() <= 2 ||
			argv[1] == "-h" ||
			argv[1] == "--help") {
		printUsage();
		return 0;
	}
	std::string cmd = std::move(argv[1]);
	argv.erase(argv.begin(), argv.begin() + 2);

	int err = 0;

	try {
		if (cmd == "run") {
			// TODO: move to parent
			if (argv.size() < 2) {
				printUsage();
			} else {
				wyre::run(argv);
			}
		} else if (cmd == "push") {
			if (argv.size() != 2) {
				printUsage();
			} else {
				wyre::push(argv);
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
	//google::protobuf::ShutdownProtobufLibrary();
	return err;
}



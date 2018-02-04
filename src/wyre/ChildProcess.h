#ifndef _CHILD_PROCESS_H_
#define _CHILD_PROCESS_H_

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cstdio>

#include <SafeResource.h>

namespace wyre {

#ifdef _WIN32
typedef SafeHandle ProcessHandle;
#else
typedef int ProcessHandle;
#endif

class process_error : public std::runtime_error {
public:
	process_error(const char *message) :
		std::runtime_error(message) {}
};

class ChildProcess {
	SafeFile _stdout;
	ProcessHandle _handle;
	std::string _cmdLine;
	bool _redirectStderr = false;

#ifdef _WIN32
	SafeHandle _stdoutWrite, _hProcess;
	PROCESS_INFORMATION _procInfo;
#endif

public:
	ChildProcess();
	ChildProcess & useStdout();
	ChildProcess & stdErrToStdOut();
	ChildProcess & withCommand(const std::string & cmdLine);
	ChildProcess & withArgv(const std::vector<std::string> & argv);
	ChildProcess & run();

	FILE * stdout_() { return _stdout; }
	//FILE * stdout_() { return _stdout; }
};

}

#endif
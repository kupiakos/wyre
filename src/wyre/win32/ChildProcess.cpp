
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <sstream>

#include <ChildProcess.h>
#include "winunicode.h"
#include "wincmdline.h"

#define ZeroObject(x) do { ZeroMemory(&x, sizeof(x)); } while (0)

namespace wyre {

namespace {
std::string formatError(DWORD err) {
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0, nullptr
	);

	std::string msg((LPSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return msg;
}

void throwSystemError(const char * component) {
	DWORD err = ::GetLastError();
	std::ostringstream ss;
	ss << component << " failed with error " << err << ": " << formatError(err);
	throw process_error(ss.str().c_str());
}

}


static SafeFile handleToSafeFile(HANDLE h, const char * mode, DWORD flags = 0) {
	if (h == INVALID_HANDLE_VALUE) {
		throw process_error("Invalid handle value");
	}
	int fd = _open_osfhandle((intptr_t)h, flags);
	if (fd == -1) {
		throw process_error("Could not convert OS file handle");
	}
	FILE *f = _fdopen(fd, mode);
	if (!f) {
		throw process_error("Could not open file");
	}
	return SafeFile(f);
}

ChildProcess::ChildProcess() { }

ChildProcess & ChildProcess::useStdout() {
	if (_stdout) {
		throw process_error("Stdout already defined!");
	}
	SafeHandle hWrite, hRead;
	SECURITY_ATTRIBUTES sAttr;
	sAttr.nLength = sizeof(sAttr);
	sAttr.bInheritHandle = TRUE;
	sAttr.lpSecurityDescriptor = nullptr;

	if (!CreatePipe(&hRead, &hWrite, &sAttr, 0)) {
		throwSystemError("Creating stdout pipe");
	}

	// Make sure the read handle cannot be inherited
	if (!SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, FALSE)) {
		throw process_error("Could not limit inheritance of stdout read pipe");
	}

	_stdoutWrite = std::move(hWrite);
	_stdout = handleToSafeFile(hRead.get(), "rb", 0);
	hRead.release();

	return *this;
}

ChildProcess & ChildProcess::stdErrToStdOut() {
	_redirectStderr = true;
	return *this;
}

ChildProcess & ChildProcess::withCommand(const std::string & cmdLine) {
	_cmdLine = cmdLine;

	return *this;
}

ChildProcess & ChildProcess::withArgv(const std::vector<std::string> & argv) {
	std::wstring wCmdLine;
	auto it = argv.cbegin();
	for (;;) {
		wCmdLine.append(win32::escapeArg(unicode::utf8ToUtf16(*it), false));
		++it;
		if (it != argv.cend()) {
			wCmdLine.push_back(L' ');
		} else {
			break;
		}
	}
	_cmdLine = unicode::utf16ToUtf8(wCmdLine);
	return *this;
}

ChildProcess & ChildProcess::run() {
	if (_cmdLine.empty()) { return *this; }

	std::wstring wCmdLine = unicode::utf8ToUtf16(_cmdLine);
	STARTUPINFO startInfo;
	BOOL success = FALSE;

	ZeroObject(_procInfo);
	ZeroObject(startInfo);
	startInfo.cb = sizeof(startInfo);

	bool redirect = _stdout;
	if (redirect) {
		startInfo.dwFlags |= STARTF_USESTDHANDLES;

		HANDLE hStdOut = _stdoutWrite ? _stdoutWrite.get() : GetStdHandle(STD_OUTPUT_HANDLE);
		startInfo.hStdOutput = hStdOut;
		startInfo.hStdInput = INVALID_HANDLE_VALUE;

		if (_redirectStderr) {
			startInfo.hStdError = hStdOut;
		} else {
			startInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		}
	}

	success = CreateProcessW(nullptr,
		&wCmdLine[0], // command line
		nullptr,      // process security attributes
		nullptr,      // primary thread security attributes
		TRUE,         // handles are inherited
		0,            // creation flags
		nullptr,      // use parent's environment
		nullptr,      // use parent's current directory
		&startInfo,   // STARTUPINFO pointer
		&_procInfo     // receives PROCESS_INFORMATION
	);
	if (!success) {
		throwSystemError("CreateProcess");
	}

	if (!CloseHandle(_procInfo.hThread)) {
		throwSystemError("Closing hThread");
	}
	_stdoutWrite.close();
	_hProcess = _procInfo.hProcess;

	return *this;
}


} // namespace wyre

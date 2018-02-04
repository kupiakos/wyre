#ifndef _WIN_UNICODE_H_
#define _WIN_UNICODE_H_

#include <Windows.h>
#include <string>

#include <stdexcept>

namespace wyre {
namespace unicode {

class unicode_error : public std::runtime_error {
	DWORD _errorCode;
public:
	unicode_error(const char *message, DWORD errorCode) :
		std::runtime_error(message),
		_errorCode(errorCode) {}
	DWORD errorCode() const { return _errorCode; }
};

std::wstring utf8ToUtf16(const std::string & utf8);
std::string utf16ToUtf8(const std::wstring & utf16);

} // namespace unicode
} // namespace wyre

#endif
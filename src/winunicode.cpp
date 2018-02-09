#include "winunicode.h"

namespace wyre {
namespace unicode {

#pragma warning (disable : 4996)
inline bool onXP() {
	return (DWORD)(LOBYTE(LOWORD(GetVersion()))) < 6;
}

std::wstring utf8ToUtf16(const std::string & utf8) {
	std::wstring utf16;
	if (utf8.empty()) { return utf16; }
	// Fail if invalid characters run into
	DWORD kFlags = 0;
	if (!onXP()) {
		kFlags |= MB_ERR_INVALID_CHARS;
	}
	if (utf8.length() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
		throw std::overflow_error("Input string too long");
	}
	const int utf8Len = static_cast<int>(utf8.length());
	const int utf16Len = ::MultiByteToWideChar(
		CP_UTF8,
		kFlags,
		utf8.data(),
		utf8Len,
		nullptr,
		0
	);
	if (utf16Len == 0) {
		throw unicode_error(
			"MultiByteToWideChar length-retrieval failed",
			::GetLastError());
	}
	utf16.resize(utf16Len);
	int result = ::MultiByteToWideChar(
		CP_UTF8,
		kFlags,
		utf8.data(),
		utf8Len,
		&utf16[0],
		utf16Len
	);
	if (result == 0) {
		throw unicode_error(
			"MultiByteToWideChar failed",
			::GetLastError()
		);
	}
	return utf16;
}

std::string utf16ToUtf8(const std::wstring & utf16) {
	std::string utf8;
	if (utf16.empty()) { return utf8; }
	// Fail if invalid characters run into
	DWORD kFlags = 0;
	if (!onXP()) {
		kFlags |= WC_ERR_INVALID_CHARS;
	}
	if (utf16.length() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
		throw std::overflow_error("Input string too long");
	}
	const int utf16Len = static_cast<int>(utf16.length());
	const int utf8Len = ::WideCharToMultiByte(
		CP_UTF8,
		kFlags,
		utf16.data(),
		utf16Len,
		nullptr,
		0,
		nullptr, nullptr
	);
	if (utf8Len == 0) {
		const DWORD err = ::GetLastError();
		throw unicode_error(
			err == ERROR_NO_UNICODE_TRANSLATION ?
			"Invalid UTF-16 sequence found in input string" :
			"WideCharToMultiByte length-retrieval failed",
			err);
	}
	utf8.resize(utf8Len);
	int result = ::WideCharToMultiByte(
		CP_UTF8,
		kFlags,
		utf16.data(),
		utf16Len,
		&utf8[0],
		utf8Len,
		nullptr, nullptr
	);
	if (result == 0) {
		const DWORD err = ::GetLastError();
		throw unicode_error(
			err == ERROR_NO_UNICODE_TRANSLATION ?
			"Invalid UTF-16 sequence found in input string" :
			"WideCharToMultiByte failed",
			err);
	}
	return utf8;
}

} // namespace unicode
} // namespace wyre

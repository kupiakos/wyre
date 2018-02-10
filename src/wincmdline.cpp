
#include "wincmdline.h"
#include <array>

namespace wyre {
namespace win32 {

/*
Escape single command lines parameter as appropriate for CommandLineToArgv/CreateProcess.

Note: NOT safe for cmd.exe!
See https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/

Extra testing of escaping functionality drastically needed.
*/
std::wstring escapeArg(const std::wstring & arg, bool forceQuote) {
	if (!forceQuote && !arg.empty() &&
			arg.find_first_of(L" \t\n\v\"") == arg.npos) {
		// Don't need to escape anything, leave as-is.
		return arg;
	}

	// Start our opening quote
	std::wstring result = L"\"";

	for (auto cIt = arg.begin(); ; ++cIt) {
		unsigned numBackslash = 0;

		while (cIt != arg.end() && *cIt == L'\\') {
			++cIt;
			++numBackslash;
		}

		if (cIt == arg.end()) {
			// Don't affect the end quote, but double the backslashes to preserve them.
			result.append(numBackslash * 2, L'\\');
			break;
		} else if (*cIt == L'"') {
			// Escape the backslashes and the quote
			result.append(numBackslash * 2 + 1, L'\\');
			result.push_back(*cIt);
		} else {
			// Backslashes are otherwise unaffected
			result.append(numBackslash, L'\\');
			result.push_back(*cIt);
		}
	}

	result.push_back(L'"');
	return result;
}

// Escape cmd.exe metacharacters.
// The onlyQuotes parameter causes only " to be escaped for the purpose
// of not screwing up arguments.
std::wstring escapeCmd(const std::wstring & text, bool onlyQuotes) {
	std::wstring result;
	const wchar_t * escaping = onlyQuotes ? L"\"" : L"()%!^\"<>&|";
	size_t pos = 0, next;
	while (pos < text.size()) {
		next = text.find_first_of(escaping, pos);
		if (next == text.npos) {
			result.append(text, pos);
			break;
		} else {
			result.append(text, pos, next - pos);
			result.push_back(L'^');
			result.push_back(text[next]);
			pos = next + 1;
		}
	}
	return result;
}

constexpr std::array<const wchar_t *, 22> forbiddenNames{
	L"CON", L"PRN", L"AUX", L"NUL",
	L"COM1", L"COM2", L"COM3", L"COM4",
	L"COM5", L"COM6", L"COM7", L"COM8", L"COM9",
	L"LPT1", L"LPT2", L"LPT3", L"LPT4",
	L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9"
};

// reserved characters outside of \x00
constexpr std::array<const wchar_t, 9> reservedChars{
	L'<', L'>', L':', L'"', L'/', L'\\', L'|', L'?', L'*'
};

template <typename ValueType, typename Container>
constexpr bool in(const ValueType & value, const Container & c) {
	return std::find(c.cbegin(), c.cend(), value) != c.cend();
}

template <typename CharT> constexpr const CharT * hexDigits() {
	static_assert(false, "Invalid character type");
}
template <> constexpr const char * hexDigits() {
	return "0123456789ABCDEF";
}
template <> constexpr const wchar_t * hexDigits() {
	return L"0123456789ABCDEF";
}

// Adapted from https://stackoverflow.com/a/33447587
template <typename CharT, typename I>
constexpr std::basic_string<CharT> n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
	static const CharT * digits = hexDigits<CharT>();
	std::basic_string<CharT> rc(hex_len, digits[0]);
	for (size_t i = 0, j = (hex_len - 1) * 4; i<hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}

std::wstring escapeFile(const std::wstring & name) {
	if (name.empty()) { return L""; }
	std::wstring result;

	if (in(name.substr(0, name.find(L'.', 0)), forbiddenNames)) {
		result.push_back(L'#');
	}

	for (auto c : name) {
		if (c >= 0 && c <= 31 || in(c, reservedChars)) {
			// url-style escape
			result.push_back(L'%');
			result.append(n2hexstr<wchar_t>(c, 2));
		} else {
			result.push_back(c);
		}
	}
	wchar_t last = *result.crbegin();
	if (last == L' ' || last == L'.') {
		result.push_back('#');
	}
	return result;
}

} // namespace win32
} // namespace wyre

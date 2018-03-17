
#include "path_util.h"
#include <array>

namespace wyre {

template <typename CharT>
constexpr std::array<const CharT *, 22> forbiddenNames = {};

template <>
constexpr std::array<const char *, 22> forbiddenNames<char>{
	"CON", "PRN", "AUX", "NUL",
	"COM1", "COM2", "COM3", "COM4",
	"COM5", "COM6", "COM7", "COM8", "COM9",
	"LPT1", "LPT2", "LPT3", "LPT4",
	"LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
};

template<>
constexpr std::array<const wchar_t *, 22> forbiddenNames<wchar_t>{
	L"CON", L"PRN", L"AUX", L"NUL",
	L"COM1", L"COM2", L"COM3", L"COM4",
	L"COM5", L"COM6", L"COM7", L"COM8", L"COM9",
	L"LPT1", L"LPT2", L"LPT3", L"LPT4",
	L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9"
};

// reserved characters outside of \x00
template <typename CharT>
constexpr std::array<const CharT, 9> reservedChars = {};

template <>
constexpr std::array<const char, 9> reservedChars<char>{
	'<', '>', ':', '"', '/', '\\', '|', '?', '*'
};

template <>
constexpr std::array<const wchar_t, 9> reservedChars<wchar_t>{
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

template <typename CharT>
std::basic_string<CharT> escapeFile(const std::basic_string<CharT> & name) {
	std::basic_string<CharT> result;
	if (name.empty()) { return result; }

	if (in(name.substr(0, name.find('.', 0)),
			forbiddenNames<CharT>)) {
		result.push_back('#');
	}

	for (auto c : name) {
		if (c >= 0 && c <= 31 || in(c, reservedChars<CharT>)) {
			// url-style escape
			result.push_back('%');
			result.append(n2hexstr<CharT>(c, 2));
		} else {
			result.push_back(c);
		}
	}
	auto last = *result.crbegin();
	if (last == ' ' || last == '.') {
		result.push_back('#');
	}
	return result;
}

template std::string escapeFile<char>(const std::string &);
template std::wstring escapeFile<wchar_t>(const std::wstring &);

} // namespace wyre

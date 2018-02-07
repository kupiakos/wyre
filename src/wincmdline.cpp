
#include "wincmdline.h"

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

} // namespace win32
} // namespace wyre

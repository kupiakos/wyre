
#ifndef _WIN_CMD_LINE_H_
#define _WIN_CMD_LINE_H_

#include <string>
#include <vector>

namespace wyre {
namespace win32 {

std::wstring escapeArg(const std::wstring & arg, bool forceQuote);
std::wstring escapeCmd(const std::wstring & text, bool onlyQuotes);

// protect a file name to protect against https://msdn.microsoft.com/en-us/library/aa365247
// not designed to be perfectly reversible.
std::wstring escapeFile(const std::wstring & name);

} // namespace win32
} // namespace wyre

#endif

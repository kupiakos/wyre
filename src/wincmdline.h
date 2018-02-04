
#ifndef _WIN_CMD_LINE_H_
#define _WIN_CMD_LINE_H_

#include <string>
#include <vector>

namespace wyre {
namespace win32 {

std::wstring escapeArg(const std::wstring & arg, bool forceQuote);
std::wstring escapeCmd(const std::wstring & text, bool onlyQuotes);

} // namespace win32
} // namespace wyre

#endif

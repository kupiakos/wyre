#pragma once

#include <string>

namespace wyre {


// protect a file name to protect against https://msdn.microsoft.com/en-us/library/aa365247
// not designed to be perfectly reversible.
template <typename CharT>
std::basic_string<CharT> escapeFile(const std::basic_string<CharT> & name);

} // namespace wyre

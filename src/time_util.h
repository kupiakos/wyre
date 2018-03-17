#pragma once

#include <ctime>
#include <sstream>
#include <locale>

namespace wyre {

std::ostream& formatDateTime(std::ostream& out, const tm& t, const char* fmt);
std::string dateTimeToString(const tm& t, const char* format);
tm now();

} // namespace wyre
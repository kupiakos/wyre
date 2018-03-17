#include "time_util.h"

namespace wyre {

std::ostream& formatDateTime(std::ostream& out, const tm& t, const char* fmt) {
	const auto& dateWriter = std::use_facet<std::time_put<char>>(out.getloc());
	auto n = std::strlen(fmt);
	if (dateWriter.put(out, out, ' ', &t, fmt, fmt + n).failed()) {
		throw std::runtime_error("failure to format date time");
	}
	return out;
}

std::string dateTimeToString(const tm& t, const char* format) {
	std::ostringstream s;
	formatDateTime(s, t, format);
	return s.str();
}

tm now() {
	time_t now = time(0);
	tm result;
	localtime_s(&result, &now);
	return result;
}

} // namespace wyre

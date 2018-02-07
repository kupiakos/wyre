#pragma once

#include <stdexcept>

namespace wyre {

class network_error : public std::system_error {
public:
	explicit network_error(const std::string & message) :
#ifdef _WIN32
		std::system_error(
			std::error_code(WSAGetLastError(), std::system_category()),
			message)
#else
		_errorCode(errno)
#endif
		{}
	explicit network_error(const char *message, int errorCode) :
		std::system_error(
			std::error_code(errorCode, std::system_category()),
			message) {}
};

} // namespace wyre
#pragma once

#include <cstdint>
#include <utility>
#include <string>
#include <stdexcept>

namespace wyre {

constexpr uint16_t DEFAULT_PORT = 34215;
inline std::pair<std::string, uint16_t> parseServerSpec(const std::string & serverSpec) {
	size_t colon = serverSpec.find(':');
	if (colon == serverSpec.npos) {
		return std::make_pair(serverSpec, DEFAULT_PORT);
	}
	const char *data = serverSpec.data();
	char *end;
	int port = strtol(data + colon + 1, &end, 10);
	if (port == 0 || *end != '\0') {
		throw std::runtime_error("Invalid port specification");
	}
	return std::make_pair(serverSpec.substr(0, colon), port);
}

} // namespace wyre

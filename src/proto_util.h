#pragma once

#include <string>

namespace wyre {
namespace proto {

constexpr size_t MAX_LENGTH = 65535;

template <typename T>
inline bool readLengthDelimited(std::istream & in, T & proto) {
	// who uses big-endian, let's be real
	uint32_t size;
	in.read(reinterpret_cast<char *>(&size), sizeof(size));
	if (size > MAX_LENGTH) {
		throw std::runtime_error("Invalid protobuf size");
	}
	// TODO: Move outside
	static char readBuf[MAX_LENGTH];
	in.read(readBuf, size);
	return proto.ParseFromArray(readBuf, size);
}

template <typename T>
inline bool writeLengthDelimited(std::ostream & out, const T & proto) {
	auto serialized = proto.SerializeAsString();
	if (serialized.empty()) { return false; }
	uint32_t size = serialized.length();
	if (size > MAX_LENGTH) {
		throw std::runtime_error("Invalid protobuf size");
	}
	out.write(reinterpret_cast<const char *>(&size), sizeof(size));
	out << serialized;
	return true;
}

}
}

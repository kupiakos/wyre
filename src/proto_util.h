#pragma once

#include <string>

namespace wyre {
namespace proto {

template <typename T>
inline bool readLengthDelimited(std::istream & in, T & proto) {
	// who uses big-endian, let's be real
	uint32_t size;
	in.read(reinterpret_cast<char *>(&size), sizeof(size));
	auto buf = std::make_unique<char[]>(size);
	in.read(buf.get(), size);
	return proto.ParseFromArray(buf.get(), size);
}

template <typename T>
inline bool writeLengthDelimited(std::ostream & out, const T & proto) {
	auto serialized = proto.SerializeAsString();
	if (serialized.empty()) { return false; }
	uint32_t size = serialized.length();
	out.write(reinterpret_cast<const char *>(&size), sizeof(size));
	out << serialized;
	return true;
}

}
}

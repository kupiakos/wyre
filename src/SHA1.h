// Adapted from https://github.com/vog/sha1, which is in the public domain.

#ifndef _SHA1_H_
#define _SHA1_H_

#include <cstdint>
#include <iostream>
#include <string>

class SHA1 {
public:
	SHA1();
	void update(const char *s, size_t len);
	void update(const std::string &s);
	void update(std::istream &is);
	std::string hexdigest();
	static std::string from_file(const std::string &filename);

private:

	uint32_t _digest[5];
	std::string _buffer;
	uint64_t _transforms;
};

#endif
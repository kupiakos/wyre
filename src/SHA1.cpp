// Adapted from https://github.com/vog/sha1, which is in the public domain.

#include "SHA1.h"
#include <sstream>
#include <iomanip>
#include <fstream>

namespace {
const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
const std::streamsize BLOCK_BYTES = BLOCK_INTS * 4;


void reset(uint32_t _digest[], std::string &_buffer, uint64_t &_transforms) {
	/* SHA1 initialization constants */
	_digest[0] = 0x67452301;
	_digest[1] = 0xefcdab89;
	_digest[2] = 0x98badcfe;
	_digest[3] = 0x10325476;
	_digest[4] = 0xc3d2e1f0;

	/* Reset counters */
	_buffer = "";
	_transforms = 0;
}


uint32_t rol(const uint32_t value, const size_t bits) {
	return (value << bits) | (value >> (32 - bits));
}


uint32_t blk(const uint32_t block[BLOCK_INTS], const size_t i) {
	return rol(block[(i + 13) & 15] ^ block[(i + 8) & 15] ^ block[(i + 2) & 15] ^ block[i], 1);
}


/*
* (R0+R1), R2, R3, R4 are the different operations used in SHA1
*/

void R0(const uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i) {
	z += ((w&(x^y)) ^ y) + block[i] + 0x5a827999 + rol(v, 5);
	w = rol(w, 30);
}


void R1(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i) {
	block[i] = blk(block, i);
	z += ((w&(x^y)) ^ y) + block[i] + 0x5a827999 + rol(v, 5);
	w = rol(w, 30);
}


void R2(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i) {
	block[i] = blk(block, i);
	z += (w^x^y) + block[i] + 0x6ed9eba1 + rol(v, 5);
	w = rol(w, 30);
}


void R3(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i) {
	block[i] = blk(block, i);
	z += (((w | x)&y) | (w&x)) + block[i] + 0x8f1bbcdc + rol(v, 5);
	w = rol(w, 30);
}


void R4(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i) {
	block[i] = blk(block, i);
	z += (w^x^y) + block[i] + 0xca62c1d6 + rol(v, 5);
	w = rol(w, 30);
}


/*
* Hash a single 512-bit block. This is the core of the algorithm.
*/

void transform(uint32_t _digest[], uint32_t block[BLOCK_INTS], uint64_t &_transforms) {
	/* Copy _digest[] to working vars */
	uint32_t a = _digest[0];
	uint32_t b = _digest[1];
	uint32_t c = _digest[2];
	uint32_t d = _digest[3];
	uint32_t e = _digest[4];

	/* 4 rounds of 20 operations each. Loop unrolled. */
	R0(block, a, b, c, d, e, 0);
	R0(block, e, a, b, c, d, 1);
	R0(block, d, e, a, b, c, 2);
	R0(block, c, d, e, a, b, 3);
	R0(block, b, c, d, e, a, 4);
	R0(block, a, b, c, d, e, 5);
	R0(block, e, a, b, c, d, 6);
	R0(block, d, e, a, b, c, 7);
	R0(block, c, d, e, a, b, 8);
	R0(block, b, c, d, e, a, 9);
	R0(block, a, b, c, d, e, 10);
	R0(block, e, a, b, c, d, 11);
	R0(block, d, e, a, b, c, 12);
	R0(block, c, d, e, a, b, 13);
	R0(block, b, c, d, e, a, 14);
	R0(block, a, b, c, d, e, 15);
	R1(block, e, a, b, c, d, 0);
	R1(block, d, e, a, b, c, 1);
	R1(block, c, d, e, a, b, 2);
	R1(block, b, c, d, e, a, 3);
	R2(block, a, b, c, d, e, 4);
	R2(block, e, a, b, c, d, 5);
	R2(block, d, e, a, b, c, 6);
	R2(block, c, d, e, a, b, 7);
	R2(block, b, c, d, e, a, 8);
	R2(block, a, b, c, d, e, 9);
	R2(block, e, a, b, c, d, 10);
	R2(block, d, e, a, b, c, 11);
	R2(block, c, d, e, a, b, 12);
	R2(block, b, c, d, e, a, 13);
	R2(block, a, b, c, d, e, 14);
	R2(block, e, a, b, c, d, 15);
	R2(block, d, e, a, b, c, 0);
	R2(block, c, d, e, a, b, 1);
	R2(block, b, c, d, e, a, 2);
	R2(block, a, b, c, d, e, 3);
	R2(block, e, a, b, c, d, 4);
	R2(block, d, e, a, b, c, 5);
	R2(block, c, d, e, a, b, 6);
	R2(block, b, c, d, e, a, 7);
	R3(block, a, b, c, d, e, 8);
	R3(block, e, a, b, c, d, 9);
	R3(block, d, e, a, b, c, 10);
	R3(block, c, d, e, a, b, 11);
	R3(block, b, c, d, e, a, 12);
	R3(block, a, b, c, d, e, 13);
	R3(block, e, a, b, c, d, 14);
	R3(block, d, e, a, b, c, 15);
	R3(block, c, d, e, a, b, 0);
	R3(block, b, c, d, e, a, 1);
	R3(block, a, b, c, d, e, 2);
	R3(block, e, a, b, c, d, 3);
	R3(block, d, e, a, b, c, 4);
	R3(block, c, d, e, a, b, 5);
	R3(block, b, c, d, e, a, 6);
	R3(block, a, b, c, d, e, 7);
	R3(block, e, a, b, c, d, 8);
	R3(block, d, e, a, b, c, 9);
	R3(block, c, d, e, a, b, 10);
	R3(block, b, c, d, e, a, 11);
	R4(block, a, b, c, d, e, 12);
	R4(block, e, a, b, c, d, 13);
	R4(block, d, e, a, b, c, 14);
	R4(block, c, d, e, a, b, 15);
	R4(block, b, c, d, e, a, 0);
	R4(block, a, b, c, d, e, 1);
	R4(block, e, a, b, c, d, 2);
	R4(block, d, e, a, b, c, 3);
	R4(block, c, d, e, a, b, 4);
	R4(block, b, c, d, e, a, 5);
	R4(block, a, b, c, d, e, 6);
	R4(block, e, a, b, c, d, 7);
	R4(block, d, e, a, b, c, 8);
	R4(block, c, d, e, a, b, 9);
	R4(block, b, c, d, e, a, 10);
	R4(block, a, b, c, d, e, 11);
	R4(block, e, a, b, c, d, 12);
	R4(block, d, e, a, b, c, 13);
	R4(block, c, d, e, a, b, 14);
	R4(block, b, c, d, e, a, 15);

	/* Add the working vars back into _digest[] */
	_digest[0] += a;
	_digest[1] += b;
	_digest[2] += c;
	_digest[3] += d;
	_digest[4] += e;

	/* Count the number of transformations */
	_transforms++;
}


void _buffer_to_block(const std::string &_buffer, uint32_t block[BLOCK_INTS]) {
	/* Convert the std::string (byte _buffer) to a uint32_t array (MSB) */
	for (size_t i = 0; i < BLOCK_INTS; i++) {
		block[i] = (_buffer[4 * i + 3] & 0xff)
			| (_buffer[4 * i + 2] & 0xff) << 8
			| (_buffer[4 * i + 1] & 0xff) << 16
			| (_buffer[4 * i + 0] & 0xff) << 24;
	}
}

} // anonymous namespace


SHA1::SHA1() {
	reset(_digest, _buffer, _transforms);
}

void SHA1::update(const char *s, size_t len) {
	// Possibly make this more efficient in the future
	update(std::string(s, len));
}

void SHA1::update(const std::string &s) {
	std::istringstream is(s);
	update(is);
}


void SHA1::update(std::istream &is) {
	while (true) {
		char sbuf[BLOCK_BYTES];
		is.read(sbuf, BLOCK_BYTES - _buffer.size());
		_buffer.append(sbuf, static_cast<size_t>(is.gcount()));
		if (_buffer.size() != BLOCK_BYTES) {
			return;
		}
		uint32_t block[BLOCK_INTS];
		_buffer_to_block(_buffer, block);
		transform(_digest, block, _transforms);
		_buffer.clear();
	}
}


/*
* Add padding and return the message _digest.
*/

std::string SHA1::hexdigest() {
	// TODO: Make this not reset the whole thing
	/* Total number of hashed bits */
	uint64_t total_bits = (_transforms*BLOCK_BYTES + _buffer.size()) * 8;

	/* Padding */
	_buffer += '\x80';
	size_t orig_size = _buffer.size();
	while (_buffer.size() < BLOCK_BYTES) {
		_buffer += (char)0x00;
	}

	uint32_t block[BLOCK_INTS];
	_buffer_to_block(_buffer, block);

	if (orig_size > BLOCK_BYTES - 8) {
		transform(_digest, block, _transforms);
		for (size_t i = 0; i < BLOCK_INTS - 2; i++) {
			block[i] = 0;
		}
	}

	/* Append total_bits, split this uint64_t into two uint32_t */
	block[BLOCK_INTS - 1] = static_cast<uint32_t>(total_bits & 0xffffffff);
	block[BLOCK_INTS - 2] = (total_bits >> 32);
	transform(_digest, block, _transforms);

	/* Hex std::string */
	std::ostringstream result;
	for (size_t i = 0; i < sizeof(_digest) / sizeof(_digest[0]); i++) {
		result << std::hex << std::setfill('0') << std::setw(8);
		result << _digest[i];
	}

	/* Reset for next run */
	reset(_digest, _buffer, _transforms);

	return result.str();
}

std::string SHA1::from_file(const std::string &filename) {
	std::ifstream stream(filename.c_str(), std::ios::binary);
	SHA1 checksum;
	checksum.update(stream);
	return checksum.hexdigest();
}


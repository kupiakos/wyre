#pragma once

#include <memory>
#include <iostream>

#include "socket_descriptor.h"
#include "socket.h"
#include "SafeResource.h"

namespace wyre {
constexpr size_t DEFAULT_BUFFER_SIZE = 16384;

class socketbuf : public std::streambuf {
	std::unique_ptr<char[]> _outputBuf;
	std::unique_ptr<char[]> _inputBuf;
	SocketDescriptor _sockfd;
	size_t _bufSize;

public:
	socketbuf(SocketDescriptor && sockfd, size_t bufsize = DEFAULT_BUFFER_SIZE);
	const SocketDescriptor::ResourceType sockfd() const;
	void close() noexcept;

protected:
	virtual int_type underflow() override;
	virtual int_type overflow(int_type extra) override;
	virtual int sync() override;
};
}

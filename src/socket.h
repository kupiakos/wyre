#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <iostream>
#include "socketbuf.h"
#include "SafeResource.h"
#include "socket_descriptor.h"

namespace wyre {

class client_response;
class socketbuf;

class socket : public std::iostream {
private:
	std::unique_ptr<socketbuf> _buf;
	void setSocket(SocketDescriptor && sock);

public:
	socket();
	explicit socket(SocketDescriptor && sock);

	virtual ~socket() {}
	socket(const socket &) = delete;
	socket & operator=(const socket &) = delete;
	socket(socket && other);
	socket & operator=(socket && other);
	void swap(socket & other);

	client_response accept();
	void bind(const std::string & hostname, uint16_t port);
	void shutdown(int how=SD_SEND);
	void close() noexcept;
	void connect(const std::string & hostname, uint16_t port);
	void listen(int max_connections = SOMAXCONN);
	SocketDescriptor::ResourceType sockfd() const;

	template<typename option_type>
	bool setsockopt(int level, int option, const option_type &value = nullptr) {
		return ::setsockopt(sockfd(), level, option, (char *)&value, sizeof(option_type)) != -1;
	}
};

class client_response {
	wyre::socket _socket;
	std::string _address;
	uint16_t _port;

public:
	explicit client_response(SocketDescriptor && sockfd, const sockaddr_in & addr);
	wyre::socket & socket();
	const wyre::socket & socket() const;
	const std::string & address() const;
	const uint16_t port() const;
};

} // namespace wyre

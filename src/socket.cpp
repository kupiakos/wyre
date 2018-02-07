
#include "socket.h"
#include "network_error.h"

#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#else
#include <netdb.h>
#endif

namespace wyre {

socket::socket() :
		std::iostream(nullptr) {
	setf(std::ios_base::unitbuf);
}

socket::socket(SocketDescriptor && sockfd) :
		std::iostream(nullptr) {
	setSocket(std::move(sockfd));
	setf(std::ios_base::unitbuf);
}

socket::socket(socket && other) :
	std::iostream(std::move(other)), _buf(std::move(other._buf)) {}

socket & socket::operator=(socket && other) {
	swap(other);
	return *this;
}

void socket::swap(socket & other) {
	std::iostream::swap(other);
	using std::swap;
	swap(_buf, other._buf);
}

client_response socket::accept() {
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	SocketDescriptor sock = ::accept(
		sockfd(), reinterpret_cast<sockaddr *>(&client_addr), &client_len);
	if (!sock) {
		throw network_error("failed to accept");
	}

	return client_response(std::move(sock), client_addr);
}

void socket::bind(const std::string & hostname, uint16_t port) {
	addrinfo *result = nullptr;
	addrinfo hints;
	int err;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	err = ::getaddrinfo(
		hostname.c_str(), std::to_string(port).c_str(),
		&hints, &result);
	if (err) { throw network_error("getaddrinfo", err); }

	SocketDescriptor sock;
	sock = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (!sock) {
		::freeaddrinfo(result);
		throw network_error("socket failed");
	}

	err = ::bind(sock, result->ai_addr, (int)result->ai_addrlen);
	::freeaddrinfo(result);
	if (err == SOCKET_ERROR) {
		throw network_error("bind failed");
	}

	setSocket(std::move(sock));
}

void socket::shutdown(int how) {
	if (!_buf) { return; }
	_buf->pubsync();
	::shutdown(sockfd(), SD_SEND);
}

void socket::close() noexcept {
	_buf->close();
	this->init(nullptr);
}

void socket::connect(const std::string & hostname, uint16_t port) {
	addrinfo *result = nullptr;
	addrinfo hints;
	int err;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	err = ::getaddrinfo(
		hostname.c_str(), std::to_string(port).c_str(),
		&hints, &result);
	if (err) { throw network_error("getaddrinfo", err); }

	SocketDescriptor sock;
	for (addrinfo *ptr = result; ptr; ptr = ptr->ai_next) {
		sock = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (!sock) {
			throw network_error("socket");
		}
		err = ::connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (err == SOCKET_ERROR) {
			sock.close();
			continue;
		}
		break;
	}
	::freeaddrinfo(result);
	if (!sock) {
		throw network_error("Unable to connect to server", ERROR_CONNECTION_REFUSED);
	}
	setSocket(std::move(sock));
}

void socket::listen(int max_connections) {
	if (::listen(sockfd(), max_connections) < 0) {
		throw network_error("Error listening for incoming connections");
	}
}

SocketDescriptor::ResourceType socket::sockfd() const {
	if (!_buf) {
		return SocketDescriptor::default();
	}
	return _buf->sockfd();
}

void socket::setSocket(SocketDescriptor && sd) {
	auto newBuf = std::make_unique<socketbuf>(std::move(sd));
	this->rdbuf(newBuf.get());
	_buf = std::move(newBuf);
	setf(std::ios_base::unitbuf);
}

namespace {
inline std::string sockaddr_str(const sockaddr_in & addr) {
	char str[INET_ADDRSTRLEN];
	if (!inet_ntop(AF_INET, (void *)&addr.sin_addr, str, INET_ADDRSTRLEN)) {
		throw network_error("Could not translate IP address to string");
	}
	return std::string(str);
}
}

client_response::client_response(SocketDescriptor && sockfd, const sockaddr_in & addr) :
	_socket(std::move(sockfd)),
	_address(sockaddr_str(addr)),
	_port(addr.sin_port) {}

wyre::socket &client_response::socket() {
	return _socket;
}

const wyre::socket &client_response::socket() const {
	return _socket;
}

const std::string &client_response::address() const {
	return _address;
}

const uint16_t client_response::port() const {
	return _port;
}

} // namespace wyre

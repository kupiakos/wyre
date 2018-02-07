
#include "socketbuf.h"
#include "network_error.h"

namespace wyre {

#ifdef _WIN32
typedef int ssize_t;
#endif

socketbuf::socketbuf(SocketDescriptor && sockfd, size_t bufsize) :
		_sockfd(std::move(sockfd)),
		_bufSize(bufsize),
		_outputBuf(std::make_unique<char[]>(bufsize)),
		_inputBuf(std::make_unique<char[]>(bufsize)) {
	setg(0, 0, 0);
	setp(_inputBuf.get(), _inputBuf.get() + bufsize);
	if (!_sockfd) {
		throw network_error("Invalid socket", ERROR_INVALID_HANDLE);
	}
}

const SocketDescriptor::ResourceType socketbuf::sockfd() const {
	return _sockfd.get();
}

void socketbuf::close() noexcept {
	_sockfd.close();
}

socketbuf::int_type socketbuf::underflow() {
	if (gptr() == egptr()) {
		ssize_t total_read = 0;
		while (total_read < _bufSize) {
			auto n_read = recv(
				_sockfd,
				_outputBuf.get() + total_read,
				_bufSize - total_read,
				0);
			if (n_read == 0) {
				return traits_type::eof();
			} else if (n_read < 0) {
				throw network_error("recv failed");
			}
			total_read += n_read;
			if (n_read < _bufSize - total_read - n_read) {
				break;
			}
		}
		//        m_output_buf[total_read] = 0;
		setg(_outputBuf.get(), _outputBuf.get(), _outputBuf.get() + total_read);
	}
	return gptr() == egptr()
		? traits_type::eof()
		: traits_type::to_int_type(*gptr());
}

socketbuf::int_type socketbuf::overflow(socketbuf::int_type extra) {
	auto left_to_write = pptr() - pbase();
	int total_written = 0;
	while (left_to_write) {
		auto n_written = send(_sockfd, pbase() + total_written, left_to_write, 0);
		if (n_written == SOCKET_ERROR) {
			throw network_error("error sending bytes");
		} else if (n_written == 0) {
			return traits_type::eof();
		}
		total_written += n_written;
		left_to_write -= n_written;
	}
	//    m_input_buf[0] = 0;
	setp(_inputBuf.get(), _inputBuf.get() + _bufSize);
	if (!traits_type::eq_int_type(extra, traits_type::eof())) {
		sputc(static_cast<char>(extra));
	}
	return traits_type::to_int_type(*pptr());
}

int socketbuf::sync() {
	auto result = overflow(traits_type::eof());
	return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
}

} // namespace wyre

#include "socket_descriptor.h"
namespace wyre {
#ifdef _WIN32
void _closesocket(SOCKET s) {
	closesocket(s);
}
#else
void _closesocket(int sock) {
	close(sock);
}
#endif
}
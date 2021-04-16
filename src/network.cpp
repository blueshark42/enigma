#include "network.h"

BOOL ConnectToServer() {
	WSADATA wsaData;
	int result;

	auto connSock = INVALID_SOCKET;
	struct sockaddr_in clientService{};

	char *sendbuf = "test test";
	char recvbuf[2048];

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result!=NO_ERROR) {
		std::cout << "WSAStartup: " << result;
		return EXIT_FAILURE;
	}

	connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connSock==INVALID_SOCKET) {
		std::cout << "socket: " << WSAGetLastError();
		WSACleanup();
		return EXIT_FAILURE;
	}

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(IP);
	clientService.sin_port = htons(PORT);

	result = connect(connSock, (SOCKADDR *)&clientService, sizeof(clientService));
	if (result==SOCKET_ERROR) {
		CleanSocket(connSock);
		return EXIT_FAILURE;
	}

	result = send(connSock, sendbuf, (int)strlen(sendbuf), 0);
	if (result==SOCKET_ERROR) {
		CleanSocket(connSock);
		return EXIT_FAILURE;
	}

	do {
		result = recv(connSock, recvbuf, 2048, 0);
		if (result > 0) {
			std::cout << "Received: " << result << std::endl;
		} else if (result==0) {
			std::cout << "Connection closed\n";
		} else {
			std::cout << "recv failed\n";
		}
	} while (result > 0);
	CleanSocket(connSock);

	return 0;
}

void CleanSocket(SOCKET socket) {
	WSACleanup();
	closesocket(socket);
}

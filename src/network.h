#ifndef ENIGMA_SRC_NETWORK_H_
#define ENIGMA_SRC_NETWORK_H_
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define IP "127.0.0.1"
#define PORT 8080

BOOL ConnectToServer();
void CleanSocket(SOCKET socket);
#endif //ENIGMA_SRC_NETWORK_H_

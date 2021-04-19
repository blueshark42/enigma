#ifndef ENIGMA_SRC_NETWORK_H_
#define ENIGMA_SRC_NETWORK_H_

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>

#define IP "127.0.0.1"
#define PORT "8080"

DWORD __stdcall ServerThread();
#endif //ENIGMA_SRC_NETWORK_H_

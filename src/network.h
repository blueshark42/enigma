#ifndef ENIGMA_SRC_NETWORK_H_
#define ENIGMA_SRC_NETWORK_H_

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>

#include "debug.h"

#define DEFAULT_BUFLEN 2048
#define IP "127.0.0.1"
#define PORT "8080"

typedef struct FileParams {
	const char *filePath;
	size_t filePathLen;
} *PFileParams;

DWORD __stdcall ServerThread(LPVOID lpParams);
#endif //ENIGMA_SRC_NETWORK_H_

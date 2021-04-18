#ifndef ENIGMA_SRC_NETWORK_H_
#define ENIGMA_SRC_NETWORK_H_

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>

#define IP "127.0.0.1"
#define PORT "8080"

BOOL ConnectToServer();
DWORD __stdcall ClientThread(LPVOID param, const std::string &file);
void CleanSocket(SOCKET &socket);
int64_t SendLogFile(SOCKET s, const std::string &file, int chunkSize = 64*1024);
int64_t GetFileSize(const std::string &file);
int SendBuffer(SOCKET s, const char *buffer, int buffSize, int chunkSize = 4*1024);
#endif //ENIGMA_SRC_NETWORK_H_

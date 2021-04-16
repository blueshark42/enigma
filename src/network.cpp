#include <fstream>
#include "network.h"

BOOL ConnectToServer() {

}

void CleanSocket(SOCKET &socket) {
	closesocket(socket);
	socket = INVALID_SOCKET;
}
int64_t SendLogFile(SOCKET s, const std::string &file, int chunkSize) {
	const int64_t fileSize = GetFileSize(file);
	if (fileSize < 0) { return -1; }

	std::ifstream iffile(file, std::ifstream::binary);
	if (iffile.fail()) { return -1; }

	if (SendBuffer(s, reinterpret_cast<const char *>(&fileSize), sizeof(fileSize))!=sizeof(fileSize)) {
		return -2;
	}

	char *buffer = new char[chunkSize];
	bool error = false;
	int64_t i = fileSize;
	while (i!=0) {
		const int64_t ssize = __min(i, (int64_t)chunkSize);
		if (!iffile.read(buffer, ssize)) {
			error = true;
			break;
		}
		const int l = SendBuffer(s, buffer, (int)ssize);
		if (l < 0) {
			error = true;
			break;
		}
		i -= l;
	}
	delete[] buffer;
	iffile.close();
	return error ? -3 : fileSize;
}

int64_t GetFileSize(const std::string &file) {
	FILE *f;
	if (fopen_s(&f, file.c_str(), "rb")!=0) {
		return -1;
	}
	_fseeki64(f, 0, SEEK_END);
	const int64_t len = _ftelli64(f);
	fclose(f);
	return len;
}

int SendBuffer(SOCKET s, const char *buffer, int buffSize, int chunkSize) {
	int i = 0;
	while (i < buffSize) {
		const int l = send(s, &buffer[i], __min(chunkSize, buffSize - i), 0);
		if (l < 0) {
			return l;
		}
		i += l;
	}
	return i;
}
DWORD ClientThread(LPVOID param) {
	struct addrinfo hints = {0}, *result, *ptr;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(IP, PORT, &hints, &result)!=0) {
		return -1;
	}

	auto client = INVALID_SOCKET;
	for (ptr = result; ptr!=nullptr; ptr = ptr->ai_next) {
		client = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client==SOCKET_ERROR) {
			CleanSocket(client);
			return -1;
		}
		if (connect(client, ptr->ai_addr, (int)ptr->ai_addrlen)==SOCKET_ERROR) {
			closesocket(client);
			client = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (client==SOCKET_ERROR) {
		std::cout << "Could not create client socket\n";
		return -2;
	}

	int64_t rc = SendFile(client, PATH);
	if (rc < 0) {
		std::cout << "Failed to send file: " << rc << std::endl;
	}
	closesocket(client);
	return 0;
};

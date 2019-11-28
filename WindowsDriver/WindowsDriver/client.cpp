#include "client.h"

client::client(const char* hostname, const char* port) {
	WSADATA wsaData;
	sock_fd = INVALID_SOCKET;

	result = NULL;
	ptr = NULL;
	int res = 0;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		this->~client();
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	res = getaddrinfo(hostname, port, &hints, &result);
	if (res != 0) {
		WSACleanup();
		this->~client();
	}
}

client::~client() {
	result = NULL;
	ptr = NULL;
}

int client::_read(void* data, int len) {
	return send(sock_fd, reinterpret_cast<const char*>(data), len, 0);
}

int client::_write(void* data, int len) {
	return recv(sock_fd, reinterpret_cast<char*>(data), len, 0);
}

int client::_connect() {
	sock_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_fd == INVALID_SOCKET) {
		WSACleanup();
		return INVALID_SOCKET;
	}
	int res = 0;
	res = connect(sock_fd, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		closesocket(sock_fd);
		sock_fd = INVALID_SOCKET;
		return res;
	}
	freeaddrinfo(result);
	return 0;

}

void client::disconnect() {
	shutdown(sock_fd, 0);
}


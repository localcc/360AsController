#include "client.h"
#include <cstdio>

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

int client::_read(char* data, int len) {
	queue_mutex.lock();
	int amount = recv(sock_fd, data, len, 0);
	queue_mutex.unlock();
	return amount;
}

int client::_write(char* data, int len) {
	queue_mutex.lock();
	int amount = send(sock_fd, data, len, 0);
	queue_mutex.unlock();
	return amount;
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


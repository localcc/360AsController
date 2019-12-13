#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>


class client {
public:
	client(const char* hostname, const char* port);
	~client();

	int _connect();
	void disconnect();

	int _read(char* data, int len);
	int _write(char* data, int len);
private:
	SOCKET sock_fd;
	struct addrinfo *result, *ptr, hints;
};
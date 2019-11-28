#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>


class client {
public:
	client(const char* hostname, const char* port);
	~client();

	int _connect();
	void disconnect();

	int _read(void* data, int len);
	int _write(void* data, int len);
private:
	SOCKET sock_fd;
	struct addrinfo *result, *ptr, hints;
};
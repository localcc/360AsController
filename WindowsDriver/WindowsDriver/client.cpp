//
// Created by local on 03.01.2020.
//

#include "client.h"
#include <cstdio>

client::client(const char* hostname, int port) {
    int res = 0;
    WSAData data;
    res = WSAStartup(MAKEWORD(2, 2), &data);
    if (res != 0)
        this->~client();
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd == INVALID_SOCKET)
        this->~client();

    address.sin_family = AF_INET;

    res = InetPton(AF_INET, hostname, &address.sin_addr.s_addr);
    if (res < 0)
        this->~client();
    address.sin_port = htons(port);
}

client::~client() {
    closesocket(sock_fd);
}


int client::client_write(char* arr, int amount) {
    return sendto(sock_fd, arr, amount, 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));
}

int client::client_read(char* arr, int amount) {
    sockaddr_in from;
    int size = sizeof(from);
    int ret = recvfrom(sock_fd, arr, amount, 0, reinterpret_cast<SOCKADDR*>(&from), &size);

    return ret;
}

void client::close_socket() {
    closesocket(sock_fd);
}
//
// Created by local on 03.01.2020.
//


#ifndef UDPCLIENT_CLIENT_H
#define UDPCLIENT_CLIENT_H
#include <ws2tcpip.h>



class client {
public:
    client(const char* hostname, int port);
    ~client();
    int client_read(char* arr, int amount);
    int client_write(char* arr, int amount);
    void close_socket();
private:
    SOCKET sock_fd;
    sockaddr_in address;
};


#endif //UDPCLIENT_CLIENT_H

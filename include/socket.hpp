#pragma once
#include <WinSock2.h>
#include <string>

class ServerSocket {
public:
    explicit ServerSocket(int port);
    ~ServerSocket();

    SOCKET accept();

    ServerSocket(const ServerSocket&) = delete;
    ServerSocket& operator=(const ServerSocket&) = delete;

private:
    SOCKET sock_;
};

class ClientSocket {
public:
    explicit ClientSocket(SOCKET sock);
    ~ClientSocket();

    std::string recv(int maxBytes = 4096);
    void send(const std::string& data);

    ClientSocket(const ClientSocket&) = delete;
    ClientSocket& operator=(const ClientSocket&) = delete;

    ClientSocket(ClientSocket&& other) noexcept;
    ClientSocket& operator=(ClientSocket&& other) noexcept;

private:
    SOCKET sock_;
};

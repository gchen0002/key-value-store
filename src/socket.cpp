#include "socket.hpp"
#include <iostream>
#include <stdexcept>

ServerSocket::ServerSocket(int port) : sock_(INVALID_SOCKET) {
    // init winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }

    // creating the socket
    sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_ == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("socket() failed: " + std::to_string(WSAGetLastError()));
    }

    // binding to port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock_);
        WSACleanup();
        throw std::runtime_error("bind() failed: " + std::to_string(WSAGetLastError()));
    }

    // listen for connections
    if (listen(sock_, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(sock_);
        WSACleanup();
        throw std::runtime_error("listen() failed: " + std::to_string(WSAGetLastError()));
    }
}

ServerSocket::~ServerSocket() {
    if (sock_ != INVALID_SOCKET) {
        closesocket(sock_);
    }
    WSACleanup();
}

SOCKET ServerSocket::accept() {
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSock = ::accept(sock_, (sockaddr*)&clientAddr, &clientSize);
    return clientSock;
}

ClientSocket::ClientSocket(SOCKET sock) : sock_(sock) {}

ClientSocket::~ClientSocket() {
    if (sock_ != INVALID_SOCKET) {
        closesocket(sock_);
    }
}

ClientSocket::ClientSocket(ClientSocket&& other) noexcept
    : sock_(other.sock_) {
    other.sock_ = INVALID_SOCKET;
}

ClientSocket& ClientSocket::operator=(ClientSocket&& other) noexcept {
    if (this != &other) {
        if (sock_ != INVALID_SOCKET) {
            closesocket(sock_);
        }
        sock_ = other.sock_;
        other.sock_ = INVALID_SOCKET;
    }
    return *this;
}

std::string ClientSocket::recv(int maxBytes) {
    std::string buffer(maxBytes, '\0');
    int bytesReceived = ::recv(sock_, &buffer[0], maxBytes, 0);
    if (bytesReceived <= 0) {
        return "";
    }
    buffer.resize(bytesReceived);
    return buffer;
}

void ClientSocket::send(const std::string& data) {
    int totalSent = 0;
    while (totalSent < static_cast<int>(data.size())) {
        int sent = ::send(sock_, data.c_str() + totalSent,
                          data.size() - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            throw std::runtime_error("send failed");
        }
        totalSent += sent;
    }
}

#include "socket.hpp"
#include <stdexcept>

ServerSocket::ServerSocket(int port) : sock_(INVALID_SOCKET) {
}

ServerSocket::~ServerSocket() {
}

SOCKET ServerSocket::accept() {
    return INVALID_SOCKET;
}

ClientSocket::ClientSocket(SOCKET sock) : sock_(sock) {
}

ClientSocket::~ClientSocket() {
}

std::string ClientSocket::recv(int maxBytes) {
    return "";
}

void ClientSocket::send(const std::string& data) {
}

ClientSocket::ClientSocket(ClientSocket&& other) noexcept : sock_(other.sock_) {
    other.sock_ = INVALID_SOCKET;
}

ClientSocket& ClientSocket::operator=(ClientSocket&& other) noexcept {
    if (this != &other) {
        sock_ = other.sock_;
        other.sock_ = INVALID_SOCKET;
    }
    return *this;
}

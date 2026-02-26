#include "server.hpp"
#include "request.hpp"
#include <iostream>

Server::Server(int port, Store& store, size_t threads)
    : port_(port), store_(store), threadCount_(threads), pool_(threads) {
}

void Server::start() {
}

void Server::run() {
}

Server::~Server() {
}

void Server::handleClient(ClientSocket client) {
}

std::string Server::extractKeyFromPath(const std::string& path) const {
    return "";
}

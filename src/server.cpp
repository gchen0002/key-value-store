#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>

Server::Server(int port, Store& store, size_t threads)
    : port_(port), store_(store), threadCount_(threads), pool_(threads) {
}

void Server::start() {
    serverSocket_ = std::make_unique<ServerSocket>(port_);
    std::cout << "Server listening on port " << port_ << std::endl;
}

void Server::run() {
    while (true) {
        SOCKET clientSock = serverSocket_->accept();
        if (clientSock == INVALID_SOCKET) {
            continue;
        }

        pool_.enqueue([this, clientSock]() {
            handleClient(ClientSocket(clientSock));
        });
    }
}

Server::~Server() {
}

void Server::handleClient(ClientSocket client) {
    std::string rawRequest = client.recv();
    if (rawRequest.empty()) {
        return;
    }

    Request request;
    if (!request.parse(rawRequest)) {
        Response response = Response::badRequest();
        client.send(response.build());
        return;
    }

    std::string path = request.path();
    std::string key = extractKeyFromPath(path);

    if (key.empty()) {
        Response response = Response::badRequest();
        client.send(response.build());
        return;
    }

    Response response;

    if (request.method() == "GET") {
        auto value = store_.get(key);
        if (value.has_value()) {
            response = Response::ok().setBody(*value);
        } else {
            response = Response::notFound();
        }
    }
    else if (request.method() == "PUT") {
        std::string value = request.body();
        if (value.empty()) {
            response = Response::badRequest();
        } else {
            std::optional<int> ttl;
            if (request.hasQueryParam("ttl")) {
                ttl = std::stoi(request.queryParam("ttl"));
            }

            bool existed = store_.set(key, value, ttl);
            if (existed) {
                response = Response::ok();
            } else {
                response = Response::created();
            }
        }
    }
    else if (request.method() == "DELETE") {
        bool existed = store_.remove(key);
        if (existed) {
            response = Response::ok();
        } else {
            response = Response::notFound();
        }
    }
    else {
        response = Response::methodNotAllowed();
    }

    client.send(response.build());
}

std::string Server::extractKeyFromPath(const std::string& path) const {
    const std::string prefix = "/keys/";
    if (path.find(prefix) != 0) {
        return "";
    }
    return path.substr(prefix.length());
}

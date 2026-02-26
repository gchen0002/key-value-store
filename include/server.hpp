#pragma once
#include "socket.hpp"
#include "store.hpp"
#include "thread_pool.hpp"
#include <thread>
#include <memory>

class Server {
public:
    explicit Server(int port, 
                    Store& store,
                    size_t threads = std::thread::hardware_concurrency());
    void start();
    void run();
    ~Server();

private:
    void handleClient(ClientSocket client);
    std::string extractKeyFromPath(const std::string& path) const;

    int port_;
    Store& store_;
    size_t threadCount_;
    ThreadPool pool_;
    std::unique_ptr<ServerSocket> serverSocket_;
};

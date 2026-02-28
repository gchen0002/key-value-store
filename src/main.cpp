#include "server.hpp"
#include "store.hpp"
#include "config.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    // Get executable directory for config file
    std::filesystem::path exePath(argv[0]);
    std::filesystem::path exeDir = exePath.parent_path();
    
    // Load config from executable directory
    Config config;
    std::string configPath = (exeDir / "config.ini").string();
    
    // Also try parent directory (for Debug/Release builds)
    if (!std::filesystem::exists(configPath)) {
        configPath = (exeDir / ".." / "config.ini").string();
    }
    
    if (!config.load(configPath)) {
        std::cout << "Using default configuration" << std::endl;
    }
    
    std::cout << "KV-Store v1.0" << std::endl;
    std::cout << "Port: " << config.getPort() << std::endl;
    std::cout << "Threads: " << config.getThreadCount() << std::endl;
    std::cout << "Capacity: " << config.getMaxCapacity() << std::endl;
    std::cout << "Log Level: " << config.getLogLevel() << std::endl;
    std::cout << std::endl;
    
    try {
        Store store(config.getMaxCapacity());
        Server server(config.getPort(), store, config.getThreadCount());
        
        server.start();
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

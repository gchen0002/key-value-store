#include "server.hpp"
#include "store.hpp"
#include <iostream>

int main() {
    Store store(10000);
    Server server(8080, store);
    
    std::cout << "KV-Store starting on port 8080..." << std::endl;
    
    server.start();
    server.run();
    
    return 0;
}

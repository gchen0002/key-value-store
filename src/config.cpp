#include "config.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

bool Config::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file: " << filepath << std::endl;
        std::cerr << "Using default values" << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }
    
    return true;
}

void Config::parseLine(const std::string& line) {
    std::string trimmed = trim(line);
    
    // Skip empty lines and comments
    if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') {
        return;
    }
    
    // Skip section headers [server], [logging], etc.
    if (trimmed[0] == '[' && trimmed.back() == ']') {
        return;
    }
    
    // Find equals sign
    size_t eqPos = trimmed.find('=');
    if (eqPos == std::string::npos) {
        return; // Invalid line, skip
    }
    
    std::string key = trim(trimmed.substr(0, eqPos));
    std::string value = trim(trimmed.substr(eqPos + 1));
    
    // Parse known keys
    try {
        if (key == "port") {
            int port = std::stoi(value);
            if (port > 0 && port <= 65535) {
                port_ = port;
            } else {
                std::cerr << "Warning: Invalid port " << value 
                          << ", using default " << port_ << std::endl;
            }
        } else if (key == "thread_count") {
            int threads = std::stoi(value);
            if (threads > 0) {
                threadCount_ = static_cast<size_t>(threads);
            } else {
                std::cerr << "Warning: Invalid thread_count " << value 
                          << ", using default " << threadCount_ << std::endl;
            }
        } else if (key == "max_capacity") {
            int capacity = std::stoi(value);
            if (capacity > 0) {
                maxCapacity_ = static_cast<size_t>(capacity);
            } else {
                std::cerr << "Warning: Invalid max_capacity " << value 
                          << ", using default " << maxCapacity_ << std::endl;
            }
        } else if (key == "level") {
            logLevel_ = value;
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not parse " << key << "=" << value 
                  << ", using default. Error: " << e.what() << std::endl;
    }
}

std::string Config::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

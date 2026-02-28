#pragma once
#include <string>

class Config {
public:
    bool load(const std::string& filepath);
    
    int getPort() const { return port_; }
    size_t getThreadCount() const { return threadCount_; }
    size_t getMaxCapacity() const { return maxCapacity_; }
    std::string getLogLevel() const { return logLevel_; }

private:
    int port_ = 8080;
    size_t threadCount_ = 4;
    size_t maxCapacity_ = 10000;
    std::string logLevel_ = "info";
    
    void parseLine(const std::string& line);
    std::string trim(const std::string& str);
};

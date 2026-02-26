#pragma once
#include <string>
#include <optional>
#include <chrono>
#include <list>
#include <list>
#include <unordered_map>
#include <mutex>

class Store {
public:
    explicit Store(size_t maxCapacity = 10000);

    std::optional<std::string> get(const std::string& key);
    bool set(const std::string& key, const std::string& value, std::optional<int> ttlSeconds = std::nullopt);
    bool remove(const std::string& key);
    bool exists(const std::string& key);

private:
    struct Entry {
        std::string value;
        std::chrono::steady_clock::time_point expiresAt;
        bool hasTtl = false;
        std::list<std::string>::iterator lruIt;
    };

    std::unordered_map<std::string, Entry> data_;
    std::list<std::string> lruList_;
    std::mutex mutex_;
    size_t maxCapacity_;

    bool isExpired(const Entry& entry) const;
    void evictLru();
    void updateLru(const std::string& key, Entry& entry);
};

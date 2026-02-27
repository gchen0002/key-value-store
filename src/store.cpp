#include "store.hpp"

Store::Store(size_t maxCapacity) : maxCapacity_(maxCapacity) {
}

std::optional<std::string> Store::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    // not found
    if (it == data_.end()) {
        return std::nullopt;
    }
    if (isExpired(it->second)) {
        return std::nullopt;
    }
    return it->second.value;
}

bool Store::set(const std::string& key, const std::string& value, std::optional<int> ttlSeconds) {
    // lock first to prevent race conditions and ensure thread safety
    std::lock_guard<std::mutex> lock(mutex_);

    bool existed = (data_.find(key) != data_.end());

    Entry entry;
    entry.value = value;
    entry.hasTtl = ttlSeconds.has_value();

    if (ttlSeconds.has_value()) {
        entry.expiresAt = std::chrono::steady_clock::now() + 
                          std::chrono::seconds(ttlSeconds.value());
    }

    data_[key] = entry;

    return existed;
}

bool Store::remove(const std::string& key) {
    // lock first
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it == data_.end()) {
        return false;
    }
    data_.erase(it);
    return true;
}

bool Store::exists(const std::string& key) {
    // lock first
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it == data_.end()) {
        return false;
    }
    // if expired, treat as not exist
    return !isExpired(it->second);
}

bool Store::isExpired(const Entry& entry) const {
    if (!entry.hasTtl) {
        return false;
    }
    return std::chrono::steady_clock::now() >= entry.expiresAt;
}

void Store::evictLru() {
}

void Store::updateLru(const std::string& key, Entry& entry) {
}

#include "store.hpp"

Store::Store(size_t maxCapacity) : maxCapacity_(maxCapacity) {
}

std::optional<std::string> Store::get(const std::string& key) {
    return std::nullopt;
}

bool Store::set(const std::string& key, const std::string& value, std::optional<int> ttlSeconds) {
    return false;
}

bool Store::remove(const std::string& key) {
    return false;
}

bool Store::exists(const std::string& key) {
    return false;
}

bool Store::isExpired(const Entry& entry) const {
    return false;
}

void Store::evictLru() {
}

void Store::updateLru(const std::string& key, Entry& entry) {
}

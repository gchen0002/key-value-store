#include "store.hpp"

Store::Store(size_t maxCapacity) : maxCapacity_(maxCapacity) {
}

std::optional<std::string> Store::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it == data_.end()) {
        return std::nullopt;
    }
    if (isExpired(it->second)) {
        // Clean up expired entry from both structures
        lruList_.erase(it->second.lruIt);
        data_.erase(it);
        return std::nullopt;
    }
    updateLru(key, it->second);
    return it->second.value;
}

bool Store::set(const std::string& key, const std::string& value, std::optional<int> ttlSeconds) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    bool existed = (it != data_.end());

    if (existed) {
        // Update existing entry - preserve the LRU iterator
        it->second.value = value;
        it->second.hasTtl = ttlSeconds.has_value();
        if (ttlSeconds.has_value()) {
            it->second.expiresAt = std::chrono::steady_clock::now() + 
                                   std::chrono::seconds(ttlSeconds.value());
        }
        updateLru(key, it->second);
    } else {
        // New entry - check capacity first
        if (data_.size() >= maxCapacity_) {
            evictLru();
        }
        
        // Create new entry with uninitialized iterator
        Entry entry;
        entry.value = value;
        entry.hasTtl = ttlSeconds.has_value();
        if (ttlSeconds.has_value()) {
            entry.expiresAt = std::chrono::steady_clock::now() + 
                              std::chrono::seconds(ttlSeconds.value());
        }
        entry.lruIt = lruList_.end();  // Initialize to end (not in list yet)
        
        // Insert into map
        auto result = data_.emplace(key, std::move(entry));
        // Add to LRU list
        updateLru(key, result.first->second);
    }

    return existed;
}

bool Store::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it == data_.end()) {
        return false;
    }
    // Erase from list first
    lruList_.erase(it->second.lruIt);
    data_.erase(it);
    return true;
}

bool Store::exists(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it == data_.end()) {
        return false;
    }
    if (isExpired(it->second)) {
        // Clean up expired entry
        lruList_.erase(it->second.lruIt);
        data_.erase(it);
        return false;
    }
    return true;
}

bool Store::isExpired(const Entry& entry) const {
    if (!entry.hasTtl) {
        return false;
    }
    return std::chrono::steady_clock::now() >= entry.expiresAt;
}

void Store::evictLru() {
    if (lruList_.empty()) {
        return;
    }
    // Get key to evict (copy, not reference)
    std::string keyToEvict = lruList_.back();
    // Erase from map first
    data_.erase(keyToEvict);
    // Then remove from list
    lruList_.pop_back();
}

void Store::updateLru(const std::string& key, Entry& entry) {
    // If already in list (iterator != end), move to front
    if (entry.lruIt != lruList_.end()) {
        lruList_.splice(lruList_.begin(), lruList_, entry.lruIt);
    } else {
        // New entry: insert at front
        entry.lruIt = lruList_.insert(lruList_.begin(), key);
    }
}

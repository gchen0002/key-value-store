#include "request.hpp"
#include <sstream>

bool Request::parse(const std::string& rawRequest) {
    std::istringstream stream(rawRequest);
    std::string line;

    // Parse request line
    if (!std::getline(stream, line)) {
        return false;
    }

    // Remove trailing \r if present
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    if (!parseRequestLine(line)) {
        return false;
    }

    // Parse headers
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            break;  // End of headers
        }

        parseHeader(line);
    }

    // Parse body if Content-Length present
    if (hasHeader("Content-Length")) {
        int contentLength = std::stoi(header("Content-Length"));
        if (contentLength > 0) {
            body_.resize(contentLength);
            stream.read(&body_[0], contentLength);
        }
    }

    return true;
}

bool Request::parseRequestLine(const std::string& line) {
    std::istringstream stream(line);

    if (!(stream >> method_)) {
        return false;
    }

    std::string fullPath;
    if (!(stream >> fullPath)) {
        return false;
    }

    // Split path and query string
    size_t queryPos = fullPath.find('?');
    if (queryPos != std::string::npos) {
        path_ = fullPath.substr(0, queryPos);
        queryString_ = fullPath.substr(queryPos + 1);
        parseQueryString(queryString_);
    } else {
        path_ = fullPath;
    }

    if (!(stream >> version_)) {
        return false;
    }

    return true;
}

void Request::parseHeader(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) {
        return;
    }

    std::string name = line.substr(0, colonPos);
    std::string value = line.substr(colonPos + 1);

    // Trim leading whitespace from value
    size_t start = value.find_first_not_of(" \t");
    if (start != std::string::npos) {
        value = value.substr(start);
    }

    headers_[name] = value;
}

void Request::parseQueryString(const std::string& query) {
    std::istringstream stream(query);
    std::string pair;

    while (std::getline(stream, pair, '&')) {
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos) {
            std::string name = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            queryParams_[name] = value;
        }
    }
}

std::string Request::header(const std::string& name) const {
    auto it = headers_.find(name);
    return it != headers_.end() ? it->second : "";
}

bool Request::hasHeader(const std::string& name) const {
    return headers_.find(name) != headers_.end();
}

std::string Request::queryParam(const std::string& name) const {
    auto it = queryParams_.find(name);
    return it != queryParams_.end() ? it->second : "";
}

bool Request::hasQueryParam(const std::string& name) const {
    return queryParams_.find(name) != queryParams_.end();
}

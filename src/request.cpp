#include "request.hpp"
#include <sstream>

bool Request::parse(const std::string& rawRequest) {
    return false;
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

bool Request::parseRequestLine(const std::string& line) {
    return false;
}

void Request::parseHeader(const std::string& line) {
}

void Request::parseQueryString(const std::string& query) {
}

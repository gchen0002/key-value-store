#include "response.hpp"

Response& Response::setStatus(int code, std::string reason) {
    statusCode_ = code;
    statusReason_ = std::move(reason);
    return *this;
}

Response& Response::setHeader(const std::string& name, const std::string& value) {
    headers_[name] = value;
    return *this;
}

Response& Response::setBody(std::string body) {
    body_ = std::move(body);
    return *this;
}

std::string Response::build() const {
    std::string result;

    // Status line: HTTP/1.1 200 OK\r\n
    result = "HTTP/1.1 " + std::to_string(statusCode_) + " " + statusReason_ + "\r\n";

    // Headers from map
    for (const auto& [name, value] : headers_) {
        result += name + ": " + value + "\r\n";
    }

    // Auto-add Content-Length based on body size
    result += "Content-Length: " + std::to_string(body_.size()) + "\r\n";

    // Blank line (end of headers)
    result += "\r\n";

    // Body
    result += body_;
    return result;
}

Response Response::ok() {
    Response r;
    r.statusCode_ = 200;
    r.statusReason_ = "OK";
    return r;
}

Response Response::created() {
    Response r;
    r.statusCode_ = 201;
    r.statusReason_ = "Created";
    return r;
}

Response Response::badRequest() {
    Response r;
    r.statusCode_ = 400;
    r.statusReason_ = "Bad Request";
    return r;
}

Response Response::notFound() {
    Response r;
    r.statusCode_ = 404;
    r.statusReason_ = "Not Found";
    return r;
}

Response Response::methodNotAllowed() {
    Response r;
    r.statusCode_ = 405;
    r.statusReason_ = "Method Not Allowed";
    return r;
}

Response Response::internalServerError() {
    Response r;
    r.statusCode_ = 500;
    r.statusReason_ = "Internal Server Error";
    return r;
}

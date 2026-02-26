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
    return "";
}

Response Response::ok() {
    return Response().setStatus(200, "OK");
}

Response Response::created() {
    return Response().setStatus(201, "Created");
}

Response Response::badRequest() {
    return Response().setStatus(400, "Bad Request");
}

Response Response::notFound() {
    return Response().setStatus(404, "Not Found");
}

Response Response::methodNotAllowed() {
    return Response().setStatus(405, "Method Not Allowed");
}

Response Response::internalServerError() {
    return Response().setStatus(500, "Internal Server Error");
}

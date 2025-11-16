#include "HTTPParser.h"
#include <sstream>
#include <algorithm>

HTTPRequest HTTPParser::parse(const std::string& rawRequest) {
    HTTPRequest req;
    std::istringstream iss(rawRequest);
    std::string line;
    
    // Parse request line
    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        lineStream >> req.method >> req.path;
    }
    
    // Parse headers
    while (std::getline(iss, line) && line != "\r" && line != "") {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            req.headers[key] = value;
        }
    }
    
    // Parse body
    std::ostringstream bodyStream;
    while (std::getline(iss, line)) {
        bodyStream << line << "\n";
    }
    req.body = bodyStream.str();
    if (!req.body.empty() && req.body.back() == '\n') {
        req.body.pop_back();
    }
    
    return req;
}

std::string HTTPParser::buildResponse(int statusCode, const std::string& body,
                                     const std::map<std::string, std::string>& headers) {
    std::ostringstream response;
    std::map<int, std::string> statusMessages = {
        {200, "OK"}, {201, "Created"}, {400, "Bad Request"},
        {404, "Not Found"}, {500, "Internal Server Error"}
    };
    
    response << "HTTP/1.1 " << statusCode << " " << statusMessages[statusCode] << "\r\n";
    response << "Content-Type: application/json\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    
    for (const auto& header : headers) {
        response << header.first << ": " << header.second << "\r\n";
    }
    
    response << "\r\n" << body;
    return response.str();
}

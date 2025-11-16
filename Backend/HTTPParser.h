#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <string>
#include <map>

struct HTTPRequest {
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
};

class HTTPParser {
public:
    static HTTPRequest parse(const std::string& rawRequest);
    static std::string buildResponse(int statusCode, const std::string& body, 
                                    const std::map<std::string, std::string>& headers = {});
};

#endif

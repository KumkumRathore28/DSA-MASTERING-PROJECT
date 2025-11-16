#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <map>
#include <vector>

class JSONParser {
public:
    static std::string toJSON(const std::map<std::string, std::string>& obj);
    static std::string toJSONArray(const std::vector<std::map<std::string, std::string>>& arr);
    static std::string escapeString(const std::string& str);
};

#endif

#include "JSONParser.h"
#include <sstream>

std::string JSONParser::escapeString(const std::string& str) {
    std::ostringstream o;
    for (char c : str) {
        if (c == '"') o << "\\\"";
        else if (c == '\\') o << "\\\\";
        else if (c == '\n') o << "\\n";
        else if (c == '\r') o << "\\r";
        else if (c == '\t') o << "\\t";
        else o << c;
    }
    return o.str();
}

std::string JSONParser::toJSON(const std::map<std::string, std::string>& obj) {
    std::ostringstream json;
    json << "{";
    bool first = true;
    for (const auto& pair : obj) {
        if (!first) json << ",";
        json << "\"" << escapeString(pair.first) << "\":\"" << escapeString(pair.second) << "\"";
        first = false;
    }
    json << "}";
    return json.str();
}

std::string JSONParser::toJSONArray(const std::vector<std::map<std::string, std::string>>& arr) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i > 0) json << ",";
        json << toJSON(arr[i]);
    }
    json << "]";
    return json.str();
}

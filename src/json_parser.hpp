#pragma once
#include "../include/json.hpp"
#include <string>
#include <iostream>

using json = nlohmann::json;

namespace json_parser {

class JSONParser {
public:
    static json FromStringToJSON(const std::string& input);
    static json FromStreamToJSON(std::istream& in);
    static std::string FromJSONToString(const json& json_input);
    static std::string FromJSONToFormattedString(const json& json_input);

};

} // namespace json_parser

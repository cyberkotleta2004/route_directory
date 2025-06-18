#include "json_parser.hpp"

namespace json_parser {

json JSONParser::FromStringToJSON(const std::string& input) {
    json data = json::parse(input);
    return data;
}

std::string JSONParser::FromJSONToString(const json& json_input) {
    std::string json_string = json_input.dump();
    return json_string;
}

std::string JSONParser::FromJSONToFormattedString(const json& json_input) {
    std::string json_string = json_input.dump(2);
    return json_string;
}

} // namespace json_parser



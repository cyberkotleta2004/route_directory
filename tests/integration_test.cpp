#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.hpp"
#include "../src/json_parser.hpp"
#include "../src/request_handler.hpp"
#include <fstream>

using namespace std::string_literals;

TEST_CASE("Integration tests", "[transport_catalogue]") {
    SECTION("Valid JSON") {
        transport_catalogue::TransportCatalogueRequestHandler handler;
        std::ifstream json_valid_file("tests/json_test_files/valid.json");
        json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(json_valid_file));
        std::ostringstream oss;
        oss << json_parser::JSONParser::FromJSONToString(response);
        REQUIRE(oss.str() == R"([{"buses":["114"],"request_id":1},{"curvature":1.6055733687630191,"request_id":2,"route_length":5700,"stop_count":7,"unique_stop_count":4},{"buses":["114","215"],"request_id":3},{"curvature":0.41874525297180604,"request_id":4,"route_length":8200,"stop_count":4,"unique_stop_count":3},{"buses":["215"],"request_id":5}])");
    }

    SECTION("Invalid type JSON") {
        transport_catalogue::TransportCatalogueRequestHandler handler;
        std::ifstream json_invalid_type_file("tests/json_test_files/invalid_type.json");
        json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(json_invalid_type_file));
        std::ostringstream oss;
        oss << json_parser::JSONParser::FromJSONToString(response);
        REQUIRE(oss.str() == R"({"error":"[json.exception.type_error.302] type must be number, but is string","status":"failed"})");
    }

    SECTION("No stat requests JSON") {
        transport_catalogue::TransportCatalogueRequestHandler handler;
        std::ifstream no_stat_requests_file("tests/json_test_files/no_stat_requests.json");
        json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(no_stat_requests_file));
        std::ostringstream oss;
        oss << json_parser::JSONParser::FromJSONToString(response);
        REQUIRE(oss.str() == R"({"error":"[json.exception.out_of_range.403] key 'stat_requests' not found","status":"failed"})");
    }
}
#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.hpp"
#include "../src/json_parser.hpp"
#include "../src/request_handler.hpp"
#include "sstream"


using namespace std::string_literals;

TEST_CASE("Correct data integration tests", "[transport_catalogue]") {
    SECTION("Test case 1") {
        transport_catalogue::TransportCatalogueRequestHandler handler;
        std::stringstream iss;
        iss << R"({
        "base_requests": [
            {
            "type": "Bus",
            "name": "114",
            "stops": ["Морской вокзал", "Ривьерский мост", "Центральная площадь", "Железнодорожный вокзал"],
            "is_roundtrip": false
            },
            {
            "type": "Bus",
            "name": "215",
            "stops": ["Аэропорт", "Северный район", "Центральная площадь", "Аэропорт"],
            "is_roundtrip": true
            },
            {
            "type": "Stop",
            "name": "Ривьерский мост",
            "latitude": 43.587795,
            "longitude": 39.716901,
            "road_distances": {
                "Морской вокзал": 850,
                "Центральная площадь": 1200
            }
            },
            {
            "type": "Stop",
            "name": "Морской вокзал",
            "latitude": 43.581969,
            "longitude": 39.719848,
            "road_distances": {
                "Ривьерский мост": 850,
                "Железнодорожный вокзал": 650
            }
            },
            {
            "type": "Stop",
            "name": "Центральная площадь",
            "latitude": 43.585000,
            "longitude": 39.720000,
            "road_distances": {
                "Ривьерский мост": 1200,
                "Железнодорожный вокзал": 800,
                "Аэропорт": 3500
            }
            },
            {
            "type": "Stop",
            "name": "Железнодорожный вокзал",
            "latitude": 43.580000,
            "longitude": 39.725000,
            "road_distances": {
                "Морской вокзал": 650,
                "Центральная площадь": 800
            }
            },
            {
            "type": "Stop",
            "name": "Аэропорт",
            "latitude": 43.650000,
            "longitude": 39.800000,
            "road_distances": {
                "Северный район": 1500,
                "Центральная площадь": 3500
            }
            },
            {
            "type": "Stop",
            "name": "Северный район",
            "latitude": 43.630000,
            "longitude": 39.790000,
            "road_distances": {
                "Аэропорт": 1500,
                "Центральная площадь": 3200
            }
            }
        ],
        "stat_requests": [
            { "id": 1, "type": "Stop", "name": "Ривьерский мост" },
            { "id": 2, "type": "Bus", "name": "114" },
            { "id": 3, "type": "Stop", "name": "Центральная площадь" },
            { "id": 4, "type": "Bus", "name": "215" },
            { "id": 5, "type": "Stop", "name": "Аэропорт" }
        ]
        })";
        json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(iss));
        std::ostringstream oss;
        oss << json_parser::JSONParser::FromJSONToString(response);
        REQUIRE(oss.str() == R"([{"buses":["114"],"request_id":1},{"curvature":1.6055733687630191,"request_id":2,"route_length":5700,"stop_count":7,"unique_stop_count":4},{"buses":["114","215"],"request_id":3},{"curvature":0.41874525297180604,"request_id":4,"route_length":8200,"stop_count":4,"unique_stop_count":3},{"buses":["215"],"request_id":5}])");
    }

    SECTION("Test case 2 ") {
    transport_catalogue::TransportCatalogueRequestHandler handler;
    std::stringstream iss;
    iss << R"({
        "base_requests": [
            {
                "type": "Bus",
                "name": "256",
                "stops": ["Университет", "Парк Победы", "Центральный рынок", "Стадион"],
                "is_roundtrip": false
            },
            {
                "type": "Bus",
                "name": "102",
                "stops": ["Вокзал", "Университет", "Стадион", "Вокзал"],
                "is_roundtrip": true
            },
            {
                "type": "Stop",
                "name": "Университет",
                "latitude": 55.790000,
                "longitude": 37.520000,
                "road_distances": {
                    "Парк Победы": 1200,
                    "Вокзал": 1800,
                    "Стадион": 2100
                }
            },
            {
                "type": "Stop",
                "name": "Парк Победы",
                "latitude": 55.785000,
                "longitude": 37.525000,
                "road_distances": {
                    "Университет": 1200,
                    "Центральный рынок": 900
                }
            },
            {
                "type": "Stop",
                "name": "Центральный рынок",
                "latitude": 55.780000,
                "longitude": 37.530000,
                "road_distances": {
                    "Парк Победы": 900,
                    "Стадион": 750
                }
            },
            {
                "type": "Stop",
                "name": "Стадион",
                "latitude": 55.775000,
                "longitude": 37.535000,
                "road_distances": {
                    "Центральный рынок": 750,
                    "Университет": 2100,
                    "Вокзал": 1500
                }
            },
            {
                "type": "Stop",
                "name": "Вокзал",
                "latitude": 55.800000,
                "longitude": 37.510000,
                "road_distances": {
                    "Университет": 1800,
                    "Стадион": 1500
                }
            }
        ],
        "stat_requests": [
            { "id": 1, "type": "Stop", "name": "Университет" },
            { "id": 2, "type": "Bus", "name": "256" },
            { "id": 3, "type": "Stop", "name": "Стадион" },
            { "id": 4, "type": "Bus", "name": "102" }
        ]
    })";
    
    json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(iss));
    std::ostringstream oss;
    oss << json_parser::JSONParser::FromJSONToString(response);
    
    REQUIRE(oss.str() == R"([{"buses":["256","102"],"request_id":1},{"curvature":1.4893751404306343,"request_id":2,"route_length":5700,"stop_count":7,"unique_stop_count":4},{"buses":["256","102"],"request_id":3},{"curvature":0.8466182897740046,"request_id":4,"route_length":5400,"stop_count":4,"unique_stop_count":3}])");
}
}
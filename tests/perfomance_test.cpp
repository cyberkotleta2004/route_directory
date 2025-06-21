#include "catch_amalgamated.hpp"
#include "../src/request_handler.hpp"
#include "../src/json_parser.hpp"

#include <chrono>
#include <iostream>
#include <fstream>

TEST_CASE("Performance test") {
    std::fstream input_file("tests/json_test_files/big_data.json");
    std::ofstream output_file("tests/json_test_files/output.json");

    auto start = std::chrono::high_resolution_clock::now();

    transport_catalogue::TransportCatalogueRequestHandler handler;
    json response = handler.HandleJSONBaseAndStatRequests(json_parser::JSONParser::FromStreamToJSON(input_file));

    output_file << json_parser::JSONParser::FromJSONToFormattedString(response);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Execution time: " << duration.count() << " ms\n";
    REQUIRE(duration.count() < 1000);  
}
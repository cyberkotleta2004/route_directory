#include "catch_amalgamated.hpp"
#include "../src/stat_reader.h"
#include "../src/input_reader.h"
#include "../src/transport_catalogue.h"
#include <sstream>

TEST_CASE("Test stat request reader", "[stat_reader]") {
    std::istringstream database(
        "10\n"
        "Stop Tolstopaltsevo: 55.611087, 37.208290\n"
        "Stop Marushkino: 55.595884, 37.209755\n"
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
        "Stop Rasskazovka: 55.632761, 37.333324\n"
        "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n"
        "Stop Biryusinka: 55.581065, 37.648390\n"
        "Stop Universam: 55.587655, 37.645687\n"
        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
    );

    std::istringstream stat_requests(
        "3\n"
        "Bus 256\n"
        "Bus 750\n"
        "Bus 751\n"
    );

    auto catalogue = tcat::ReadTransportCatalogueCreateRequests(database);

    std::ostringstream oss;
    tcat::ParseStatRequests(catalogue, stat_requests, oss);
    std::string result = oss.str();

    REQUIRE(result == "Bus 256: 6 stops on route, 5 unique stops, 4371.017261 route length"
                       "Bus 750: 5 stops on route, 3 unique stops, 20939.483047 route length"
                       "Bus 751: not found");
}
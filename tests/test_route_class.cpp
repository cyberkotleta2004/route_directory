#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.h"
#include <limits>

constexpr double EPS = 1e-3;

TEST_CASE("Class Route creating correctly", "[route]") {
    std::string name = "295";
    std::deque<transport_catalogue::Stop> stops = {
        {"Volgina 4", {55.7512, 37.6981}},
        {"Novatorskaya", {55.7112, 37.6971}},
        {"Lenina", {55.7311, 36.6982}},
        {"Dyatlova 5", {55.7422, 35.6211}},
    };

    std::vector<const transport_catalogue::Stop*> stops_ptrs = {
        &stops[0], &stops[1], &stops[2], &stops[3], &stops[0]};

    transport_catalogue::Route route(name, std::move(stops_ptrs));

    REQUIRE(route.GetName() == "295");
    REQUIRE(route.GetStopsCount() == 5);
    REQUIRE(route.GetUniqueStopsCount() == 4);
    double expected = 264475.8154645019;
    REQUIRE(std::abs(route.GetLength() - expected) < EPS);
}




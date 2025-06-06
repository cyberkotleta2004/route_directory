#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.h"
#include <limits>

using namespace std::string_literals;

constexpr double EPS = 1e-3;

TEST_CASE("Class Route creating correctly", "[route]") {
    std::deque<tcat::Stop> stops = {
        {"Volgina 4"s, {55.7512, 37.6981}},
        {"Novatorskaya"s, {55.7112, 37.6971}},
        {"Lenina"s, {55.7311, 36.6982}},
        {"Dyatlova 5"s, {55.7422, 35.6211}},
    };

    std::vector<const tcat::Stop*> stops_ptrs = {
        &stops[0], &stops[1], &stops[2], &stops[3], &stops[0]};

    tcat::Route route("295"s, std::move(stops_ptrs));

    REQUIRE(route.GetName() == "295");
    REQUIRE(route.GetStopsCount() == 5);
    REQUIRE(route.GetUniqueStopsCount() == 4);
    double expected = 264475.8154645019;
    REQUIRE(std::abs(route.GetLength() - expected) < EPS);
}




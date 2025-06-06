#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.h"

using namespace std::string_literals;


TEST_CASE("Class Transport catalogue works correctly", "[transport_catalogue]") {
    std::deque<tcat::Stop> stops = {
        {"Volgina 4"s, {55.7512, 37.6981}},
        {"Novatorskaya"s, {55.7112, 37.6971}},
        {"Lenina"s, {55.7311, 36.6982}},
        {"Dyatlova 5"s, {55.7422, 35.6211}},
    };

    std::vector<const tcat::Stop*> stops_ptrs = {
        &stops[0], &stops[1], &stops[2], &stops[3], &stops[0]};

    tcat::Route route("1"s, std::move(stops_ptrs));

    tcat::TransportCatalogue transport_catalogue;

    for(auto& stop : stops) {
        transport_catalogue.AddStop(std::move(stop));
    }

    transport_catalogue.AddRoute(std::move(route));

    REQUIRE(transport_catalogue.GetStop("Lenina"s).name_ == "Lenina"s);
    REQUIRE_THROWS_AS(transport_catalogue.GetStop("Tushino"s), std::out_of_range);
    REQUIRE(transport_catalogue.GetRoute("1"s).GetName() == "1");
    REQUIRE_THROWS_AS(transport_catalogue.GetRoute("100"s), std::out_of_range);
    REQUIRE(transport_catalogue.GetRouteInfo("1"s) == 
        "Bus 1: 5 stops on route, 4 unique stops, 264475.815467 route length\n"s);
    REQUIRE(transport_catalogue.GetRouteInfo("100"s) == "Bus 100: not found\n"s);
}
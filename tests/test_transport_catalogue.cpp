#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.h"

TEST_CASE("Class Transport catalogue works correctly", "[transport_catalogue]") {
    std::deque<transport_catalogue::Stop> stops = {
        {"Volgina 4", {55.7512, 37.6981}},
        {"Novatorskaya", {55.7112, 37.6971}},
        {"Lenina", {55.7311, 36.6982}},
        {"Dyatlova 5", {55.7422, 35.6211}},
    };

    std::vector<const transport_catalogue::Stop*> stops_ptrs = {
        &stops[0], &stops[1], &stops[2], &stops[3], &stops[0]};

    transport_catalogue::Route route("1", std::move(stops_ptrs));

    transport_catalogue::TransportCatalogue transport_catalogue;

    for(auto& stop : stops) {
        transport_catalogue.AddStop(std::move(stop));
    }

    transport_catalogue.AddRoute(std::move(route));

    REQUIRE(transport_catalogue.GetStop("Lenina").name_ == "Lenina");
    REQUIRE_THROWS_AS(transport_catalogue.GetStop("Tushino"), std::out_of_range);
    REQUIRE(transport_catalogue.GetRoute("1").GetName() == "1");
    REQUIRE_THROWS_AS(transport_catalogue.GetRoute("100"), std::out_of_range);
    REQUIRE(transport_catalogue.GetRouteInfo("1") == 
        "Bus 1: 5 stops on route, 4 unique stops, 264475.815467 route length\n");
    REQUIRE(transport_catalogue.GetRouteInfo("100") == "Bus 100: not found\n");
}
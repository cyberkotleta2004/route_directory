#include "catch_amalgamated.hpp"
#include "../src/input_reader.h"
#include "../src/transport_catalogue.h"
#include <sstream>

using namespace std::string_view_literals;

TEST_CASE("Test stream input", "[input_reader]") {
    std::istringstream iss(
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

    auto catalogue = transport_catalogue::ReadTransportCatalogueCreateRequests(iss);

    SECTION("GetStop return correct coordinates") {
        const auto& stop = catalogue.GetStop("Tolstopaltsevo"sv);
        REQUIRE(stop.name_ == "Tolstopaltsevo");
        REQUIRE(stop.coordinates_.lat == Catch::Approx(55.611087).epsilon(1e-3));
        REQUIRE(stop.coordinates_.lng == Catch::Approx(37.208290).epsilon(1e-3));
    }

    SECTION("GetStop throws for nonexistent stop") {
        REQUIRE_THROWS_AS(catalogue.GetStop("InvalidStop"sv), std::out_of_range);
    }

    SECTION("GetRoute 750 parsed correctly") {
        const auto& route = catalogue.GetRoute("750");
        REQUIRE(route.GetName() == "750");
        REQUIRE(route.GetStopsCount() == 3);
        REQUIRE(route.GetUniqueStopsCount() == 3);
    }

    SECTION("GetRoute 256 parsed correctly") {
        const auto& route = catalogue.GetRoute("256");
        REQUIRE(route.GetStopsCount() == 6);
        REQUIRE(route.GetUniqueStopsCount() == 5); 
    }

    SECTION("Route info for 750 is formatted") {
        const auto info = catalogue.GetRouteInfo("750");
        REQUIRE(info.starts_with("Bus 750: 3 stops on route"));
    }

    SECTION("Route info for non-existent bus returns not found") {
        const auto info = catalogue.GetRouteInfo("999");
        REQUIRE(info == "Bus 999: not found\n");
    }

    SECTION("GetRoute throws on invalid name") {
        REQUIRE_THROWS_AS(catalogue.GetRoute("XXX"), std::out_of_range);
    }
}
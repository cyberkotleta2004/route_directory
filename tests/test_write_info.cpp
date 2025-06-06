#include "catch_amalgamated.hpp"
#include "../src/transport_catalogue.h"
#include "../src/stat_reader.h"
#include "../src/input_reader.h"

TEST_CASE("Test write info prints correctly", "[write_info]") {
    std::istringstream database_requests_s(
        "13\n"
        "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
        "Stop Marushkino: 55.595884, 37.209755\n"
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
        "Stop Rasskazovka: 55.632761, 37.333324\n"
        "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
        "Stop Biryusinka: 55.581065, 37.64839\n"
        "Stop Universam: 55.587655, 37.645687\n"
        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
        "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
        "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
        "Stop Prazhskaya: 55.611678, 37.603831\n"
    );

    std::istringstream requests_to_database(
        "6\n"
        "Bus 256\n"
        "Bus 750\n"
        "Bus 751\n"
        "Stop Samara\n"
        "Stop Prazhskaya\n"
        "Stop Biryulyovo Zapadnoye\n"
    );

    std::ostringstream result;

    tcat::TransportCatalogue catalogue = tcat::ReadTransportCatalogueCreateRequests(database_requests_s);
    tcat::WriteInfo(catalogue, requests_to_database, result);

    REQUIRE(result.str() == "Bus 256: 6 stops on route, 5 unique stops, 4371.017261 route length\n"
                            "Bus 750: 5 stops on route, 3 unique stops, 20939.483047 route length\n"
                            "Bus 751: not found\n"
                            "Stop Samara: not found\n"
                            "Stop Prazhskaya: no buses\n"
                            "Stop Biryulyovo Zapadnoye: buses 256 828\n");
}
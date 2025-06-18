#pragma once
#include "transport_catalogue.hpp"
#include "json_parser.hpp"

/*
todo:
    change .at() to []
    change Route to Bus name
*/
namespace transport_catalogue {

class TransportCatalogueRequestHandler {
public:
    json HandleJSONBaseAndStatRequests(const json& json_input);
    void ParseJSONBaseRequests(const json& base_requests);
    json ParseJSONStatRequests(const json& base_requests);
    void AddStopFromJSON(const json& stop_json);
    void AddBusFromJSON(const json& bus_json);

private:
    TransportCatalogue transport_catalogue_;
};

} // namespace transport_catalogue
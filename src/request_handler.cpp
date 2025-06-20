#include "request_handler.hpp"
#include <string>
#include <tuple>

using namespace std::string_literals;

namespace transport_catalogue {

json TransportCatalogueRequestHandler::HandleJSONBaseAndStatRequests(const json& json_input) {
    try {
        ParseJSONBaseRequests(json_input.at("base_requests"));
        return ParseJSONStatRequests(json_input.at("stat_requests"));

    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return json{
            {"error", e.what()},
            {"status", "failed"}
        };
    } 
}

json TransportCatalogueRequestHandler::ParseJSONStatRequests(const json& stat_requests) {
    json responses = json::array();

    for(const auto& request : stat_requests) {
        json response;
        response["request_id"] = request.at("id");

        const std::string type = request.at("type");
        const std::string name = request.at("name");

        if(type == "Bus") {
            try {
                response["curvature"] = transport_catalogue_.GetBusCurvature(name);
                response["route_length"] = transport_catalogue_.GetBusLength(name);
                response["stop_count"] = transport_catalogue_.GetStopsCountOnBus(name);
                response["unique_stop_count"] = transport_catalogue_.GetUniqueStopsCountOnBus(name);

            } catch(const std::out_of_range& e) {
                response["error_message"] = "not found";
                responses.push_back(response);
                continue;
            }
        }
        else if(type == "Stop") {
            try {
                response["buses"] = transport_catalogue_.GetBusesGoesThroughStop(name);
            } catch(const std::out_of_range& e) {
                response["error_message"] = "not found";
                responses.push_back(response);
                continue;
            }
        }
        
        responses.push_back(response);
    }

    return responses;
}

void TransportCatalogueRequestHandler::ParseJSONBaseRequests(const json& base_requests) {
    std::vector<std::tuple<std::string, std::string, size_t>> distances;
    std::vector<json> buses;

    for(const auto& request : base_requests) {
        std::string type = request.at("type").get<std::string>();

        if(type == "Stop"s) {
            AddStopFromJSON(request);

            for(const auto& [to, dist] : request.at("road_distances").items()) {
                distances.emplace_back(
                    request.at("name").get<std::string>(),
                    to,
                    dist.get<size_t>()
                );
            }
        }
        else if (type == "Bus") {
            buses.push_back(request);
        }
    }

    for (const auto& [from, to, dist] : distances) {
        transport_catalogue_.SetDistanceBetweenStops(from, to, dist);
    }

    for (const auto& bus : buses) {
        AddBusFromJSON(bus);
    }
}

void TransportCatalogueRequestHandler::AddStopFromJSON(const json& stop_json) {
    std::string name = stop_json.at("name").get<std::string>();
    double lat = stop_json.at("latitude").get<double>();
    double lng = stop_json.at("longitude").get<double>();
    
    transport_catalogue_.AddStop({std::move(name), {lat, lng}});
}

void TransportCatalogueRequestHandler::AddBusFromJSON(const json& bus_json) {
    std::string name = bus_json.at("name").get<std::string>();
    std::vector<const Stop*> stops;
    
    for(const auto& stop_name : bus_json.at("stops")) {
        const Stop* stop_ptr = transport_catalogue_.GetStopPtr(stop_name.get<std::string>());
        stops.push_back(stop_ptr);
    }

    if(bus_json.at("is_roundtrip").get<bool>() == false) {
        stops.reserve(stops.size() * 2 - 1);
        
        for(size_t i = stops.size() - 1; i > 0; --i) {
            stops.push_back(stops[i - 1]);
        }
    }

    transport_catalogue_.AddBus({std::move(name), std::move(stops)});
}

}; // namespace transport_catalogue
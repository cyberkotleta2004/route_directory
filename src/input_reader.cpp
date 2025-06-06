#include "input_reader.h"
#include <vector>
#include <charconv>
#include "geo.h"

using namespace std::string_literals;

namespace transport_catalogue {
    TransportCatalogue ReadTransportCatalogueCreateRequests(std::istream& in) {
        size_t request_count;
        in >> request_count;
        in.ignore();
        
        std::vector<std::string> stops_create_requests;
        std::vector<std::string> routes_create_requests;

        std::string current_request;
        for(size_t i = 0; i < request_count; ++i) {
            std::getline(in, current_request);
            if(current_request.starts_with("Stop"s)) {
                stops_create_requests.push_back(std::move(current_request));
            } else if (current_request.starts_with("Bus"s)) {
                routes_create_requests.push_back(std::move(current_request));
            }
        }

        TransportCatalogue transport_catalogue;
        AddStopsFromRequests(transport_catalogue, stops_create_requests);
        AddRoutesFromRequests(transport_catalogue, routes_create_requests);

        return transport_catalogue;
    }

    void AddStopsFromRequests(
        TransportCatalogue& transport_catalogue, 
        const std::vector<std::string>& requests
    ) {
        for(const std::string& str : requests) {
            transport_catalogue.AddStop(CreateStopByRequest(std::string_view(str)));
        }
    }

    void AddRoutesFromRequests(
        TransportCatalogue& transport_catalogue, 
        const std::vector<std::string>& requests
    ) {
        for(const std::string& str : requests) {
            transport_catalogue.AddRoute(CreateRouteByRequest(transport_catalogue, std::string_view(str)));
        }
    }

    Stop CreateStopByRequest(std::string_view request) {
        std::string_view name;
        double lat, lng;

        request.remove_prefix(5);
        size_t colon_pos = request.find_first_of(":");
        name = request.substr(0, colon_pos);
        request.remove_prefix(name.size() + 2);

        std::string_view lat_sv;
        size_t comma_pos = request.find_first_of(",");
        lat_sv = request.substr(0, comma_pos);
        std::from_chars(lat_sv.data(), lat_sv.data() + lat_sv.size(), lat);
        request.remove_prefix(lat_sv.size() + 2);

        std::string_view lng_sv;
        lng_sv = request.substr(0);
        std::from_chars(lng_sv.data(), lng_sv.data() + lng_sv.size(), lng);
        request.remove_prefix(lng_sv.size() + 2);
        
        return Stop{name, {lat, lng}};
    }

    Route CreateRouteByRequest(TransportCatalogue& transport_catalogue, std::string_view request) {
        std::string_view name;
        std::vector<const Stop*> stop_ptrs;

        request.remove_prefix(4);
        size_t colon_pos = request.find_first_of(":");
        name = request.substr(0, colon_pos);
        request.remove_prefix(name.size() + 2);

        size_t stop_delim_pos = request.find_first_of(">-");
        bool is_returning_route = request[stop_delim_pos] == '-' ? true : false;
        
        std::string_view current_stop;
        while(true) {
            size_t delim_pos = request.find_first_of(">-");

            if(delim_pos == std::string::npos) {
                const Stop* stop_ptr = &transport_catalogue.GetStop(request.substr(0, request.size()));
                stop_ptrs.push_back(stop_ptr);
                break;
            }
            
            current_stop = request.substr(0, delim_pos - 1);

            try {
                const Stop* stop_ptr = &transport_catalogue.GetStop(current_stop);
                stop_ptrs.push_back(stop_ptr);
            } catch(const std::out_of_range& except) {
                throw;
            }
            request.remove_prefix(current_stop.size() + 3);
        }

        if(is_returning_route) {
            stop_ptrs.reserve(stop_ptrs.size() * 2 - 1);

            for(int i = stop_ptrs.size() - 2; i >= 0; --i) {
                stop_ptrs.push_back(stop_ptrs[i]);
            }
        }

        return Route{name, std::move(stop_ptrs)};
    }
}

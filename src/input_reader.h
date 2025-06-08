#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace tcat {
    TransportCatalogue ReadTransportCatalogueCreateRequests(std::istream& in = std::cin);

    namespace detail {
        void AddStopsFromRequests(
            TransportCatalogue& transport_catalogue, 
            const std::vector<std::string>& requests
        );
    
        void AddRoutesFromRequests(
            TransportCatalogue& transport_catalogue, 
            const std::vector<std::string>& requests
        );

        void ParseAndSetDistancesByRequest(
            std::string_view request_sv,
            TransportCatalogue& transport_catalogue, 
            const std::vector<std::pair<std::string, size_t>>& stop_names_to_tails,
            size_t current_name_and_tail_index
        );
    
        std::pair<Stop, size_t> ParseStopAndTailByRequest(std::string_view request_sv);

        Route CreateRouteByRequest(TransportCatalogue& transport_catalogue, std::string_view request_sv);
    }
}
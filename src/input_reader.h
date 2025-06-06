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
    
        Stop CreateStopByRequest(std::string_view request_sv);
        Route CreateRouteByRequest(TransportCatalogue& transport_catalogue, std::string_view request_sv);
    }
}
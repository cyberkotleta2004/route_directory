#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace transport_catalogue {
    TransportCatalogue ReadTransportCatalogueCreateRequests(std::istream& in = std::cin);

    void AddStopsFromRequests(
        TransportCatalogue& transport_catalogue, 
        const std::vector<std::string>& requests
    );

    void AddRoutesFromRequests(
        TransportCatalogue& transport_catalogue, 
        const std::vector<std::string>& requests
    );

    Stop CreateStopByRequest(std::string_view request);
    Route CreateRouteByRequest(TransportCatalogue& transport_catalogue, std::string_view request);
}
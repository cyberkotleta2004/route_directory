#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace transport_catalogue {
    TransportCatalogue ReadTransportCatalogueCreateRequests(std::istream& in = std::cin);

    Stop CreateRouteByRequest(std::string_view request);
    Route CreateStopByRequest(std::string_view request);
}
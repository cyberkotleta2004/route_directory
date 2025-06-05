#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace transport_catalogue {
    void ReadStatRequests(TransportCatalogue& catalogue, std::istream& in = std::cin, std::ostream& out = std::cout);
}
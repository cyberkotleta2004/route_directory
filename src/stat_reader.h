#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace tcat {
    void ParseStatRequests(TransportCatalogue& catalogue, std::istream& in = std::cin, std::ostream& out = std::cout);
}
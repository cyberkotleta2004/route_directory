#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace tcat {
    void GetRoutesStat(TransportCatalogue& catalogue, std::istream& in = std::cin, std::ostream& = std::cout);

    namespace details {
        std::vector<std::string> ParseStatRequests(std::istream& in);
        void WriteStatRequestsResult(
            TransportCatalogue& catalogue, 
            const std::vector<std::string>& requests, 
            std::ostream& out
        );
    }
}
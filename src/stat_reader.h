#pragma once
#include <iostream>
#include "transport_catalogue.h"

namespace tcat {
    void WriteInfo(TransportCatalogue& catalogue, std::istream& in = std::cin, std::ostream& = std::cout);

    namespace details {
        std::vector<std::string> ReadRequests(std::istream& in);

        void WriteRequestsResult(
            TransportCatalogue& catalogue, 
            const std::vector<std::string>& requests, 
            std::ostream& out
        );

        std::string GetBusInfo(TransportCatalogue& catalogue, std::string_view request_sv);
        std::string GetStopInfo(TransportCatalogue& catalogue, std::string_view request_sv);
    }
}
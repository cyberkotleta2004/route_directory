#include "stat_reader.h"

namespace tcat {
    void ParseStatRequests(TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
        size_t request_count = 0;
        in >> request_count;
        in.ignore();
    
        std::vector<std::string> requests;
        requests.reserve(request_count);

        std::string request;
        for (size_t i = 0; i < request_count; ++i) {
            std::getline(in, request);
            requests.push_back(std::move(request));
        }

        for (std::string_view req : requests) {
            if (req.starts_with("Bus ")) {
                req.remove_prefix(4);
                out << catalogue.GetRouteInfo(req) << '\n';
            }
        }
    }
}



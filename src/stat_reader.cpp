#include "stat_reader.h"

namespace transport_catalogue {
    void ReadStatRequests(TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
        size_t request_count = 0;
        in >> request_count;
        in.ignore();
    
        std::string current_request;
        for(size_t i = 0; i < request_count; ++i) {
            std::getline(in, current_request);
            std::string_view current_request_sv(current_request);
            current_request_sv.remove_prefix(4);
            out << catalogue.GetRouteInfo(current_request_sv);
        }
    }
}



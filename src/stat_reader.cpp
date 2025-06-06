#include "stat_reader.h"

namespace tcat {
    void GetRoutesStat(TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
        details::WriteStatRequestsResult(catalogue, details::ParseStatRequests(in), out);
    }
    
    namespace details {
        std::vector<std::string> ParseStatRequests(std::istream& in) {
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
    
            return requests;
        }

        void WriteStatRequestsResult(TransportCatalogue& catalogue, const std::vector<std::string>& requests, std::ostream& out) {
            for (std::string_view req : requests) {
                req.remove_prefix(4);
                out << catalogue.GetRouteInfo(req) << '\n';
            }
        }
    }
}



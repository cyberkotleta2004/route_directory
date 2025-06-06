#include "stat_reader.h"

namespace tcat {
    void WriteInfo(TransportCatalogue& catalogue, std::istream& in, std::ostream& out) {
        details::WriteRequestsResult(catalogue, details::ReadRequests(in), out);
    }
    
    namespace details {
        std::vector<std::string> ReadRequests(std::istream& in) {
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

        

        void WriteRequestsResult(TransportCatalogue& catalogue, const std::vector<std::string>& requests, std::ostream& out) {
            for(std::string_view request_sv : requests) {
                if(request_sv.starts_with("Bus")) {
                    out << GetBusInfo(catalogue, request_sv) << '\n';
                } else if(request_sv.starts_with("Stop")) {
                    out << GetStopInfo(catalogue, request_sv) << '\n';
                }
            }
        }

        std::string GetBusInfo(TransportCatalogue& catalogue, std::string_view request_sv) {
            request_sv.remove_prefix(4);
            return catalogue.GetRouteInfo(request_sv);
        }

        std::string GetStopInfo(TransportCatalogue& catalogue, std::string_view request_sv) {
            request_sv.remove_prefix(5);
            return catalogue.GetRoutesGoesThroughStop(request_sv);
        }
    }
}



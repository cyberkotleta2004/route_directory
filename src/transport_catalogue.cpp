#include "transport_catalogue.h"
#include <unordered_set>
#include <stdexcept>
#include <format>

using namespace std::string_literals;

namespace tcat {
    void TransportCatalogue::AddStop(Stop&& stop) {
        stops_.push_back(std::move(stop));
        stop_name_to_stop_.insert({stops_.back().name_, &stops_.back()});
    }

    void TransportCatalogue::AddRoute(Route&& route) {
        std::string route_name(route.GetName());

        routes_.push_back(std::move(route));

        const Route* current_route_ptr = &routes_.back();
        route_name_to_route_.insert({current_route_ptr->GetName(), current_route_ptr});


        for(const Stop* ptr : current_route_ptr->GetStopsPtrs()) {
            stop_ptr_to_routes_ptrs_[ptr].insert(current_route_ptr);
        }
    }

    const Stop& TransportCatalogue::GetStop(std::string_view stop_name_sv) const {
        return *stop_name_to_stop_.at(stop_name_sv);
    }

    const Route& TransportCatalogue::GetRoute(std::string_view route_name_sv) const {
        return *route_name_to_route_.at(route_name_sv);
    }

    std::string TransportCatalogue::GetRouteInfo(std::string_view route_name_sv) const {
        try {
            const Route& route_ref = GetRoute(route_name_sv);
            return std::format(
                "Bus {}: {} stops on route, {} unique stops, {:.6f} route length",
                route_ref.GetName(), 
                route_ref.GetStopsCount(), 
                route_ref.GetUniqueStopsCount(),
                route_ref.GetLength()
            );
        } catch(const std::out_of_range& except) {
            return std::format("Bus {}: not found", route_name_sv);
        }
    }

    std::string TransportCatalogue::GetRoutesGoesThroughStop(std::string_view stop_name_sv) const {
        try {
            const Stop* stop_ptr = &GetStop(stop_name_sv);

            if(stop_ptr_to_routes_ptrs_.contains(stop_ptr)) {
                std::set<const Route*> routes_ptrs_set = stop_ptr_to_routes_ptrs_.at(stop_ptr);
                std::string routes_info = std::format(
                    "Stop {}: buses", stop_name_sv
                );
                
                for(const Route* route : routes_ptrs_set) {
                    routes_info.push_back(' ');
                    routes_info += std::string(route->GetName());
                }

                return routes_info;
            } else {
                return std::format(
                    "Stop {}: no buses",
                    stop_name_sv
                );
            }
            
        } catch(const std::out_of_range& except) {
            return std::format("Stop {}: not found", stop_name_sv);
        }
    }

}
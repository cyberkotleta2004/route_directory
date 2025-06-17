#include "transport_catalogue.h"
#include <unordered_set>
#include <stdexcept>
#include <format>

using namespace std::string_literals;

namespace transport_catalogue {

namespace detail {
    size_t StopPtrPairHasher::operator()(const std::pair<const Stop*, const Stop*>& p) const {
        std::hash<const Stop*> hasher;
        size_t h1 = hasher(p.first);
        size_t h2 = hasher(p.second);
        return h1 * 37 + h2;
    }
}

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

void TransportCatalogue::SetDistanceBetweenStops(const Stop* first_stop, const Stop* second_stop, size_t distance) {
    stops_pair_to_distance_[{first_stop, second_stop}] = distance;

    if(!stops_pair_to_distance_.contains({second_stop, first_stop})) {
        stops_pair_to_distance_[{second_stop, first_stop}] = distance;
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
            "Bus {}: {} stops on route, {} unique stops, {} route length, {:.6f} curvature",
            route_ref.GetName(), 
            route_ref.GetStopsCount(), 
            route_ref.GetUniqueStopsCount(),
            GetRouteRealLength(route_ref),
            GetRouteCurvature(route_ref)
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

double TransportCatalogue::GetRouteGeographicLength(const Route& route) const {
    double route_geographic_length = 0.0;

    auto stops_ptrs = route.GetStopsPtrs();

    for(int i = 1; i < stops_ptrs.size(); ++i) {
        route_geographic_length += geo::ComputeDistance(stops_ptrs[i - 1]->coordinates_, stops_ptrs[i]->coordinates_);
    }

    return route_geographic_length;
}

size_t TransportCatalogue::GetRouteRealLength(const Route& route) const {
    size_t route_real_length = 0;

    auto stop_ptrs = route.GetStopsPtrs();

    for(int i = 0; i < stop_ptrs.size() - 1; ++i) {
        route_real_length += stops_pair_to_distance_.at({stop_ptrs[i], stop_ptrs[i + 1]});
    }

    return route_real_length;
}

double TransportCatalogue::GetRouteCurvature(const Route& route) const {
    double curvature;
    double route_geographic_length = GetRouteGeographicLength(route);
    size_t route_real_length = GetRouteRealLength(route);

    curvature = static_cast<double>(route_real_length) / route_geographic_length;
    return curvature;
}

} // namespace transport_catalogue
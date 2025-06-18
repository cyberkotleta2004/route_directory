#include "transport_catalogue.hpp"
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

void TransportCatalogue::SetDistanceBetweenStops(const std::string& first_stop_name, const std::string& second_stop_name, size_t distance) {
    const Stop* first_stop = GetStopPtr(first_stop_name);
    const Stop* second_stop = GetStopPtr(second_stop_name);

    SetDistanceBetweenStops(first_stop, second_stop, distance);
}

const Stop* TransportCatalogue::GetStopPtr(std::string_view stop_name_sv) const {
    return stop_name_to_stop_.at(stop_name_sv);
}

const Route* TransportCatalogue::GetRoutePtr(std::string_view route_name_sv) const {
    return route_name_to_route_.at(route_name_sv);
}


const Stop& TransportCatalogue::GetStop(std::string_view stop_name_sv) const {
    return *stop_name_to_stop_.at(stop_name_sv);
}

const Route& TransportCatalogue::GetRoute(std::string_view route_name_sv) const {
    return *route_name_to_route_.at(route_name_sv);
}

size_t TransportCatalogue::GetStopsCountOnRoute(std::string_view route_name_sv) const {
    const Route* route_ptr = route_name_to_route_.at(route_name_sv);
    size_t stops_count = route_ptr->GetStopsCount();
    return stops_count;
}

size_t TransportCatalogue::GetUniqueStopsCountOnRoute(std::string_view route_name_sv) const {
    const Route* route_ptr = route_name_to_route_.at(route_name_sv);
    size_t unique_stops_count = route_ptr->GetUniqueStopsCount();
    return unique_stops_count;
}

size_t TransportCatalogue::GetRouteLength(std::string_view route_name_sv) const {
    const Route* route_ptr = route_name_to_route_.at(route_name_sv);
    size_t route_length = CountRouteRealLength(route_ptr);
    return route_length;
}

double TransportCatalogue::GetRouteCurvature(std::string_view route_name_sv) const {
    const Route* route_ptr = route_name_to_route_.at(route_name_sv);

    double route_geographic_length = CountRouteGeographicLength(route_ptr);
    size_t route_real_length = CountRouteRealLength(route_ptr);
    double curvature = static_cast<double>(route_real_length) / route_geographic_length;

    return curvature;
}


std::vector<std::string> TransportCatalogue::GetRoutesGoesThroughStop(std::string_view stop_name_sv) const {
    const Stop* stop_ptr = stop_name_to_stop_.at(stop_name_sv);
    std::set<const Route*> routes_ptrs = stop_ptr_to_routes_ptrs_.at(stop_ptr);

    std::vector<std::string> routes_names;

    for(const Route* route_ptr : routes_ptrs) {
        routes_names.push_back(std::string(route_ptr->GetName()));
    }

    return routes_names;
}

double TransportCatalogue::CountRouteGeographicLength(const Route* route) const {
    double route_geographic_length = 0.0;

    auto stops_ptrs = route->GetStopsPtrs();

    for(int i = 1; i < stops_ptrs.size(); ++i) {
        route_geographic_length += geo::ComputeDistance(stops_ptrs[i - 1]->coordinates_, stops_ptrs[i]->coordinates_);
    }

    return route_geographic_length;
}

size_t TransportCatalogue::CountRouteRealLength(const Route* route) const {
    size_t route_real_length = 0;

    auto stop_ptrs = route->GetStopsPtrs();

    for(int i = 0; i < stop_ptrs.size() - 1; ++i) {
        route_real_length += stops_pair_to_distance_.at({stop_ptrs[i], stop_ptrs[i + 1]});
    }

    return route_real_length;
}

} // namespace transport_catalogue
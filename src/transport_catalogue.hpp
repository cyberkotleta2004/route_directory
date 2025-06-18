#pragma once
#include <deque>
#include <unordered_map>
#include <set>
#include <utility>
#include <string>
#include "stop.hpp"
#include "route.hpp"

namespace transport_catalogue {

namespace detail {
    struct StopPtrPairHasher {
        size_t operator()(const std::pair<const Stop*, const Stop*>& p) const;
    };
}

class TransportCatalogue {
private:
    std::deque<Stop> stops_;
    std::unordered_map<std::string_view, const Stop*> stop_name_to_stop_;
    std::deque<Route> routes_;
    std::unordered_map<std::string_view, const Route*> route_name_to_route_;
    std::unordered_map<const Stop*, std::set<const Route*>> stop_ptr_to_routes_ptrs_;
    std::unordered_map<std::pair<const Stop*, const Stop*>, size_t, detail::StopPtrPairHasher> stops_pair_to_distance_;

public:
    void AddStop(Stop&& stop);
    void AddRoute(Route&& route);
    void SetDistanceBetweenStops(const Stop* first_stop, const Stop* second_stop, size_t distance);
    void SetDistanceBetweenStops(const std::string& first_stop_name, const std::string& second_stop, size_t distance);

    const Stop* GetStopPtr(std::string_view stop_name_sv) const;
    const Route* GetRoutePtr(std::string_view route_name_sv) const;
    const Stop& GetStop(std::string_view stop_name_sv) const;
    const Route& GetRoute(std::string_view route_name_sv) const;
    size_t GetStopsCountOnRoute(std::string_view route_name_sv) const;
    size_t GetUniqueStopsCountOnRoute(std::string_view route_name_sv) const;
    size_t GetRouteLength(std::string_view route_name_sv) const;
    double GetRouteCurvature(std::string_view route_name_sv) const;
    std::vector<std::string> GetRoutesGoesThroughStop(std::string_view stop_name_sv) const;

private:
    double CountRouteGeographicLength(const Route* route) const;
    size_t CountRouteRealLength(const Route* route) const;
};

} // namespace transport_catalogue

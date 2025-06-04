#include "transport_catalogue.h"
#include <unordered_set>

namespace transport_catalogue {
    Route::Route(std::string_view name, std::vector<const Stop*>&& route)
        : name_(name)
        , stops_count_(route.size() + 1)
        , unique_stops_count_(CountUniqueStopsCount(route))
        , route_length_(CountRouteLength(route))
        , route_(std::move(route))
    {}

    double Route::GetRouteLength() const noexcept {
        return route_length_;
    }

    size_t Route::GetStopsCount() const noexcept {
        return stops_count_;
    }

    size_t Route::GetUniqueStopsCount() const noexcept {
        return unique_stops_count_;
    }

    size_t Route::CountUniqueStopsCount(const std::vector<const Stop*>& route) const {
        size_t unique_stops_counter = 0;
        std::unordered_set<const Stop*> unique_stops;

        for(const Stop* stop_ptr : route) {
            if(unique_stops.contains(stop_ptr)) return unique_stops_counter;
            unique_stops.insert(stop_ptr);
            ++unique_stops_counter;
        }
        return unique_stops_counter;
    }

    double Route::CountRouteLength(const std::vector<const Stop*>& route) const {
        double route_length = 0.0;

        for(size_t i = 0; i < route.size() - 1; ++i) {
            route_length += geo::ComputeDistance(route[i]->coordinates_, route[i + 1]->coordinates_);
        }

        return route_length;
    }
}
#include "route.h"
#include <unordered_set>

namespace transport_catalogue {
    Route::Route(std::string_view name, std::vector<const Stop*>&& route)
        : name_(name)
        , unique_stops_count_(CountUniqueStopsCount(route))
        , stops_count_(route.size())
        , route_length_(CountLength(route))
        , route_(std::move(route))
    {}

    std::string_view Route::GetName() const noexcept {
        return name_;
    }
 
    double Route::GetLength() const noexcept {
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

    double Route::CountLength(const std::vector<const Stop*>& route) const {
        double route_length = 0.0;

        for(size_t i = 1; i < route.size(); ++i) {
            route_length += geo::ComputeDistance(route[i - 1]->coordinates_, route[i]->coordinates_);
        }

        return route_length;
    }
}
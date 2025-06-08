#include "route.h"
#include <unordered_set>

namespace tcat {
    // Continue here
    Route::Route(std::string_view name_sv, std::vector<const Stop*>&& stops)
        : name_(name_sv)
        , stops_count_(stops.size())
        , unique_stops_count_(CountUniqueStopsCount(stops))
        , stops_(std::move(stops))
    {}

    std::string_view Route::GetName() const noexcept {
        std::string_view name_sv(name_);
        return name_sv;
    }

    size_t Route::GetStopsCount() const noexcept {
        return stops_count_;
    }

    size_t Route::GetUniqueStopsCount() const noexcept {
        return unique_stops_count_;
    }

    const std::vector<const Stop*>& Route::GetStopsPtrs() const {
        return stops_;
    }

    size_t Route::CountUniqueStopsCount(const std::vector<const Stop*>& stops) const {
        size_t unique_stops_counter = 0;
        std::unordered_set<const Stop*> unique_stops;

        for(const Stop* stop_ptr : stops) {
            if(!unique_stops.contains(stop_ptr)) {
                unique_stops.insert(stop_ptr);
                ++unique_stops_counter;
            }
        }
        return unique_stops_counter;
    }
}
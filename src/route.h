#pragma once
#include <string>
#include <vector>
#include "stop.h"

namespace transport_catalogue {
    class Route {
    private:
        std::string name_;
        size_t stops_count_;
        size_t unique_stops_count_;
        double route_length_;
        std::vector<const Stop*> route_;
    public: 
        Route(const std::string& name, std::vector<const Stop*>&& route);
        
        std::string_view GetName() const noexcept;
        double GetLength() const noexcept;
        size_t GetStopsCount() const noexcept;
        size_t GetUniqueStopsCount() const noexcept;
    private:
        size_t CountUniqueStopsCount(const std::vector<const Stop*>& route) const;
        double CountLength(const std::vector<const Stop*>& route) const;
    };
}
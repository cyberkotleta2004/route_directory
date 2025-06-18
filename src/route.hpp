#pragma once
#include <string>
#include <vector>
#include "stop.hpp"

namespace transport_catalogue {

class Route {
private:
    std::string name_;
    size_t stops_count_;
    size_t unique_stops_count_;
    std::vector<const Stop*> stops_;
public: 
    Route(std::string_view name_sv, std::vector<const Stop*>&& stops);
    Route(std::string&& name, std::vector<const Stop*>&& stops);
    
    std::string_view GetName() const noexcept;
    size_t GetStopsCount() const noexcept;
    size_t GetUniqueStopsCount() const noexcept;
    const std::vector<const Stop*>& GetStopsPtrs() const; 
private:
    size_t CountUniqueStopsCount(const std::vector<const Stop*>& stops) const;
};

} //namespace transport_catalogue
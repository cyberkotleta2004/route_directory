#pragma once
#include <string>
#include <vector>
#include "stop.hpp"

namespace transport_catalogue {

class Bus {
public: 
    Bus(const std::string& name, std::vector<const Stop*>&& stops);
    Bus(std::string&& name, std::vector<const Stop*>&& stops);
    Bus(std::string_view name_sv, std::vector<const Stop*>&& stops);
    
    std::string GetName() const;
    std::string_view GetNameSV() const;
    size_t GetStopsCount() const noexcept;
    size_t GetUniqueStopsCount() const noexcept;
    const std::vector<const Stop*>& GetStopsPtrs() const noexcept; 

private:
    std::string name_;
    size_t stops_count_;
    size_t unique_stops_count_;
    std::vector<const Stop*> stops_;

    void ValidateData(std::string_view name, const std::vector<const Stop*>& stops);
    size_t CountUniqueStopsCount(const std::vector<const Stop*>& stops) const;
};

} //namespace transport_catalogue
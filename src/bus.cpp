#include "bus.hpp"
#include <unordered_set>
#include <stdexcept>

namespace transport_catalogue {

Bus::Bus(const std::string& name, std::vector<const Stop*>&& stops) 
{
    ValidateData(name, stops);

    name_ = name;
    stops_ = std::move(stops);
    stops_count_ = stops_.size();
    unique_stops_count_ = CountUniqueStopsCount(stops_);
}

Bus::Bus(std::string&& name, std::vector<const Stop*>&& stops) 
{
    ValidateData(name, stops);

    name_ = std::move(name);
    stops_ = std::move(stops);
    stops_count_ = stops_.size();
    unique_stops_count_ = CountUniqueStopsCount(stops_);
}

Bus::Bus(std::string_view name, std::vector<const Stop*>&& stops)
{
    ValidateData(name, stops);

    name_ = name;
    stops_ = std::move(stops);
    stops_count_ = stops_.size();
    unique_stops_count_ = CountUniqueStopsCount(stops_);
}

std::string Bus::GetName() const {
    return name_;
}

std::string_view Bus::GetNameSV() const {
    std::string_view name_sv(name_);
    return name_sv;
}

size_t Bus::GetStopsCount() const noexcept {
    return stops_count_;
}

size_t Bus::GetUniqueStopsCount() const noexcept {
    return unique_stops_count_;
}

const std::vector<const Stop*>& Bus::GetStopsPtrs() const noexcept {
    return stops_;
}

void Bus::ValidateData(std::string_view name, const std::vector<const Stop*>& stops) {
    if(name.empty()) {
        throw std::invalid_argument("Bus name can not be empty!");
    } else if(stops.size() < 2) {
        throw std::invalid_argument("Bus should contain at least two stops!");
    }
}


size_t Bus::CountUniqueStopsCount(const std::vector<const Stop*>& stops) const {
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

} // namespace transport_catalogue
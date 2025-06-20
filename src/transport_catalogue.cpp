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

void TransportCatalogue::AddBus(Bus&& bus) {
    buses_.push_back(std::move(bus));

    const Bus* current_bus_ptr = &buses_.back();
    bus_name_to_bus_.insert({current_bus_ptr->GetNameSV(), current_bus_ptr});


    for(const Stop* ptr : current_bus_ptr->GetStopsPtrs()) {
        stop_ptr_to_bus_ptrs_[ptr].insert(current_bus_ptr);
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

const Bus* TransportCatalogue::GetBusPtr(std::string_view bus_name_sv) const {
    return bus_name_to_bus_.at(bus_name_sv);
}


const Stop& TransportCatalogue::GetStop(std::string_view stop_name_sv) const {
    return *stop_name_to_stop_.at(stop_name_sv);
}

const Bus& TransportCatalogue::GetBus(std::string_view bus_name_sv) const {
    return *bus_name_to_bus_.at(bus_name_sv);
}

size_t TransportCatalogue::GetStopsCountOnBus(std::string_view bus_name_sv) const {
    const Bus* bus_ptr = bus_name_to_bus_.at(bus_name_sv);
    size_t stops_count = bus_ptr->GetStopsCount();
    return stops_count;
}

size_t TransportCatalogue::GetUniqueStopsCountOnBus(std::string_view bus_name_sv) const {
    const Bus* bus_ptr = bus_name_to_bus_.at(bus_name_sv);
    size_t unique_stops_count = bus_ptr->GetUniqueStopsCount();
    return unique_stops_count;
}

size_t TransportCatalogue::GetBusLength(std::string_view bus_name_sv) const {
    const Bus* bus_ptr = bus_name_to_bus_.at(bus_name_sv);
    size_t bus_length = CountBusRealLength(bus_ptr);
    return bus_length;
}

double TransportCatalogue::GetBusCurvature(std::string_view bus_name_sv) const {
    const Bus* bus_ptr = bus_name_to_bus_.at(bus_name_sv);

    double bus_geographic_length = CountBusGeographicLength(bus_ptr);
    size_t bus_real_length = CountBusRealLength(bus_ptr);
    double curvature = static_cast<double>(bus_real_length) / bus_geographic_length;

    return curvature;
}


std::vector<std::string> TransportCatalogue::GetBusesGoesThroughStop(std::string_view stop_name_sv) const {
    const Stop* stop_ptr = stop_name_to_stop_.at(stop_name_sv);
    std::set<const Bus*> bus_ptrs = stop_ptr_to_bus_ptrs_.at(stop_ptr);

    std::vector<std::string> bus_names;

    for(const Bus* bus_ptr : bus_ptrs) {
        bus_names.push_back(std::string(bus_ptr->GetName()));
    }

    return bus_names;
}

double TransportCatalogue::CountBusGeographicLength(const Bus* bus) const {
    double bus_geographic_length = 0.0;

    auto stops_ptrs = bus->GetStopsPtrs();

    for(size_t i = 1; i < stops_ptrs.size(); ++i) {
        bus_geographic_length += geo::ComputeDistance(stops_ptrs[i - 1]->coordinates_, stops_ptrs[i]->coordinates_);
    }

    return bus_geographic_length;
}

size_t TransportCatalogue::CountBusRealLength(const Bus* bus) const {
    size_t bus_real_length = 0;

    auto stop_ptrs = bus->GetStopsPtrs();

    for(size_t i = 0; i < stop_ptrs.size() - 1; ++i) {
        bus_real_length += stops_pair_to_distance_.at({stop_ptrs[i], stop_ptrs[i + 1]});
    }

    return bus_real_length;
}

} // namespace transport_catalogue
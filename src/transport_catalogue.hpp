#pragma once
#include <deque>
#include <unordered_map>
#include <set>
#include <utility>
#include <string>
#include "stop.hpp"
#include "bus.hpp"

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
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, const Bus*> bus_name_to_bus_;
    std::unordered_map<const Stop*, std::set<const Bus*>> stop_ptr_to_bus_ptrs_;
    std::unordered_map<std::pair<const Stop*, const Stop*>, size_t, detail::StopPtrPairHasher> stops_pair_to_distance_;

public:
    TransportCatalogue() = default;
    TransportCatalogue(const TransportCatalogue&) = delete;
    TransportCatalogue(TransportCatalogue&&) = delete;
    TransportCatalogue& operator=(const TransportCatalogue&) = delete;
    TransportCatalogue& operator=(TransportCatalogue&&) = delete;

    void AddStop(Stop&& stop);
    void AddBus(Bus&& bus);
    void SetDistanceBetweenStops(const Stop* first_stop, const Stop* second_stop, size_t distance);
    void SetDistanceBetweenStops(const std::string& first_stop_name, const std::string& second_stop, size_t distance);

    const Stop* GetStopPtr(std::string_view stop_name_sv) const;
    const Bus* GetBusPtr(std::string_view bus_name_sv) const;
    const Stop& GetStop(std::string_view stop_name_sv) const;
    const Bus& GetBus(std::string_view bus_name_sv) const;
    size_t GetStopsCountOnBus(std::string_view bus_name_sv) const;
    size_t GetUniqueStopsCountOnBus(std::string_view bus_name_sv) const;
    size_t GetBusLength(std::string_view bus_name_sv) const;
    double GetBusCurvature(std::string_view bus_name_sv) const;
    std::vector<std::string> GetBusesGoesThroughStop(std::string_view stop_name_sv) const;

private:
    double CountBusGeographicLength(const Bus* bus) const;
    size_t CountBusRealLength(const Bus* bus) const;
};

} // namespace transport_catalogue

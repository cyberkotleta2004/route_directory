#include "transport_catalogue.h"
#include <unordered_set>
#include <stdexcept>
#include <format>

using namespace std::string_literals;

namespace transport_catalogue {
    void TransportCatalogue::AddStop(Stop&& stop) {
        stops_.push_back(std::move(stop));
        stop_name_to_stop_.insert({stops_.back().name_, &stops_.back()});
    }

    void TransportCatalogue::AddRoute(Route&& route) {
        routes_.push_back(std::move(route));
        route_name_to_route_.insert({routes_.back().GetName(), &routes_.back()});
    }

    const Stop& TransportCatalogue::GetStop(std::string_view stop_name) const {
        return *stop_name_to_stop_.at(stop_name);
    }

    const Route& TransportCatalogue::GetRoute(std::string_view route_name_sv) const {
        return *route_name_to_route_.at(route_name_sv);
    }

    std::string TransportCatalogue::GetRouteInfo(std::string_view route_name) const noexcept {
        try {
            const Route& route_ref = GetRoute(route_name);
            return std::format(
                "Bus {}: {} stops on route, {} unique stops, {:.6f} route length\n",
                route_ref.GetName(), 
                route_ref.GetStopsCount(), 
                route_ref.GetUniqueStopsCount(),
                route_ref.GetLength()
            );
        } catch(const std::out_of_range& except) {
            return std::format("Bus {}: not found\n", route_name);
        }
    }
}
#pragma once
#include <deque>
#include <unordered_map>
#include <string>
#include "stop.h"
#include "route.h"

namespace transport_catalogue {
    class TransportCatalogue {
    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, const Stop*> stop_name_to_stop_;
        std::deque<Route> routes_;
        std::unordered_map<std::string_view, const Route*> route_name_to_route_;

    public:
        void AddStop(Stop&& stop);
        void AddRoute(Route&& route);
        const Stop& GetStop(std::string_view stop_name_sv) const;
        const Route& GetRoute(std::string_view route_name_sv) const;
        std::string GetRouteInfo(std::string_view route_name_sv) const noexcept;
    };
};

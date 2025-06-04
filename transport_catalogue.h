#pragma once
#include <deque>
#include <vector>
#include <unordered_map>
#include <string>
#include "geo.h"

namespace transport_catalogue {
    struct Stop {
        std::string name_;
        geo::Coordinates coordinates_;
    };

    class Route {
    private:
        std::string name_;
        size_t stops_count_;
        size_t unique_stops_count_;
        double route_length_;
        std::vector<Stop*> route_;
    public: 
        Route(std::string_view name, std::vector<Stop*>&& route)
            : name_(name)
            , stops_count_(route.size() + 1)
            , unique_stops_count_(CountUniqueStopsCount(route))
            , route_length_(CountRouteLength(route))
            , route_(std::move(route))
        {}
            
        double GetRouteLength() const noexcept;
        size_t GetStopsCount() const noexcept;
        size_t GetUniqueStopsCount() const noexcept;
    private:
        size_t CountUniqueStopsCount(const std::vector<Stop*>& route);
        double CountRouteLength(const std::vector<Stop*>& route);
    };

    class TransportCatalogue {
    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, const Stop*> stop_name_to_stop;
        std::deque<Route> routes_;
        std::unordered_map<std::string_view, const Route*> route_name_to_route;

    public:
        void AddRoute(Route&& route);
        void AddStop(Stop&& stop);
        Route& GetRoute(const std::string& route_name) const;
        Stop& GetStop(const std::string& stop_name) const;
    };
};

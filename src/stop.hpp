#pragma once
#include <string>
#include "geo.hpp"

namespace transport_catalogue {

struct Stop {
    std::string name_;
    geo::Coordinates coordinates_;

    Stop(const std::string& name, geo::Coordinates coordinates) 
    : name_(name)
    , coordinates_(coordinates)
    {}
    
    Stop(std::string&& name, geo::Coordinates coordinates) 
        : name_(std::move(name))
        , coordinates_(coordinates)
    {}

    Stop(std::string_view name_sv, geo::Coordinates coordinates) 
        : name_(name_sv)
        , coordinates_(coordinates)
    {}
};

} // namespace transport_catalogue
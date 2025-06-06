#pragma once
#include <string>
#include "geo.h"

namespace tcat {
    struct Stop {
        std::string name_;
        geo::Coordinates coordinates_;

        Stop(std::string_view name_sv, geo::Coordinates coordinates) 
            : name_(name_sv)
            , coordinates_(coordinates)
        {}
    };

}
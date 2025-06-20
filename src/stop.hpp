#pragma once
#include <string>
#include "geo.hpp"

namespace transport_catalogue {

class Stop {
public:
    std::string name_;
    geo::Coordinates coordinates_;

    Stop(const std::string& name, geo::Coordinates coordinates);
    Stop(std::string&& name, geo::Coordinates coordinates);
    Stop(std::string_view name, geo::Coordinates coordinates);
private:
    void ValidateData(std::string_view name) const;
};

} // namespace transport_catalogue
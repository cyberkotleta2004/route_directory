#include "stop.hpp"
#include "stdexcept"
    
namespace transport_catalogue {

Stop::Stop(const std::string& name, geo::Coordinates coordinates)
    : coordinates_(coordinates)
{
    ValidateData(name);
    name_ = name;
}

Stop::Stop(std::string&& name, geo::Coordinates coordinates) 
    : coordinates_(coordinates)
{
    ValidateData(name);
    name_ = std::move(name);
}

Stop::Stop(std::string_view name, geo::Coordinates coordinates) 
    : coordinates_(coordinates)
{
    ValidateData(name);
    name_ = name;
}

void Stop::ValidateData(std::string_view name) const {
    if(name.empty()) {
        throw std::invalid_argument("Stop name can not be empty!");
    } 
}

} // namespace transport_catalogue
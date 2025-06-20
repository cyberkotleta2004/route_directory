#pragma once

#include <cmath>
#include <stdexcept>

namespace geo {

struct Coordinates {
public:
    double lat_;
    double lng_;

    Coordinates(double lat, double lng)
        : lat_(lat)
        , lng_(lng) 
    {
        if(lat_ < -90.0) {
            throw std::invalid_argument("Latitude can not be less than -90.0!");
        } else if (lat_ > 90.0) {
            throw std::invalid_argument("Latitude can not be more than 90.0!");
        } else if (lng_ < -180.0) {
            throw std::invalid_argument("Longitude can not be less than -180.0!");
        } else if (lng_ > 180.0) {
            throw std::invalid_argument("Longitude can not be more than 180.0!");
        }
    }

    bool operator==(const Coordinates& other) const noexcept {
        return lat_ == other.lat_ && lng_ == other.lng_;
    }
    bool operator!=(const Coordinates& other) const noexcept {
        return !(*this == other);
    }
};

inline double ComputeDistance(Coordinates from, Coordinates to) {
    using namespace std;
    if (from == to) {
        return 0;
    }
    static const double dr = 3.1415926535 / 180.;
    return acos(sin(from.lat_ * dr) * sin(to.lat_ * dr)
                + cos(from.lat_ * dr) * cos(to.lat_ * dr) * cos(abs(from.lng_ - to.lng_) * dr))
        * 6371000;
}

} // namespace geo

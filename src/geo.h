#pragma once

#include <cmath>

namespace geo {
    struct Coordinates {
        double lat_;
        double lng_;
        bool operator==(const Coordinates& other) const {
            return lat_ == other.lat_ && lng_ == other.lng_;
        }
        bool operator!=(const Coordinates& other) const {
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
}

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <=  b ? a : b; }

inline double ffmax(double a, double b) { return a >= b ? a : b; }

#endif
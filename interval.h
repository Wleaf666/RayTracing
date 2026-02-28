#ifndef INTERVAL_H
#define INTERVAL_H
#include "rtweekend.h"

class interval
{
public:
    double min, max;
    interval() : min(+infinity), max{-infinity} {}
    interval(double _min, double _max) : min(_min), max(_max) {}

    interval (const interval &a,const interval &b):min(ffmin(a.min,b.min)),max(ffmax(a.max,b.max)){}

    bool contains(double x) const
    {
        return min <= x && x < max;
    }

    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    double size()
    {
        return max - min;
    }

    interval expand(double delta)
    {
        return interval(min - delta, max + delta);
    }

    static const interval empty, universe;
};

const interval interval::empty(+infinity, -infinity);

const interval interval::universe(-infinity, +infinity);


inline interval operator+(const interval& ival,double offset)
{
    return interval(ival.min + offset, ival.max + offset);
}

inline interval operator+(double offset, const interval &ival)
{
    return interval(ival.min + offset, ival.max + offset);
}

#endif
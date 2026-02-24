#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"
#include "vec3.h"

class texture
{
    public:
        virtual ~texture() = default;
        virtual vec3 value(double u, double v, const vec3 &p) const = 0;
};

class solid_color:public texture
{

    public:
        vec3 color_value;
    public:
        solid_color(vec3 c):color_value(c){}
        solid_color(double r, double g, double b) : color_value(vec3(r,g,b)){}

        vec3 value(double u, double v, const vec3 &p) const override
        {
            return color_value;
        }
};

class checker_texture:public texture
{
    public:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
    public:
        checker_texture(double _scale, std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd):inv_scale(1/_scale),even(_even),odd(_odd){}
        checker_texture(double _scale, vec3 color1, vec3 color2) : inv_scale(1 / _scale), even(std::make_shared<solid_color>(color1)), odd(std::make_shared<solid_color>(color2)) {}

        vec3 value(double u,double v,const vec3 &p)const override
        {
            int xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
            int yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
            int zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, u, p) : odd->value(u, v, p);
        }


};

#endif
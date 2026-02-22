#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

class camera{
    public:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
    public:
        camera(double vfov, double aspect, vec3 lookfrom, vec3 lookat, vec3 vup)
        {
            origin = lookfrom;
            double theta = degrees_to_radians(vfov);
            double half_height = tan(theta / 2);
            double half_width = half_height * aspect;

            vec3 u, v, w;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            lower_left_corner = origin - half_width * u - half_height * v - w;

            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }
        ray get_ray(double u,double v)
        {
            return ray(origin, lower_left_corner+u*horizontal + v*vertical  - origin);
        }
};
#endif
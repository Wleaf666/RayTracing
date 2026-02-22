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
        vec3 u, v, w;
        double lens_radius;

    public:
        camera(double vfov, double aspect, vec3 lookfrom, vec3 lookat, vec3 vup,double apetrure,double focus_disk)
        {
            origin = lookfrom;
            lens_radius = apetrure / 2;

            double theta = degrees_to_radians(vfov);
            double half_height = tan(theta / 2);
            double half_width = half_height * aspect;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            lower_left_corner = origin - half_width * focus_disk * u - half_height * focus_disk * v - focus_disk*w ;

            horizontal = 2*half_width*focus_disk*u;
            vertical = 2*half_height*focus_disk*v;
        }
        ray get_ray(double s,double t)
        {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = rd.x() * u + rd.y() * v;
            return ray(origin+offset, lower_left_corner+s*horizontal + t*vertical  - origin-offset);
        }
};
#endif
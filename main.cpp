#include <iostream>
#include <cstdio>
#include "vec3.h"
#include "ray.h"
#include <cmath>
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "rtweekend.h"

// double hit_shape(const vec3 &center, double radius, const ray &r)
// {
//     vec3 oc = r.origin() - center;
//     double a = dot(r.direction(), r.direction());
//     double b = 2.0 * dot(oc, r.direction());
//     double c = dot(oc, oc) - radius * radius;
//     double discriminant = b * b - 4 * a * c;
//     if (discriminant < 0)
//         return -1.0;
//     else
//         return (-b - sqrt(discriminant)) / (2.0 * a);
// }

vec3 ray_color(const ray &r,const hittable &world)
{
    // double t = hit_shape(vec3(0, 0, -1), 0.5, r);
    // if (t > 0.0)
    // {
    //     vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
    //     return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    // }
    hit_record rec;
    if(world.hit(r,0,infinity,rec))
    {
        return 0.5 * (rec.normal + vec3(1.0, 1.0, 1.0));
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
    freopen("image.ppm", "w", stdout);
    const int image_width = 200;
    const int image_height = 100;
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    hittable_list world;
    world.add(std::make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(vec3(0, -100.5, -1), 100));

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            double u = double(i) / image_width;
            double v = double(j) / image_height;
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 color = ray_color(r,world);
            color.write_color(std::cout);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
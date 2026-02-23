#include <iostream>
#include <cstdio>
#include "vec3.h"
#include "ray.h"
#include <cmath>
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "rtweekend.h"
#include "camera.h"
#include "material.h"

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
hittable_list random_scene()
{
    using namespace std;
    hittable_list world;

    world.add(make_shared<sphere>(
        vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    world.add(
                        make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = vec3::random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(
        make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

    world.add(
        make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

vec3 ray_color(const ray &r, const hittable &world,int depth)
{
    // double t = hit_shape(vec3(0, 0, -1), 0.5, r);
    // if (t > 0.0)
    // {
    //     vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
    //     return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    // }
    hit_record rec;
    if(depth<=0)
        return vec3(1, 0, 0);
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        vec3 attenuation;
        if(rec.mat_ptr->scatter(r,rec,attenuation,scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return vec3(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
    freopen("image.ppm", "w", stdout);
    const int image_width = 1000;
    const int image_height =500;
    const int samples_per_pixel = 80;
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    int max_depth = 50;
    double aspect_ratio = double(image_width) / image_height;
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(20, aspect_ratio,lookfrom, lookat, vup, aperture, 10.0);

    hittable_list world=random_scene();
    // world.add(std::make_shared<sphere>(vec3(0, 0, -1), 0.5, std::make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
    // world.add(std::make_shared<sphere>(vec3(0, -100.5, -1), 100,std::make_shared<lambertian>(vec3(0.5,0.5,0.5))));

    // world.add(std::make_shared<sphere>(vec3(1, 0, -1), 0.5, std::make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.3)));
    // // world.add(std::make_shared<sphere>(vec3(-1, 0, -1), 0.5, std::make_shared<dielectric>(1.5)));

    // world.add(std::make_shared<sphere>(vec3(-1, 0, -1), 0.5, std::make_shared<dielectric>(1.5)));
    // world.add(std::make_shared<sphere>(vec3(-1, 0, -1), -0.45, std::make_shared<dielectric>(1.5)));
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            vec3 color(0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                double u = double(i + random_double()) / image_width;
                double v = double(j + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world,max_depth);
            }
            color.write_color(std::cout, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
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
#include "bvh.h"
#include "quad.h"
#include "triangle.h"

hittable_list random_scene()
{
    using namespace std;
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, vec3(.2, .3, .1), vec3(.9, .9,.9));

    world.add(make_shared<sphere>(
        vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

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
                    // vec3 center2 = center + vec3(0, random_double(0, 0.5), 0);
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
    world = hittable_list(make_shared<bvh_node>(world));
    return world;
}
void two_spheres()
{
    using namespace std;
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.4, vec3(.2, .3, .1), vec3(.9, .9, .9));

    world.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = vec3(13, 2, 3);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.apetrure = 0.1;

    cam.render(world);
}
void earth()
{
    using namespace std;
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = vec3(0, 0, 12);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.apetrure = 0;

    cam.render(hittable_list(globe));
}
void two_perlin_spheres()
{
    using namespace std;
    hittable_list world;

    auto pertext = make_shared<noise_texture>(20);
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = vec3(13, 2, 3);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.apetrure=0;

    cam.render(world);
}

void quads()
{
    using namespace std;

    hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian>(vec3(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(make_shared<image_texture>("b_632871a556bea3c3aa30a7c3e92bd9f9.jpg"));
    auto right_blue = make_shared<lambertian>(make_shared<image_texture>("b_f26c89bb915f83a77b5f3cfe9d0dd28e.jpg"));
    auto upper_orange = make_shared<lambertian>(make_shared<image_texture>("b_8f618d15ccc455720621e411853462a4.jpg"));
    auto lower_teal = make_shared<lambertian>(make_shared<image_texture>("b_fad585d95e4a99c1ef1fb0930a09cfbe.jpg"));

    // Quads
    world.add(make_shared<quad>(vec3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(vec3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(vec3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(vec3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookfrom = vec3(0, 0, 9);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.apetrure = 0;

    cam.render(world);
}

void triangles()
{
    hittable_list world;
    using namespace std;

    vec3 center = vec3(-2, -sqrt(3), -sqrt(3));
    vec3 u = vec3(3, 3 * sqrt(3), 0);
    vec3 v = vec3(6, 0, 0);
    world.add(make_shared<triangle>(center, u, v, make_shared<lambertian>(vec3(1.0, 0.2, 0.2))));

    camera cam;
    cam.lookfrom = vec3(0, 0, -9);
    cam.lookat = vec3(0, 0, -sqrt(3));
    cam.vfov = 120;
    cam.render(world);
}

int main()
{
    // camera cam;
    // hittable_list world=random_scene();
    // cam.render(world);
    quads();

    return 0;
}
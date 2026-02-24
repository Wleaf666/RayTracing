#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"

class camera{
    public:
        double vfov=20;
        vec3 lookfrom=vec3(13, 2, 3);
        vec3 lookat=vec3(0, 0, 0);
        vec3 vup=vec3(0, 1, 0);
        double aspect_ratio = 16.0/9;
        int image_width = 480;
        int image_height = 270;
        int max_depth = 50;
        int samples_per_pixel = 80;
        double apetrure = 0.1;
        double focus_dist = 10.0;

    private:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;

        public : camera()
        {
  
        }
        ray get_ray(double s,double t)
        {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = rd.x() * u + rd.y() * v;
            double ray_time = random_double();
            return ray(origin+offset, lower_left_corner+s*horizontal + t*vertical  - origin-offset,ray_time);
        }
        vec3 ray_color(const ray &r, const hittable &world, int depth)
        {

            hit_record rec;
            if (depth <= 0)
                return vec3(1, 0, 0);
            if (world.hit(r, interval(0.001,infinity), rec))
            {
                ray scattered;
                vec3 attenuation;
                if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, world, depth - 1);
                return vec3(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            double t = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        }
        void render(const hittable &world)
        {
            freopen("image.ppm", "w", stdout);
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
                        ray r = this->get_ray(u, v);
                        color += ray_color(r, world,max_depth);
                    }
                    color.write_color(std::cout, samples_per_pixel);
                }
            }
            std::cerr << "\nDone.\n";
        }
        void init()
        {
            origin = lookfrom;
            lens_radius = apetrure / 2;

            double theta = degrees_to_radians(vfov);
            double half_height = tan(theta / 2);
            double half_width = half_height * aspect_ratio;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            lower_left_corner = origin - half_width * focus_dist
             * u - half_height * focus_dist * v - focus_dist * w;

            horizontal = 2 * half_width * focus_dist * u;
            vertical = 2 * half_height * focus_dist * v;
        }
};

#endif
#include <iostream>
#include <cstdio>

int main()
{
    freopen("image.ppm", "w", stdout);
    const int image_width = 200;
    const int image_height = 100;
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            double r = double(i) / image_width;
            double g = double(j) / image_height;
            double b = 0.2;
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
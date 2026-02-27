void triangles()
{
    hittable_list world;
    using namespace std;

    vec3 center = vec3(-2, -sqrt(3), -sqrt(3));
    vec3 u = vec3(3, 3 * sqrt(3), 0);
    vec3 v = vec3(6, 0, 0);
    world.add(make_shared<triangle>(center, u, v, make_shared<lambertian>(vec3(1.0, 0.2, 0.2))));

    camera cam;
    cam.lookfrom = vec3(0, 0, 0);
    cam.lookat = vec3(0, 0, -sqrt(3));
    cam.render(world);
}
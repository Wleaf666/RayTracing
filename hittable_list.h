#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>
#include "rtweekend.h"
#include "aabb.h"
#include "quad.h"

class hittable_list : public hittable{
    public:
        std::vector<std::shared_ptr<hittable>> objects;
        aabb bbox;

    public:
        hittable_list(){}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<hittable> object) { 

            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }

        virtual bool hit(const ray &r, interval ray_t,hit_record &rec) const;

        aabb bounding_box() const { return bbox; }
};

bool hittable_list::hit(const ray &r,interval ray_t,hit_record &rec)const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;
    
    for(const auto &object:objects)
    {
        if(object->hit(r,interval(ray_t.min,closest_so_far),temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

inline std::shared_ptr<hittable_list> box(const vec3& a,const vec3& b,std::shared_ptr<material> mat)
{
    using namespace std;
    auto sides = make_shared<hittable_list>();
    auto min = vec3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = vec3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(vec3(min.x(), min.y(), max.z()), dx, dy, mat));  // front
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dz, dy, mat));  // left
    sides->add(make_shared<quad>(vec3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dx, dz, mat));  // bottom
    return sides;
}

#endif
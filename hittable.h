#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "memory"
#include "interval.h"
#include "aabb.h"
class material;

struct hit_record
{
    vec3 p; //指向相交点的向量
    vec3 normal; //法向量
    double t;
    bool front_face;
    std::shared_ptr<material> mat_ptr;

    inline void set_face_normal(const ray &r,const vec3 & outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable{
    public : 
        virtual bool hit(const ray &r, interval ray_t,hit_record &rec) const = 0;
        virtual aabb bounding_box() const = 0;
};

#endif
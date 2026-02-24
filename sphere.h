#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere:public hittable{
    public:
        vec3 center0;
        double radius;
        std::shared_ptr<material> mat_ptr;
        bool is_moving;
        vec3 center_vec;

    public:
        sphere(){}
        sphere(vec3 cen, double r,std::shared_ptr<material> m ): center0(cen), radius(r),mat_ptr(m) ,is_moving(false){}
        sphere(vec3 cen1, vec3 cen2, double r, std::shared_ptr<material> m) : center0(cen1), radius(r), mat_ptr(m), is_moving(true) 
        { 
            center_vec = cen2 - cen1; 
        }
        virtual bool hit(const ray &r, interval ray_t, hit_record & rec) const;

        vec3 center_in_time(double time)const
        {
            return center0 + time * center_vec;
        }
};

bool sphere::hit(const ray &r,interval ray_t,hit_record &rec)const
{
    vec3 center = is_moving ? center_in_time(r.tm) : center0;
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if(discriminant>0)
    {
        double root = sqrt(discriminant);
        double temp = (-half_b - root) / a;
        if(ray_t.surrounds(temp))
        {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b +root) / a;
        if (ray_t.surrounds(temp) )
        {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif
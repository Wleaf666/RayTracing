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
    double u;
    double v;

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

class translate: public hittable
{
    private:
        std::shared_ptr<hittable> object;
        vec3 offset;
        aabb bbox;

    public:
        translate(std::shared_ptr<hittable> _object,const vec3& _offset):object(_object),offset(_offset)
        {
            bbox = object->bounding_box() + offset;
        }

        aabb bounding_box()const override
        {
            return bbox;
        }

        bool hit(const ray& r,interval ray_t,hit_record& rec)const override
        {
            ray offset_r = ray(r.origin() - offset, r.direction(), r.time());
            if(!object->hit(offset_r,ray_t,rec))
                return false;
            rec.p += offset;
            return true;
        }
};

class rotate_y:public hittable
{
    private:
        std::shared_ptr<hittable> object;
        aabb bbox;
        double sin_theta;
        double cos_theta;
    public:
        rotate_y(std::shared_ptr<hittable>p,double angle):object(p)
        {
            double radians = degrees_to_radians(angle);

            sin_theta = sin(radians);
            cos_theta = cos(radians);

            bbox = object->bounding_box();
            vec3 min(infinity, infinity, infinity);
            vec3 max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                        auto newx = cos_theta * x + sin_theta * z;
                        auto newz = -sin_theta * x + cos_theta * z;

                        vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++)
                        {
                            min[c] = fmin(min[c], tester[c]);
                            max[c] = fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = aabb(min, max);
        }

        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            vec3 origin = r.origin();
            vec3 direction = r.direction();

            origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
            origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

            direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
            direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

            ray rotate_yr(origin, direction, r.time());

            if(!object->hit(rotate_yr,ray_t,rec))
                return false;
            vec3 p = rec.p;
            p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
            p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

            vec3 normal = rec.normal;
            normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
            normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

            rec.p = p;
            rec.normal = normal;
            return true;
        }

        aabb bounding_box() const override
        {
            return bbox;
        }
};


#endif
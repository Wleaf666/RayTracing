#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "rtweekend.h"
#include "material.h"

class triangle:public hittable
{
    private:
        vec3 P;
        vec3 u, v;
        aabb bbox;
        std::shared_ptr<material> mat_ptr;
        vec3 normal;
        double D;
        vec3 w;

    public:
        triangle(const vec3& _P,const vec3& _u,const vec3& _v,std::shared_ptr<material> _mat_ptr):P(_P),u(_u),v(_v),mat_ptr(_mat_ptr)
        {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, P);
            w = n / dot(n, n);
            set_bounding_box();
        }
        virtual void set_bounding_box()
        {
            aabb box_u(P, P + u);
            aabb box_v(P + v, P + v);
            bbox = aabb(box_u, box_v).pad();
        }
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            auto denom = dot(normal, r.direction());
            if (abs(denom) < 1e-8)
                return false;
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t))
                return false;
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - P;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));
            if (!is_interior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.p = intersection;
            rec.mat_ptr = mat_ptr;
            rec.set_face_normal(r, normal);
            return true;
        }

        bool is_interior(double alpha,double beta,hit_record& rec)const
        {
            if(alpha<0||beta<0||alpha>1||beta>1||alpha+beta>1)
                return false;
            rec.u = alpha;
            rec.v = beta;
            return true;

        }

        aabb bounding_box()const
        {
            return bbox;
        }
};

#endif
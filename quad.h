#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "rtweekend.h"
#include "vec3.h"

class quad:public hittable
{
    private:
        vec3 Q;
        vec3 u, v;
        aabb bbox;
        std::shared_ptr<material> mat_ptr;
        vec3 normal;
        double D;
        vec3 w;

    public:
        quad(const vec3& _Q,const vec3 _u,const vec3 _v,std::shared_ptr<material> _mat_ptr):Q(_Q),u(_u),v(_v),mat_ptr(_mat_ptr)
        {
            
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n, n);
            set_bounding_box();
        }

        virtual void set_bounding_box()
        {
            bbox = aabb(Q, Q + u + v).pad();
        }

        bool hit(const ray& r,interval ray_t,hit_record& rec)const override
        {
            auto denom = dot(normal, r.direction());
            if(abs(denom)<1e-8)
                return false;
            auto t = (D - dot(normal, r.origin())) / denom;
            if(!ray_t.contains(t))
                return false;
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));
            if(!is_interior(alpha,beta,rec))
                return false;

            rec.t = t;
            rec.p = intersection;
            rec.mat_ptr = mat_ptr;
            rec.set_face_normal(r, normal);
            return true;
        }

        virtual bool is_interior(double a,double b,hit_record& rec)const
        {
            if((a<0)||(1<a)||(b<0)||(1<b))
                return false;
            rec.u = a;
            rec.v = b;
            return true;
        }

        aabb bounding_box()const
        {
            return bbox;
        }
};

#endif
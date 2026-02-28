#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "interval.h"
#include "texture.h"


class material
{
    public:
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
        virtual vec3 emitted(double u,double v,const vec3& p)const
        {
            return vec3(0, 0, 0);
        }
};

class lambertian : public material
{
    public:
        std::shared_ptr<texture> albedo;

    public:
        lambertian() {};
        lambertian(const vec3 &a) : albedo(std::make_shared<solid_color>(a)) {}
        lambertian(std::shared_ptr<texture> a):albedo(a){}

        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered)const
        {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero())
            {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction,r_in.time());
            attenuation = albedo->value(rec.u,rec.v,rec.p);
            return true;
        }
};

class metal : public material
{
    public:
        std::shared_ptr<texture> albedo;
        double fuzz;

    public:
        metal(){}
        metal(const vec3 &a,double f):albedo(std::make_shared<solid_color>(a)),fuzz(f<1?f:1){}
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere(),r_in.time());
            attenuation = albedo->value(rec.u,rec.v,rec.p);
            return (dot(scattered.direction(),rec.normal)>0);
        }
};

class dielectric : public material
{
    public:
        double ref_idx;

        dielectric(){}
        dielectric(double ri) : ref_idx(ri) {};

        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            attenuation = vec3(1.0, 1.0, 1.0);
            double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
            if(etai_over_etat*sin_theta>1.0)
            {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected,r_in.time());
                return true;
            }
            double reflect_prob = schlick(cos_theta, etai_over_etat);
            if(random_double()<reflect_prob)
            {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected,r_in.time());
                return true;
            }

            vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            scattered = ray(rec.p, refracted,r_in.time());
            return true;
        }
};

class diffuse_light:public material
{
    private:
        std::shared_ptr<texture> emit_mat;
    public:
        diffuse_light(std::shared_ptr<texture> a):emit_mat(a){}
        diffuse_light(vec3 color):emit_mat(std::make_shared<solid_color>(color)){}

        bool scatter(const ray& r_in,const hit_record& rec,vec3& attenuation,ray& scattered)const override
        {
            return false;
        }

        vec3 emitted(double u,double v,const vec3& p)const override
        {
            return emit_mat->value(u, v, p);
        }
};

class isotropic:public material
{
    private:
        std::shared_ptr<texture> albedo;
    public:
        isotropic(vec3 c) : albedo(std::make_shared<solid_color>(c)) {};
        isotropic(std::shared_ptr<texture> a) : albedo(a){}

        bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override
        {
            scattered = ray(rec.p, random_unit_vector(), r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }
};

#endif
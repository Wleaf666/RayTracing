#ifndef VEC3_H
#define VEC3_H


#include <iostream>
#include <math.h>
#include "rtweekend.h"

class vec3
{
    public:
        double e[3];
    public:
        vec3():e{0,0,0}{}
        vec3(double e0,double e1,double e2):e{e0,e1,e2}{}

        double x()const
        {
            return e[0];
        }
        double y() const
        {
            return e[1];
        }
        double z() const
        {
            return e[2];
        }

        vec3 operator-()const
        {
            return vec3(-e[0], -e[1], -e[2]);
        }
        double operator[](int i)const
        {
            return e[i];
        }
        double& operator[](int i)
        {
            return e[i];
        }

        vec3& operator+=(const vec3& v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3 &operator*=(const double t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3 &operator/=(const double t)
        {

            return *this*=1/t;
        }

        double length()const
        {
            return sqrt(length_squared());
        }

        double length_squared()const
        {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }
        bool near_zero()const
        {
            double s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s)&&(fabs(e[2]) < s);
        }

        void write_color(std::ostream &out,int samples_per_pixel){
            double scale = 1.0 / samples_per_pixel;
            double r = sqrt(scale * e[0]);
            double g = sqrt(scale * e[1]);
            double b = sqrt(scale * e[2]);
            out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' ' << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' ' << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
        }

        public:
            inline static vec3 random()
            {
                return vec3(random_double(), random_double(), random_double());
            }
            inline static vec3 random(double min,double max)
            {
                return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
            }
};

vec3 random_in_unit_sphere()
{
    while(true)
    {
        vec3 p = vec3::random(-1.0, 1.0);
        if(p.length_squared()>=1)
            continue;
        return p;
    }
}

vec3 random_unit_vector()
{
    double a = random_double(0, 2 * pi);
    double z = random_double(-1,1);
    double r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

inline std::ostream& operator<<(std::ostream &out,const vec3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& v1,const vec3& v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(double t,const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v,double t)
{
    return (1 / t) *v ;
}

inline double dot(const vec3 &v1,const vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

inline vec3 reflect(const vec3 &v,const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3 &uv,const vec3 &n,double etai_over_etat)
{
    double cos_thetta = dot(-uv, n);
    vec3 r_out_parallel = etai_over_etat * (uv + cos_thetta * n);
    vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
    return r_out_parallel + r_out_perp;
}

double schlick(double cosine,double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

vec3 random_in_unit_disk()
{
    while(true)
    {
        vec3 p = vec3(random_double(-1, 1), random_double(-1, 1),0);
        if(p.length_squared()>=1)continue;
        return p;
    }
}

#endif
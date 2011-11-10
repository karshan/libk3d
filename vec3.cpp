#include <math.h>

#include "k3d.h"

namespace k3d {

    void vec3::normalize()
    {
        float den = sqrt(x*x + y*y + z*z);
        if (den > 0.0) {
            x /= den;
            y /= den;
            z /= den;
        }
    }

    vec3 & vec3::operator=(const vec3 & rhs)
    {
        if (this == &rhs)
            return *this;
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    vec3 cross(const vec3 & a, const vec3 & b)
    {
        vec3 r(0, 0, 0);
        r.x = a.y * b.z - a.z * b.y;
        r.y = -a.x * b.z + a.z * b.x;
        r.z = a.x * b.y - a.y * b.x;
        return r;
    }


    float dot(const vec3 & a, const vec3 & b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    vec3 operator+(const vec3 & a, const vec3 & b)
    {
        return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    vec3 operator-(const vec3 & a, const vec3 & b)
    {
        return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

} // namespace k3d

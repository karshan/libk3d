#include <math.h>

#include "k3d.h"

namespace k3d {

    std::ostream & operator<<(std::ostream & os, const vec2 & v)
    {
        os << '<' << v.x << ", " << v.y << '>';
        return os;
    }

    vec2 & vec2::normalize()
    {
        float den = sqrt(x*x + y*y);
        if (den > 0.0) {
            x /= den;
            y /= den;
        }
        return *this;
    }

    float dot(const vec2 & a, const vec2 & b)
    {
        return a.x*b.x + a.y*b.y;
    }

    vec2 & vec2::operator=(const vec2 & rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    vec2 operator+(const vec2 & a, const vec2 & b)
    {
        return vec2(a.x + b.x, a.y + b.y);
    }

    vec2 operator-(const vec2 & a, const vec2 & b)
    {
        return vec2(a.x - b.x, a.y - b.y);
    }

    vec2 operator*(const float s, const vec2 & v)
    {
        return vec2(s*v.x, s*v.y);
    }

}

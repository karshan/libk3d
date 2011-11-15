#include "k3d.h"

namespace k3d {

    vec2 & vec2::operator=(const vec2 & rhs)
    {
        x = rhs.x;
        y = rhs.y;
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

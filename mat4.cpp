#include <iostream>
#include <iomanip>
#include <math.h>

#include "k3d.h"

namespace k3d {

    mat4::mat4() {
        loadIdentity();
    }

    mat4::mat4(const float *mat) {
#define M(row, col) mat[4*col + row]
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                m[col][row] = M(row, col);
            }
        }
#undef M
    }

    mat4::mat4(const float mat[4][4]) {
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                m[col][row] = mat[col][row];
            }
        }
    }

    bool operator==(const mat4 & a, const mat4 & b) {
        float d;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                d = a.m[i][j] - b.m[i][j];
                d = d < 0 ? -d:d;
                if (d > 0.0001)
                    return false;
            }
        }
        return true;
    }

    mat4 operator*(const mat4 & a, const mat4 & b)
    {
        mat4 r;
        r.m[0][0] = 0.0;
        r.m[1][1] = 0.0;
        r.m[2][2] = 0.0;
        r.m[3][3] = 0.0;
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                for (int k = 0; k < 4; k++) {
                    r.m[i][j] += a.m[k][j] * b.m[i][k];
                }
            }
        }
        return r;
    }

    std::ostream & operator<<(std::ostream & os, const mat4 & m) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                os << std::fixed << std::setw(10) << std::setprecision(3) << m.m[col][row] << ' ';
            }
            os << '\n';
        }
    }

    void mat4::glUniform(GLuint loc)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)m);
    }

    void mat4::loadIdentity()
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[j][i] = (i == j)?1.0:0.0;
            }
        }
    }

    mat4 & mat4::transpose()
    {
        mat4 tmp(*this);
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                m[row][col] = tmp.m[col][row];
            }
        }
        return *this;
    }

    void mat4::scalef(float x, float y, float z)
    {
        float s[3] = {x, y, z};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                m[col][row] *= s[col];
            }
        }
    }

    void mat4::translatef(float a, float b, float c)
    {
        float t[3] = {a, b, c};
        for (int row = 0; row < 4; row++) {
            m[3][row] += a*m[0][row] + b*m[1][row] + c*m[2][row];
        }
    }

    void mat4::rotatef(vec3 u, float angle)
    {
        angle = angle * M_PI/180.0;
        float cos = cosf(angle);
        float sin = sinf(angle);
        u.normalize();
        float r[4][4] =
        {
            { cos + u.x*u.x*(1-cos), u.x*u.y*(1-cos) - u.z*sin, u.x*u.z*(1-cos) + u.y*sin, 0.0 },
            { u.y*u.x*(1-cos) + u.z*sin, cos + u.y*u.y*(1-cos), u.y*u.z*(1-cos) - u.x*sin, 0.0 },
            { u.z*u.x*(1-cos) - u.y*sin, u.z*u.y*(1-cos) + u.x*sin, cos + u.z*u.z*(1-cos), 0.0 },
            { 0.0, 0.0, 0.0, 1.0 }
        };
        *this = *this * (mat4(r)).transpose(); //FIXME sad bug, I didn't write the matrix in column major so I have to transpose it here
    }

    void mat4::lookAt(vec3 eye, vec3 center, vec3 up)
    {
        vec3 n = (eye - center); // viewplane normal == lookat vector FIXME what the hell ? this should be -n
        vec3 v = up - ((dot(up, n)/dot(n, n))*n); // up vector projection in viewplane
        vec3 u = cross(v, n).normalize();
        n.normalize();
        v.normalize();

        // remember this is column major
        float m[4][4] = {
            { u.x, u.y, u.z, 0.0 },
            { v.x, v.y, v.z, 0.0 },
            { n.x, n.y, n.z, 0.0 },
            { 0.0, 0.0, 0.0, 1.0}
        };
        *this = *this * mat4(m).transpose(); // FIXME what the hell... why is transpose correct ?

        translatef(-eye.x, -eye.y, -eye.z);
    }

    // This is a temporary function, it will be replaced with a perspective
    // function that allows you to specify a viewing frustum
    void mat4::infPerspective()
    {
        mat4 m;
        m.m[3][3] = 0.0;
        m.m[2][3] = -1.0;
        *this = *this * m;
    }

}

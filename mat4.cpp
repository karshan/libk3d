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

    vec3 operator*(const mat4 & mat, const vec3 & v) //hacky
    {
        vec3 out(mat.m[0][0]*v.x + mat.m[1][0]*v.y + mat.m[2][0]*v.z + mat.m[3][0],
                mat.m[0][1]*v.x + mat.m[1][1]*v.y + mat.m[2][1]*v.z + mat.m[3][1],
                mat.m[0][2]*v.x + mat.m[1][2]*v.y + mat.m[2][2]*v.z + mat.m[3][2]);
        float w = mat.m[0][3]*v.x + mat.m[1][3]*v.y + mat.m[2][3]*v.z + mat.m[3][3];
        return (1.0/w)*out;
    }

    mat4 & mat4::operator*=(const float s)
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] *= s;
            }
        }
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

    mat4 & mat4::inverse()
    {
        // Calculate all mat2 determinants
        float SubFactor00 = this->m[2][2] * this->m[3][3] - this->m[3][2] * this->m[2][3];
        float SubFactor01 = this->m[2][1] * this->m[3][3] - this->m[3][1] * this->m[2][3];
        float SubFactor02 = this->m[2][1] * this->m[3][2] - this->m[3][1] * this->m[2][2];
        float SubFactor03 = this->m[2][0] * this->m[3][3] - this->m[3][0] * this->m[2][3];
        float SubFactor04 = this->m[2][0] * this->m[3][2] - this->m[3][0] * this->m[2][2];
        float SubFactor05 = this->m[2][0] * this->m[3][1] - this->m[3][0] * this->m[2][1];
        float SubFactor06 = this->m[1][2] * this->m[3][3] - this->m[3][2] * this->m[1][3];
        float SubFactor07 = this->m[1][1] * this->m[3][3] - this->m[3][1] * this->m[1][3];
        float SubFactor08 = this->m[1][1] * this->m[3][2] - this->m[3][1] * this->m[1][2];
        float SubFactor09 = this->m[1][0] * this->m[3][3] - this->m[3][0] * this->m[1][3];
        float SubFactor10 = this->m[1][0] * this->m[3][2] - this->m[3][0] * this->m[1][2];
        float SubFactor11 = this->m[1][1] * this->m[3][3] - this->m[3][1] * this->m[1][3];
        float SubFactor12 = this->m[1][0] * this->m[3][1] - this->m[3][0] * this->m[1][1];
        float SubFactor13 = this->m[1][2] * this->m[2][3] - this->m[2][2] * this->m[1][3];
        float SubFactor14 = this->m[1][1] * this->m[2][3] - this->m[2][1] * this->m[1][3];
        float SubFactor15 = this->m[1][1] * this->m[2][2] - this->m[2][1] * this->m[1][2];
        float SubFactor16 = this->m[1][0] * this->m[2][3] - this->m[2][0] * this->m[1][3];
        float SubFactor17 = this->m[1][0] * this->m[2][2] - this->m[2][0] * this->m[1][2];
        float SubFactor18 = this->m[1][0] * this->m[2][1] - this->m[2][0] * this->m[1][1];
/*
        tmat4x4<T> Inverse(
            + (this->m[1][1] * SubFactor00 - this->m[1][2] * SubFactor01 + this->m[1][3] * SubFactor02),
            - (this->m[1][0] * SubFactor00 - this->m[1][2] * SubFactor03 + this->m[1][3] * SubFactor04),
            + (this->m[1][0] * SubFactor01 - this->m[1][1] * SubFactor03 + this->m[1][3] * SubFactor05),
            - (this->m[1][0] * SubFactor02 - this->m[1][1] * SubFactor04 + this->m[1][2] * SubFactor05),

            - (this->m[0][1] * SubFactor00 - this->m[0][2] * SubFactor01 + this->m[0][3] * SubFactor02),
            + (this->m[0][0] * SubFactor00 - this->m[0][2] * SubFactor03 + this->m[0][3] * SubFactor04),
            - (this->m[0][0] * SubFactor01 - this->m[0][1] * SubFactor03 + this->m[0][3] * SubFactor05),
            + (this->m[0][0] * SubFactor02 - this->m[0][1] * SubFactor04 + this->m[0][2] * SubFactor05),

            + (this->m[0][1] * SubFactor06 - this->m[0][2] * SubFactor07 + this->m[0][3] * SubFactor08),
            - (this->m[0][0] * SubFactor06 - this->m[0][2] * SubFactor09 + this->m[0][3] * SubFactor10),
            + (this->m[0][0] * SubFactor11 - this->m[0][1] * SubFactor09 + this->m[0][3] * SubFactor12),
            - (this->m[0][0] * SubFactor08 - this->m[0][1] * SubFactor10 + this->m[0][2] * SubFactor12),

            - (this->m[0][1] * SubFactor13 - this->m[0][2] * SubFactor14 + this->m[0][3] * SubFactor15),
            + (this->m[0][0] * SubFactor13 - this->m[0][2] * SubFactor16 + this->m[0][3] * SubFactor17),
            - (this->m[0][0] * SubFactor14 - this->m[0][1] * SubFactor16 + this->m[0][3] * SubFactor18),
            + (this->m[0][0] * SubFactor15 - this->m[0][1] * SubFactor17 + this->m[0][2] * SubFactor18));
*/
        float inv[16] = {
            + this->m[1][1] * SubFactor00 - this->m[1][2] * SubFactor01 + this->m[1][3] * SubFactor02,
            - this->m[1][0] * SubFactor00 + this->m[1][2] * SubFactor03 - this->m[1][3] * SubFactor04,
            + this->m[1][0] * SubFactor01 - this->m[1][1] * SubFactor03 + this->m[1][3] * SubFactor05,
            - this->m[1][0] * SubFactor02 + this->m[1][1] * SubFactor04 - this->m[1][2] * SubFactor05,

            - this->m[0][1] * SubFactor00 + this->m[0][2] * SubFactor01 - this->m[0][3] * SubFactor02,
            + this->m[0][0] * SubFactor00 - this->m[0][2] * SubFactor03 + this->m[0][3] * SubFactor04,
            - this->m[0][0] * SubFactor01 + this->m[0][1] * SubFactor03 - this->m[0][3] * SubFactor05,
            + this->m[0][0] * SubFactor02 - this->m[0][1] * SubFactor04 + this->m[0][2] * SubFactor05,

            + this->m[0][1] * SubFactor06 - this->m[0][2] * SubFactor07 + this->m[0][3] * SubFactor08,
            - this->m[0][0] * SubFactor06 + this->m[0][2] * SubFactor09 - this->m[0][3] * SubFactor10,
            + this->m[0][0] * SubFactor11 - this->m[0][1] * SubFactor09 + this->m[0][3] * SubFactor12,
            - this->m[0][0] * SubFactor08 + this->m[0][1] * SubFactor10 - this->m[0][2] * SubFactor12,

            - this->m[0][1] * SubFactor13 + this->m[0][2] * SubFactor14 - this->m[0][3] * SubFactor15,
            + this->m[0][0] * SubFactor13 - this->m[0][2] * SubFactor16 + this->m[0][3] * SubFactor17,
            - this->m[0][0] * SubFactor14 + this->m[0][1] * SubFactor16 - this->m[0][3] * SubFactor18,
            + this->m[0][0] * SubFactor15 - this->m[0][1] * SubFactor17 + this->m[0][2] * SubFactor18 };

        mat4 Inverse(inv);
        Inverse.transpose();
        float Determinant = 
                        + this->m[0][0] * Inverse.m[0][0] 
                        + this->m[0][1] * Inverse.m[1][0] 
                        + this->m[0][2] * Inverse.m[2][0] 
                        + this->m[0][3] * Inverse.m[3][0];

        Inverse *= (1.0/Determinant);
        *this = Inverse;
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
        m.m[2][2] = 0.0;
        m.m[2][3] = -1.0;
        *this = *this * m;
    }

    void mat4::perspective(float left, float right, float top, float bottom, float near, float far)
    {
        float pm[16] = { (-2*near)/(right-left), 0.0, 0.0, 0.0,
                         0.0, (-2*near)/(top-bottom), 0.0, 0.0,
                         (right+left)/(right-left), (top+bottom)/(top-bottom), -((far+near)/(far-near)), -1.0,
                         0.0, 0.0, (2*far*near)/(far-near), 0.0 };
        *this = *this * mat4(pm);
    }

}

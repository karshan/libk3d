#include "k3d.h"

namespace k3d {

    mat4::mat4() {
        loadIdentity();
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

    void mat4::loadIdentity()
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[j][i] = (i == j)?1.0:0.0;
            }
        }
    }

    void mat4::scalef(float x, float y, float z)
    {
        float s[3] = {x, y, z};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                m[j][i] *= s[i];
            }
        }
    }

    void mat4::translatef(float a, float b, float c)
    {
        float t[3] = {a, b, c};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                m[j][i] += m[j][3]*t[i];
            }
        }
    }

    void mat4::glUniform(GLuint loc)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)m);
    }

}

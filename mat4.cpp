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
        float s[4] = {x, y, z, 1.0};
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                m[col][row] *= s[col];
            }
        }
    }

    void mat4::translatef(float a, float b, float c)
    {
        float t[3] = {a, b, c};
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 4; col++) {
                m[col][row] += m[col][3]*t[row];
            }
        }
    }

    void mat4::glUniform(GLuint loc)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)m);
    }

}

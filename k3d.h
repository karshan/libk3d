#ifndef _K3D_H
#define _K3D_H

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef linux 
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <iostream>
#include <vector>

namespace k3d
{
    struct vec3 {
        float x, y, z;

        vec3(): x(0.0), y(0.0), z(0.0) {}
        vec3(float xx, float yy, float zz): x(xx), y(yy), z(zz) {}

        void normalize();
        vec3 & operator=(const vec3 & rhs);
        friend vec3 cross(const vec3 & a, const vec3 & b);
        friend float dot(const vec3 & a, const vec3 & b);
        friend vec3 operator+(const vec3 & a, const vec3 & b);
        friend vec3 operator-(const vec3 & a, const vec3 & b);
    };

    struct mat4 {
        float m[4][4];

        mat4();
        mat4(float *mat);

        friend std::ostream & operator<<(std::ostream &, const mat4 &);

        void glUniform(GLuint loc);

        void loadIdentity();

        void scalef(float x, float y, float z);

        void translatef(float a, float b, float c);

        friend mat4 operator*(const mat4 & a, const mat4 & b);
    };

    struct face {
        GLushort a, b, c;

        face(): a(0), b(0), c(0) {}
        face(GLushort aa, GLushort bb, GLushort cc) : a(aa), b(bb), c(cc) {}
    };

    class model {
        std::vector<vec3> vs;
        std::vector<vec3> ns;
        std::vector<face> faces;

        void computeNormals();
    public:
        void draw(GLuint, GLuint);

        void clear();

        bool loadObj(const char *objfilename);
    };
} // namespace k3d

#endif //_K3D_H

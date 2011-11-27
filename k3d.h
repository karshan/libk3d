#ifndef _K3D_H
#define _K3D_H

#if 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <iostream>
#include <vector>

// TODO: const correctness (for member functions)
namespace k3d
{
    struct vec2 {
        float x, y;

        vec2(): x(0.0), y(0.0) {}
        vec2(float xx, float yy): x(xx), y(yy) {}


        vec2 & normalize();
        friend float dot(const vec2 & a, const vec2 & b);
        vec2 & operator=(const vec2 & rhs);
        friend vec2 operator+(const vec2 & a, const vec2 & b);
        friend vec2 operator-(const vec2 & a, const vec2 & b);
        friend vec2 operator*(const float s, const vec2 & v);
    };

    struct vec3 {
        float x, y, z;

        vec3(): x(0.0), y(0.0), z(0.0) {}
        vec3(float xx, float yy, float zz): x(xx), y(yy), z(zz) {}

        friend std::ostream & operator<<(std::ostream &, const vec3 &);
        vec3 & normalize();
        float magnitude();
        vec3 & operator=(const vec3 & rhs);
        friend vec3 cross(const vec3 & a, const vec3 & b);
        friend float dot(const vec3 & a, const vec3 & b);
        friend vec3 operator+(const vec3 & a, const vec3 & b);
        friend vec3 operator-(const vec3 & a, const vec3 & b);
        friend vec3 operator*(const float s, const vec3 & v);
    };

    struct mat4 {
        float m[4][4];

        mat4();
        mat4(const float *mat);
        mat4(const float mat[4][4]);

        friend std::ostream & operator<<(std::ostream &, const mat4 &);

        friend bool operator==(const mat4 & a, const mat4 & b);

        friend mat4 operator*(const mat4 & a, const mat4 & b);
        mat4 & operator*=(const float s);

        void glUniform(GLuint loc);

        void loadIdentity();

        mat4 & transpose();

        mat4 & inverse();

        void scalef(float x, float y, float z);

        void translatef(float a, float b, float c);

        void rotatef(vec3 u, float angle);

        void lookAt(vec3 eye, vec3 center, vec3 up);

        void infPerspective(); // TO BE REPLACED

        void perspective(float left, float right, float top, float bottom, float near, float far);
    };

    vec3 operator*(const mat4 & mat, const vec3 & v); //hacky

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
        model() {}
        model(const char *objfilename) { loadObj(objfilename); }

        void draw();

        void clear();

        bool loadObj(const char *objfilename);
    };

    class gl {
        // Handles to uniform variables in the shader
        static GLuint gProgram;
        static GLuint gmMV;     // mModelView
        static GLuint gmMVP;    // mModelViewProjection
        static GLuint gmN;      // mNormalMatrix
        static GLuint gvLight0;
        static GLuint gvColor;
        static GLuint gvPosition;
        static GLuint gvNormal;

        static k3d::mat4 mNormal;

        static const char *readFile(const char *filename);
        static GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
        static GLuint loadShader(GLenum shaderType, const char* pSource);
    public:
        static k3d::mat4 mModelView;
        static k3d::mat4 mProjection;
        static k3d::vec3 vLight0;

        static void initialize(const char *vsfilename, const char *fsfilename);
        static void sendMatrices();
        static void sendLight0();
        static void sendColor(float r, float g, float b, float a);
        static void draw(const std::vector<vec3> &vs, const std::vector<vec3> &ns, const std::vector<face> &faces);
    };
} // namespace k3d

#endif //_K3D_H

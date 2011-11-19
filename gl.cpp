#include "k3d.h"
#include "util.h"

namespace k3d {

    GLuint gl::gmMV = -1;     // mModelView
    GLuint gl::gmMVP = -1;    // mModelViewProjection
    GLuint gl::gmN = -1;      // mNormalMatrix
    GLuint gl::gvLight0 = -1;
    GLuint gl::gvPosition = -1;
    GLuint gl::gvNormal = -1;
    GLuint gl::gvColor = -1;
    k3d::mat4 gl::mModelView;
    k3d::mat4 gl::mProjection;
    k3d::vec3 gl::vLight0;
    k3d::mat4 gl::mNormal;


    void gl::initialize(GLuint pos, GLuint norm, GLuint MV, GLuint MVP, GLuint N, GLuint Color, GLuint Light0)
    {
        gl::gvPosition = pos;
        gl::gvNormal = norm;
        gl::gmMV = MV;
        gl::gmMVP = MVP;
        gl::gmN = N;
        gl::gvColor = Color;
        gl::gvLight0 = Light0;
    }

    void gl::sendMatrices()
    {
        gl::mModelView.glUniform(gl::gmMV);
        checkGlError("glUniformMatrix4fv mModelView");

        gl::mNormal = mModelView;
        gl::mNormal.inverse().transpose();
        gl::mNormal.glUniform(gl::gmN);
        checkGlError("glUniformMatrix4fv mNormal");

        (gl::mProjection * gl::mModelView).glUniform(gl::gmMVP);
        checkGlError("glUniformMatrix4fv mModelViewProjection");
    }

    void gl::sendLight0()
    {
        vec3 light0 = gl::mModelView * gl::vLight0;
        glUniform3f(gl::gvLight0, light0.x, light0.y, light0.z);
        checkGlError("glUniform3f");
    }

    void gl::draw(const std::vector<vec3> &vs, const std::vector<vec3> &ns, const std::vector<face> &faces)
    {
        glVertexAttribPointer(gl::gvPosition, 3, GL_FLOAT, GL_FALSE, 0, &(vs[0]));
        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gl::gvPosition);
        checkGlError("glEnableVertexAttribArray");

        glVertexAttribPointer(gl::gvNormal, 3, GL_FLOAT, GL_FALSE, 0, &(ns[0]));
        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gl::gvNormal);
        checkGlError("glEnableVertexAttribArray");

        glDrawElements(GL_TRIANGLES, faces.size()*3, GL_UNSIGNED_SHORT, &(faces[0]));
        checkGlError("glDrawElements");
    }

    void gl::sendColor(float r, float g, float b, float a)
    {
        glUniform4f(gl::gvColor, r, g, b, a);
        checkGlError("glUniform4f gvColor");
    }

} // namespace k3d

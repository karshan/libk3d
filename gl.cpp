#include "k3d.h"
#include "util.h"

namespace k3d {

    GLuint gl::gmMV = -1;     // mModelView
    GLuint gl::gmMVP = -1;    // mModelViewProjection
    GLuint gl::gmN = -1;      // mNormalMatrix
    GLuint gl::gvLight0 = -1;
    k3d::mat4 gl::mModelView;
    k3d::mat4 gl::mProjection;
    k3d::vec3 gl::vLight0;
    k3d::mat4 gl::mNormal;


    void gl::initialize(GLuint MV, GLuint MVP, GLuint N, GLuint Light0)
    {
        gl::gmMV = MV;
        gl::gmMVP = MVP;
        gl::gmN = N;
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

} // namespace k3d

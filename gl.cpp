#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "k3d.h"

void checkGlError(const char *n)
{
// TODO implement me
}

namespace k3d {

    GLuint gl::gProgram = 0;
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

    const char * gl::readFile(const char *filename)
    {
        std::string strbuf;
        std::string line;
        std::ifstream in(filename);
        while (std::getline(in, line))
            strbuf.append(line + '\n');

        char *buf = new char[(strbuf.size() + 1) * sizeof(*buf)];
        strcpy(buf, strbuf.c_str());

        return buf;
    }

    GLuint gl::loadShader(GLenum shaderType, const char* pSource) {
        GLuint shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {
                    char* buf = (char*) malloc(infoLen);
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        printf("Could not compile shader %d:\n%s\n",
                                shaderType, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }



    GLuint gl::createProgram(const char* pVertexSource, const char* pFragmentSource) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
        if (!vertexShader) {
            return 0;
        }

        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!pixelShader) {
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            glAttachShader(program, pixelShader);
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength) {
                    char* buf = (char*) malloc(bufLength);
                    if (buf) {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        printf("Could not link program:\n%s\n", buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }


    void gl::initialize(const char *vsfilename, const char *fsfilename)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        gl::gProgram = createProgram(readFile(vsfilename), readFile(fsfilename));
        glUseProgram(gl::gProgram);

        gl::gvPosition = glGetAttribLocation(gProgram, "vPosition");
        gl::gvNormal = glGetAttribLocation(gProgram, "vNormal");
        gl::gmMV = glGetUniformLocation(gProgram, "mModelView");
        gl::gmN = glGetUniformLocation(gProgram, "mNormalMatrix");
        gl::gmMVP = glGetUniformLocation(gProgram, "mModelViewProjection");
        gl::gvLight0 = glGetUniformLocation(gProgram, "vLightSource0");
        gl::gvColor = glGetUniformLocation(gProgram, "vColor");

        glEnable(GL_DEPTH_TEST);
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

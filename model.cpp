#include "k3d.h"
#include "math.h"

#include <fstream>
#include <sstream>
#include <string>

namespace k3d {

    void model::draw(GLuint gvPositionHandle, GLuint gvNormalHandle)
    {
        glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, &(vs[0]));
//        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gvPositionHandle);
//        checkGlError("glEnableVertexAttribArray");

        glVertexAttribPointer(gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, &(ns[0]));
        //checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gvNormalHandle);

        glDrawElements(GL_TRIANGLES, faces.size()*3, GL_UNSIGNED_SHORT, &(faces[0]));
//        checkGlError("glDrawElements");
    }

    void model::clear()
    {
        vs.clear();
        ns.clear();
        faces.clear();
    }

    void model::computeNormals()
    {
        for (unsigned i = 0; i < vs.size(); i++) {
            int count = 0;
            ns.push_back(vec3(0.0, 0.0, 0.0));
            for (unsigned j = 0; j < faces.size(); j++) {
                face f = faces[j];
                if (f.a == i || f.b == i || f.c == i) {
                    vec3 n = cross(vs[f.c] - vs[f.b], vs[f.a] - vs[f.b]).normalize();
                    ns[i] = ns[i] + n;
                }
            }
        }
    }

    bool model::loadObj(const char *objfilename)
    {
        std::ifstream in(objfilename);
        std::string line;
        char ch;
        vec3 v;
        face f;

        if (!in.is_open())
            return false;

        clear();

        while(!in.eof()) {
            std::getline(in, line);
            std::istringstream iss(line, std::istringstream::in);

            iss >> ch;

            if (ch == 'v') {
                iss >> v.x;
                iss >> v.y;
                iss >> v.z;
                vs.push_back(v);
            } else if (ch == 'f') {
                iss >> f.a;
                iss >> f.b;
                iss >> f.c;
                f.a--;
                f.b--;
                f.c--;
                faces.push_back(f);
            }

            ch = '#';
        }

        computeNormals();

        return true;
    }
} //namespace k3d

//
// Created by 田淙宇 on 2019/9/22.
//

#ifndef GRENDERER_OBJLOADER_H
#define GRENDERER_OBJLOADER_H

#include <string>
#include <sstream>
#include "../GRObject.h"
using GRCore::GRObject;

static void LoadObj(const char*file,GRObject& obj){
    std::ifstream in;
    in.open (file, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            obj.AddVert(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            obj.AddNorm(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            obj.AddUv(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                obj.AddFace(tmp);
            }
            obj.mFaceCount++;
        }
    }
    std::cerr << "# v# " << obj.VertCount() << " f# "  << obj.FaceCount() << " vt# " << obj.UVCount()<< " vn# " << obj.NormCount() << std::endl;

}
#endif //GRENDERER_OBJLOADER_H

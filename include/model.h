#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "VBO.h"

class Model {
  public:
    std::string path;

  std::vector <Vertex> vertices;
  std::vector <GLuint> indices;

  // MODE
  // 0: Vertex indices (f v1 v2 v3 ...)
  // 1: Vertex texture coordinate indices (f v1/vt1 v2/vt2 v3/vt3 ...)
  // 2: Vertex normal indices (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...)
  // 3: Vertex normal indices without texture coordinate indices (f v1//vn1 v2//vn2 v3//vn3 ...)
  Model(std::string path, int mode);
};

#endif
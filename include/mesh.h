#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include "VAO.h"
#include "EBO.h"
#include "shader.h"

class Mesh {
  public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    Shader* shader;
  
  VAO VAO1;

  Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, Shader* shader);
  void ActivateShader();
  void Draw();
  void Delete();
};

#endif

#include "mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, Shader* shader) {
  Mesh::vertices = vertices;
  Mesh::indices = indices;
  Mesh::shader = shader;
  assert(Mesh::shader != NULL);

  VAO1.Bind();

  VBO VBO(vertices);
  EBO EBO(indices);

  VAO1.LinkAtt(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);
  VAO1.LinkAtt(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*) (3 * sizeof(GL_FLOAT)));
  VAO1.LinkAtt(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*) (6 * sizeof(GL_FLOAT)));

  // unbind: avoid accidentally modify buffer
  VAO1.Unbind();
  VBO.Unbind();
  EBO.Unbind(); // TODO: not sure why I have to unbind this after unbind VAO for something that stores in VAO
};

void Mesh::ActivateShader() {
  (*shader).Activate();
}

void Mesh::Draw() {
  VAO1.Bind();

  // specify:
  // primitive: GL_TRIANGLES (triangle)
  // how many vertex indices to draw (size of indices array)
  // datatype of indices array
  // index from
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete() {
  VAO1.Delete();
  // TODO: VBO, EBO memory leak?
  (*shader).Delete();
}
#include "VAO.h"

VAO::VAO() {
  // n: Specifies the number of vertex array object names to generate.
  glGenVertexArrays(1, &ID); // this must be done before initializing VBO and EBO
}

// layout: usually 0, for position of VBO in VAO
void VAO::LinkAtt(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
  VBO.Bind();
  // specify:
  // 1. position of vertex attribute in VAO
  // 2. how many values in each VBO (per vertex)
  // 3. value type
  // 4. if coordinates are integer
  // 5. stride (0 if tightly packed)
  // 6. offset, where it begins
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.Unbind();
}

void VAO::Bind() {
  glBindVertexArray(ID); // bind VAO
}

void VAO::Unbind() {
  glBindVertexArray(0);
}

void VAO::Delete() {
  glDeleteVertexArrays(1, &ID);
}
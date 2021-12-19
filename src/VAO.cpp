#include "VAO.h"

VAO::VAO() {
  glGenVertexArrays(1, &ID); // this must be done before initializing VBO and EBO
}

// layout: usually 0, for position of VBO in VAO
void VAO::LinkVBO(VBO VBO, GLuint layout) {
  VBO.Bind();
  // specify:
  // 1. position of vertex attribute in VAO
  // 2. how many values in each VBO (per vertex)
  // 3. value type
  // 4. if coordinates are integer
  // 5. stride (0 if tightly packed)
  // 6. offset, where it begins
  glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
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
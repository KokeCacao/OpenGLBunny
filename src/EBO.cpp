#include"EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr indices_size) {
  // ========== Create EBO (Index Buffer Object) ==========
  // GL_ELEMENT_ARRAY_BUFFER is used to indicate this buffer
  // contains the indices of each element in the "other"(GL_ARRAY_BUFFER) buffer
  glGenBuffers(1, &ID); // allocate 1 element
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // GL_ARRAY_BUFFER is the type for vertex buffer

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
}

void EBO::Bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
  glDeleteBuffers(1, &ID);
}
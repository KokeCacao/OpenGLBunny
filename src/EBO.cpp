#include"EBO.h"

EBO::EBO(std::vector<GLuint>& indices) {
  // ========== Create EBO (Index Buffer Object) ==========
  // GL_ELEMENT_ARRAY_BUFFER is used to indicate this buffer
  // contains the indices of each element in the "other"(GL_ARRAY_BUFFER) buffer
  // TODO: I think there can only be one bounded [GL_ELEMENT_ARRAY_BUFFER] at given time. But [GL_ELEMENT_ARRAY_BUFFER] and [GL_ARRAY_BUFFER] can coexist.
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
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
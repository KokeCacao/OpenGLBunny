#include"VBO.h"

VBO::VBO(std::vector<Vertex>& vertices) {
  // ========== Setting VBO ==========
  // vertex buffer object: a array of data about one aspect of geometry (vertex / vertex color / normal / etc). It is stored in VAO
  // If you still don't understand, watch first 3min of
  // This video [https://www.youtube.com/watch?v=WMiggUPst-Q&ab_channel=ThinMatrix]

  glGenBuffers(1, &ID); // allocate 1 element // TODO: what is this element

  // GL_ARRAY_BUFFER is the type for vertex buffer, this binds [ID] to a spot called [GL_ARRAY_BUFFER]
  glBindBuffer(GL_ARRAY_BUFFER, ID);

  // STREAM: vertices will be used a few times
  // STATIC: vertices will be used many times
  // DYNAMIC: vertices re-supplied or modified and will be used many times
  // DRAW: will be modified for drawing
  // READ: ?
  // COPY: ?
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
  glDeleteBuffers(1, &ID);
}
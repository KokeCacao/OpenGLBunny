#include<iostream>
#include "include/glad/glad.h"
#include<GLFW/glfw3.h>
#include<math.h>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Configuration
const uint WIDTH = 800;
const uint HEIGHT = 800;
const char *NAME = "Bunny";
const uint GLFW_MAJOR_VERSION = 4;
const uint GLFW_MINOR_VERSION = 6;

// define vertices to render
// Note: we use [GLfloat] instead of [float] due to versions of [OpenGL] may specify different precision
GLfloat vertices[] = {
  -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
  0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
  0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
};

// connect vertices to triangles
GLuint indices[] = {
  0, 1, 2,
};
uint INDICES_LEN = 3;

int main() {
  glfwInit();

  // Telling GLFW the library version stuff
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, NULL, NULL);

  // Error Handling
  if (window == NULL) {
    std::cout << "Failed to create GLfW winder" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Link window we have created to the screen
  glfwMakeContextCurrent(window);

  // ========== Load OpenGL ==========
  gladLoadGL();
  glViewport(0, 0, WIDTH, HEIGHT); // Now we have created a window, this specify which region of the window needs OpenGL

  // ========== Load Shader ==========
  Shader shaderProgram("./src/shader/default.vert", "./src/shader/default.frag");

  // ========== Create VAO, VBO, EBO ==========
  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkVBO(VBO1, 0);

  // unbind: avoid accidentally modify buffer
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind(); // TODO: not sure why I have to unbind this after unbind VAO for something that stores in VAO

  // ========== Main program ==========
  while (!glfwWindowShouldClose(window)) {
    // "Clearing" in this context means filling with some predefined values
    glClearColor(1.00f, 1.00f, 1.00f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Activate();
    VAO1.Bind();

    // specify:
    // primitive: GL_TRIANGLES (triangle)
    // starting index of vertices
    // how many vertices to draw
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // specify:
    // primitive: GL_TRIANGLES (triangle)
    // how many vertex indices to draw (size of indices array)
    // datatype of indices array
    // index from
    glDrawElements(GL_TRIANGLES, INDICES_LEN, GL_UNSIGNED_INT, 0);

    // swap back buffer with front buffer
    // so that image can display through
    glfwSwapBuffers(window);

    // get mouse click events, so window won't freeze
    glfwPollEvents();
    // std::cout << "Runing" << std::endl;
  }

  // ========== Clear Memory ==========
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();

  // clear and unlink window we have created
  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
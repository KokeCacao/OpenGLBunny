#include<iostream>
#include "include/glad/glad.h"
#include<GLFW/glfw3.h>
#include<math.h>

// Shader Code
const char* vertexCode = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentCode = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

// Configuration
const uint WIDTH = 800;
const uint HEIGHT = 800;
const char *NAME = "Bunny";

// define vertices to render
// Note: we use [GLfloat] instead of [float] due to versions of [OpenGL] may specify different precision
const GLfloat vertices[] = {
  -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
  0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
  0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
};

int main() {
  glfwInit();

  // Telling GLFW the library version stuff
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // 1 means 1 screen for the shader
  glShaderSource(vertexShader, 1, &vertexCode, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // 1 means 1 screen for the shader
  glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
  glCompileShader(fragmentShader);

  // attach shader to object called shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // clear memory
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // ========== Create VAO ==========
  GLuint VAO;
  glGenVertexArrays(1, &VAO); // this must be done before [glGenBuffers]

  // ========== Create VBO ==========
  // vertex buffer object: a array of data about one aspect of geometry (vertex / vertex color / normal / etc). It is stored in VAO
  // If you still don't understand, watch first 3min of
  // This video [https://www.youtube.com/watch?v=WMiggUPst-Q&ab_channel=ThinMatrix]
  GLuint VBO;
  glGenBuffers(1, &VBO); // allocate 1 element
  glBindVertexArray(VAO); // bind with VAO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is the type for vertex buffer
  // STREAM: vertices will be used a few times
  // STATIC: vertices will be used many times
  // DYNAMIC: vertices re-supplied or modified and will be used many times
  // DRAW: will be modified for drawing
  // READ: ?
  // COPY: ?
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // specify:
  // 1. position of vertex attribute in VAO
  // 2. how many values in each VBO (per vertex)
  // 3. value type
  // 4. if coordinates are integer
  // 5. stride
  // 6. offset, where it begins
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *) 0);
  glEnableVertexAttribArray(0); // 0 for position of VBO in VAO

  // OPTIONSAL: avoid accidentally modify buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  // ========== Main program ==========
  while (!glfwWindowShouldClose(window)) {
    // "Clearing" in this context means filling with some predefined values
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // specify:
    // primitive: GL_TRIANGLES (triangle)
    // starting index of vertices
    // how many vertices to draw
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap back buffer with front buffer
    // so that image can display through
    glfwSwapBuffers(window);

    // get mouse click events, so window won't freeze
    glfwPollEvents();
  }

  // ========== Clear Memory ==========
  // clear all buffer
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // clear and unlink window we have created
  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
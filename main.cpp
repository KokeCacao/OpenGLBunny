#include<iostream>
#include "include/glad/glad.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<math.h>

#include "camera.h"
#include "shader.h"
#include "mesh.h"

// Configuration
const uint WIDTH = 800;
const uint HEIGHT = 800;
const char *NAME = "Bunny";
const uint GLFW_MAJOR_VERSION = 4;
const uint GLFW_MINOR_VERSION = 6;

// define vertices to render
// Note: we use [GLfloat] instead of [float] due to versions of [OpenGL] may specify different precision
// =========== POSITION =========== // =========== NORMAL =========== // =========== COLOR =========== //
Vertex vertices[] = {
  Vertex{glm::vec3(-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
  Vertex{glm::vec3(0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
  Vertex{glm::vec3(0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
};

// connect vertices to triangles
GLuint indices[] = {
  0, 1, 2,
};

Vertex lightVertices[] = {
  Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)  },
  Vertex{glm::vec3(-0.1f, -0.1f, -0.1f) },
  Vertex{glm::vec3(0.1f, -0.1f, -0.1f)  },
  Vertex{glm::vec3(0.1f, -0.1f, 0.1f)   },
  Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)   },
  Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)  },
  Vertex{glm::vec3(0.1f, 0.1f, -0.1f)   },
  Vertex{glm::vec3(0.1f, 0.1f, 0.1f)    },
};
GLuint lightIndices[] = {
  0, 1, 2,
  0, 2, 3,
  0, 4, 7,
  0, 7, 3,
  3, 7, 6,
  3, 6, 2,
  2, 6, 5,
  2, 5, 1,
  1, 5, 4,
  1, 4, 0,
  4, 5, 6,
  5, 6, 7,
};
glm::vec4 lightColor = glm::vec4(1.0f, 0.9f, 0.9f, 1.0f);

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

  // ========== Load OpenGL ========== //
  gladLoadGL();
  glViewport(0, 0, WIDTH, HEIGHT); // Now we have created a window, this specify which region of the window needs OpenGL

  // ========== Load Shader ========== //
  Shader shaderProgram("./src/shader/default.vert", "./src/shader/default.frag");
  GLuint uniformModel = glGetUniformLocation(shaderProgram.ID, "model");
  GLuint uniformView = glGetUniformLocation(shaderProgram.ID, "view");
  GLuint uniformProj = glGetUniformLocation(shaderProgram.ID, "proj");
  GLuint uniformLightColor = glGetUniformLocation(shaderProgram.ID, "lightColor");
  GLuint uniformLightPos = glGetUniformLocation(shaderProgram.ID, "lightPos");
  GLuint uniformCameraPos = glGetUniformLocation(shaderProgram.ID, "cameraPos");

  Shader lightShaderProgram("./src/shader/light.vert", "./src/shader/light.frag");
  GLuint lightUniformModel = glGetUniformLocation(lightShaderProgram.ID, "model");
  GLuint lightUniformView = glGetUniformLocation(lightShaderProgram.ID, "view");
  GLuint lightUniformProj = glGetUniformLocation(lightShaderProgram.ID, "proj");
  GLuint lightUniformLightColor = glGetUniformLocation(lightShaderProgram.ID, "lightColor");

  // ========== Create VAO, VBO, EBO ========== //
  // vector(begin,end):复制[begin,end)区间内另一个数组的元素到vector中
  // https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html
  std::vector <Vertex> v(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
  std::vector <GLuint> i(indices, indices + (sizeof(indices) / sizeof(GLuint)));
  Mesh meshMesh = Mesh(v, i, &shaderProgram);

  std::vector <Vertex> vLight(lightVertices, lightVertices + (sizeof(lightVertices) / sizeof(Vertex)));
  std::vector <GLuint> iLight(lightIndices, lightIndices + (sizeof(lightIndices) / sizeof(GLuint)));
  Mesh lightMesh = Mesh(vLight, iLight, &lightShaderProgram);

  // Enable Depth Buffer so that triangle can be on top of each other
  glEnable(GL_DEPTH_TEST); // TODO: not sure why triangle disappear

  // ========== Camera ========== //
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
  glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f); // TODO: not sure why this
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  Camera camera(WIDTH, HEIGHT, position, orientation, up, 0.001f, 0.1f);

  // ========== Main program ========== //
  double prev_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    double current_time = glfwGetTime();
    double elapsed_time = glfwGetTime() - prev_time;
    prev_time = current_time;

    // ========== Background and Settings ========== //
    // "Clearing" in this context means filling with some predefined values
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ========== Camera and Send View/Proj Data to Shader ========== //
    camera.KeyboardControl(window);
    glm::mat4 view;
    glm::mat4 proj;
    camera.Matrix(90.0f, 0.01f, 100.0f, &view, &proj); // 0.01f is crucial for depth buffer

    // ========== Calculate Model Position and Rotation ========== //
    glm::mat4 lightModel = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
    model = glm::rotate(glm::translate(model, modelPos), glm::radians((float) current_time * 128.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // ========== Activate Shader ========== //
    meshMesh.ActivateShader();
    // ========== Send Model Data to Shader ========== //
    glUniform4f(uniformLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(uniformLightPos, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(uniformCameraPos, camera.Position.x, camera.Position.y, camera.Position.z);
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    camera.UpdateMatrix(view, proj, uniformView, uniformProj);
    // ========== Draw Stuff ========== //
    meshMesh.Draw();

    // ========== Activate Shader ========== //
    lightMesh.ActivateShader();
    // ========== Send Model Data to Shader ========== //
    glUniform4f(lightUniformLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniformMatrix4fv(lightUniformModel, 1, GL_FALSE, glm::value_ptr(lightModel));
    camera.UpdateMatrix(view, proj, lightUniformView, lightUniformProj);
    // ========== Draw Stuff ========== //
    lightMesh.Draw();

    // ========== Done ========== //
    // swap back buffer with front buffer
    // so that image can display through
    glfwSwapBuffers(window);

    // get mouse click events, so window won't freeze
    glfwPollEvents();
    // std::cout << "Runing" << std::endl;
  }

  // ========== Clear Memory ==========
  meshMesh.Delete();
  lightMesh.Delete();

  // clear and unlink window we have created
  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
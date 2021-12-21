#include<iostream>
#include "include/glad/glad.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<math.h>

#include "camera.h"
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
// =========== POSITION =========== // =========== COLOR =========== // =========== NORMAL =========== //
GLfloat vertices[] = {
  -0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,   1.0f, 0.0f, 0.0f,        0.0f, 0.0f, 1.0f,
  0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,   0.0f, 1.0f, 0.0f,        0.0f, 0.0f, 1.0f,
  0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   0.0f, 0.0f, 1.0f,        0.0f, 0.0f, 1.0f,
};

// connect vertices to triangles
GLuint indices[] = {
  0, 1, 2,
};

GLfloat lightVertices[] = {
  -0.1f, -0.1f, 0.1f,
  -0.1f, -0.1f, -0.1f,
  0.1f, -0.1f, -0.1f,
  0.1f, -0.1f, 0.1f,
  -0.1f, 0.1f, 0.1f,
  -0.1f, 0.1f, -0.1f,
  0.1f, 0.1f, -0.1f,
  0.1f, 0.1f, 0.1f,
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

  Shader lightShaderProgram("./src/shader/light.vert", "./src/shader/light.frag");
  GLuint lightUniformModel = glGetUniformLocation(lightShaderProgram.ID, "model");
  GLuint lightUniformView = glGetUniformLocation(lightShaderProgram.ID, "view");
  GLuint lightUniformProj = glGetUniformLocation(lightShaderProgram.ID, "proj");
  GLuint lightUniformLightColor = glGetUniformLocation(shaderProgram.ID, "lightColor");

  // ========== Create VAO, VBO, EBO ========== //
  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAtt(VBO1, 0, 3, GL_FLOAT, 9 * sizeof(GL_FLOAT), (void*) 0);
  VAO1.LinkAtt(VBO1, 1, 3, GL_FLOAT, 9 * sizeof(GL_FLOAT), (void*) (3 * sizeof(GL_FLOAT)));
  VAO1.LinkAtt(VBO1, 2, 3, GL_FLOAT, 9 * sizeof(GL_FLOAT), (void*) (6 * sizeof(GL_FLOAT)));

  // unbind: avoid accidentally modify buffer
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind(); // TODO: not sure why I have to unbind this after unbind VAO for something that stores in VAO

  // Enable Depth Buffer so that triangle can be on top of each other
  glEnable(GL_DEPTH_TEST); // TODO: not sure why triangle disappear

  VAO lightVAO;
  lightVAO.Bind();

  VBO lightVBO(lightVertices, sizeof(lightVertices));
  EBO lightEBO(lightIndices, sizeof(lightIndices));

  lightVAO.LinkAtt(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(GL_FLOAT), (void*) 0);

  lightVAO.Unbind();
  lightVBO.Unbind();
  lightEBO.Unbind();

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
    // printf("%f\n", (float) current_time);

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
    shaderProgram.Activate();
    // ========== Send Model Data to Shader ========== //
    glUniform4f(uniformLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(uniformLightPos, lightPos.x, lightPos.y, lightPos.z);
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    camera.UpdateMatrix(view, proj, uniformView, uniformProj);
    // ========== Draw Stuff ========== //
    VAO1.Bind();
    // specify:
    // primitive: GL_TRIANGLES (triangle)
    // how many vertex indices to draw (size of indices array)
    // datatype of indices array
    // index from
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);


    // ========== Activate Shader ========== //
    lightShaderProgram.Activate();
    // ========== Send Model Data to Shader ========== //
    glUniform4f(lightUniformLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniformMatrix4fv(lightUniformModel, 1, GL_FALSE, glm::value_ptr(lightModel));
    camera.UpdateMatrix(view, proj, lightUniformView, lightUniformProj);
    // ========== Draw Stuff ========== //
    lightVAO.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);


    // ========== Done ========== //
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
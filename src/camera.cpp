#include "camera.h"

Camera::Camera(int screen_width, int screen_height, glm::vec3 position, glm::vec3 orientation, glm::vec3 up, float speed, float sensitivity) {
  Camera::screen_width = screen_width;
  Camera::screen_height = screen_height;
  Camera::speed = speed;
  Camera::sensitivity = sensitivity;

  Position = position;
  Orientation = orientation;
  Up = up;
}

void Camera::UpdateMatrix(float FOV, float nearPlane, float farPlane, GLuint uniformView, GLuint uniformProj) {
  glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
  glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)(screen_width/screen_height), nearPlane, farPlane);

  // GL_FALSE: don't want OpenGL to transpose it
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void Camera::KeyboardControl(GLFWwindow* window) {
  printf("%f, %f, %f\n", Position.x, Position.y, Position.z);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Position += speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Position += speed * -glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Position += speed * -Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Position += speed * glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Position += speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    Position += speed * -Up;
  }
}
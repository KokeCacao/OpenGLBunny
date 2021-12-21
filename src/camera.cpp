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

void Camera::Matrix(float FOV, float nearPlane, float farPlane, glm::mat4* view, glm::mat4* proj) {
  *view = glm::lookAt(Position, Position + Orientation, Up);
  *proj = glm::perspective(glm::radians(FOV), (float)(screen_width/screen_height), nearPlane, farPlane);
}

void Camera::UpdateMatrix(glm::mat4 view, glm::mat4 proj, GLuint uniformView, GLuint uniformProj) {
  // GL_FALSE: don't want OpenGL to transpose it
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void Camera::KeyboardControl(GLFWwindow* window) {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    exit(0);
  }

  double mouseX;
  double mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  float rotX = sensitivity * (float)(mouseX - (screen_width / 2));
  float rotY = sensitivity * (float)(mouseY - (screen_height / 2));

  // choose 5.0f avoid camera jiggering when 90 degree up
  // TODO: not sure why X, Y inverted
  glm::vec3 _ = glm::rotate(Orientation, glm::radians(-rotY), glm::normalize(glm::cross(Orientation, Up)));
  if (glm::angle(_, Up) > glm::radians(5.0f) and glm::angle(_, -Up) > glm::radians(5.0f)) {
    Orientation = _;
  }
  Orientation = glm::rotate(Orientation, glm::radians(-rotX), Up);

  
  glfwSetCursorPos(window, screen_width / 2, screen_height / 2);
}
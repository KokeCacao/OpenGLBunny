#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

// Experimental Feature enable 3d rotation
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shader.h"

class Camera {
  public:
    glm::vec3 Position;
    glm::vec3 Orientation;
    glm::vec3 Up; // up vector

    int screen_width;
    int screen_height;

    float speed;
    float sensitivity;

    Camera(int screen_width, int screen_height, glm::vec3 position, glm::vec3 orientation, glm::vec3 up, float speed, float sensitivity);

    void UpdateMatrix(float FOV, float nearPlane, float farPlane, GLuint uniformView, GLuint uniformProj);
    void KeyboardControl(GLFWwindow* window);
};

#endif
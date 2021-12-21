#version 460 core
// TODO: take VBO at position 0? or VBO's position 0's position 0?
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 color;
out vec3 normal;
out vec3 crntPos;

// This [uniform] does not need input, like variable dirrectly from CPU (to GPU), not models
// You should never declear [uniform] is not used, otherwise it will be optimized and may cause issues
// For more about [uniform], see: https://www.youtube.com/watch?v=DE6Xlx_kbo0&ab_channel=TheCherno
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  gl_Position = proj * view * model * vec4(aPos, 1.0);

  color = aColor;
  crntPos = vec3(model * vec4(aPos, 1.0));
  normal = aNormal;
}

#version 460 core
// TODO: take VBO at position 0? or VBO's position 0's position 0?
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

// This [uniform] does not need input, like variable dirrectly from CPU (to GPU), not models
// You should never declear [uniform] is not used, otherwise it will be optimized and may cause issues
// For more about [uniform], see: https://www.youtube.com/watch?v=DE6Xlx_kbo0&ab_channel=TheCherno
uniform float scale;

void main() {
    gl_Position = vec4(scale * aPos.x, scale * aPos.y, scale * aPos.z, 1.0);
    color = aColor;
}

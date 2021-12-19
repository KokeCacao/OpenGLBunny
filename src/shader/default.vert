#version 460 core
// TODO: take VBO at position 0? or VBO's position 0's position 0?
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor;
}

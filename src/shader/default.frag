#version 460 core
out vec4 FragColor;

in vec3 color; // this name has to match output from [.vert]

uniform vec4 lightColor;

void main() {
    FragColor = vec4(color, 1.0f) * lightColor;
}

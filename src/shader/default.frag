#version 460 core
out vec4 FragColor;

in vec3 color; // this name has to match output from [.vert]
in vec3 normal;
in vec3 crntPos;

uniform vec4 lightColor;
uniform vec3 lightPos;

void main() {
  float ambient = 0.2f;
  vec3 lightDirection = normalize(lightPos - crntPos);

  float diffuse = max(dot(normalize(normal), lightDirection), 0.0f);
  FragColor = vec4(color, 1.0f) * lightColor * (diffuse + ambient);
}

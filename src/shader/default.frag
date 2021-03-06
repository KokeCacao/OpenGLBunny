#version 460 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec3 color; // this name has to match output from [.vert]

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {
  // ambient light
  float ambientIntensity = 0.2f;
  vec3 lightDirection = normalize(lightPos - crntPos);

  // specular light
  float specular = 0.5f;
  float decreaseSpeed = 5.0f;
  vec3 viewDirection = normalize(cameraPos - crntPos);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float specularIntensity = specular * pow(max(dot(viewDirection, reflectDirection), 0.0f), decreaseSpeed);

  // diffuse light
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  
  FragColor = vec4(color, 1.0f) * lightColor * (diffuse + ambientIntensity + specularIntensity);
}

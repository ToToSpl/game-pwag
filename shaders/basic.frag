#version 330 core
in vec2 UV;
in vec3 norm;
in vec3 FragPos;
out vec4 color;

uniform sampler2D myTextureSampler;
uniform vec3 V;
uniform int alive;
uniform vec3 lightDir;
uniform vec3 lightPoint;

float specularStrength = 0.9;
float ambientStrenght = 0.6;
float shininess = 0.6;

vec3 lightDirColor = vec3(0.95, 0.91, 0.60);
vec3 lightPointColor = vec3(0.9, 0.9, 1);


vec3 calcDirLight() {
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 viewDir = normalize(V - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32);

  return 0.4 * (ambientStrenght + diff + spec) * lightDirColor;
}

vec3 calcPointLight() {
  float diff = max(dot(norm, lightPoint), 0.0);
  vec3 viewDir = normalize(V - FragPos);
  vec3 lightDirection = lightPoint - FragPos;
  float lightLength = length(lightDirection);
  lightDirection = normalize(lightDirection);
  vec3 reflectDir = reflect(-lightDirection, norm);

  float spec = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 64);
  float attentuation = 1.0 / (0.01 * lightLength * lightLength + 0.3 * lightLength + 0.1);

  return (ambientStrenght + diff + spec) * attentuation  * lightPointColor;
}

void main(){
  vec3 objectColor = texture( myTextureSampler, UV ).rgb;

  if(alive == 1) {
    vec3 directionalLight = calcDirLight();
    vec3 pointLight = calcPointLight();
    color = vec4((directionalLight + pointLight) * objectColor, 1);
  } else {
    color = vec4(ambientStrenght * lightDirColor * objectColor, 0.5);
    color.r += 0.5;
  }
}

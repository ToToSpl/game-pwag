#version 330 core
in vec2 UV;
in vec3 norm;
in vec3 FragPos;
out vec3 color;

uniform sampler2D myTextureSampler;
uniform vec3 V;

float specularStrength = 0.5;
float ambientStrenght = 0.1;

vec3 light = vec3(100,100,100);
vec3 lightColor = vec3(0.95, 0.91, 0.60);


void main(){
  float diff = max(dot(norm, light), 0.0);

  vec3 viewDir = normalize(V - FragPos);
  vec3 reflectDir = reflect(-light, norm);  
  float spec = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 5);

  vec3 lighting = (ambientStrenght + diff + spec) * lightColor;
  vec3 objectColor = texture( myTextureSampler, UV ).rgb;

  color = lighting * objectColor;
}

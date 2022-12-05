#version 330 core
in vec2 UV;
in vec3 norm;
in vec3 FragPos;
out vec3 color;

uniform sampler2D myTextureSampler;
uniform vec3 V;
uniform int alive;

float specularStrength = 0.9;
float ambientStrenght = 0.6;

vec3 light = normalize(vec3(100,100,100));
vec3 lightColor = vec3(0.95, 0.91, 0.60);


void main(){
  float diff = max(dot(norm, light), 0.0);
  vec3 objectColor = texture( myTextureSampler, UV ).rgb;

  if(alive == 1) {
    vec3 viewDir = normalize(V - FragPos);
    vec3 reflectDir = reflect(-light, norm);  
    float spec = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 lighting = (ambientStrenght + diff + spec) * lightColor;
   
    color = lighting * objectColor;
  } else {
    color = ambientStrenght * lightColor * objectColor;
    color.r += 0.5;
  }
}

#version 330 core

in vec2 pos;
out vec4 color;

uniform float health;
uniform float t_elapsed;


void main(){
  if (health < 0.0) {
    color = vec4(1,0,0,1);
  }else if (health < 100.0) {
    float dist = distance(vec2(0), pos) / sqrt(2.0);
    float intensity = 1.0 * (exp((100.0 - health) / 100.0) - 1.0);
    intensity += 0.2 * (1.0 + sin(6.28 * 0.001 * t_elapsed));
    color = vec4(1,0,0,intensity*dist);
  } else {
    color = vec4(0);
  }
}

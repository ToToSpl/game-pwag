#version 330

layout(location = 0) in vec2 vertex;

out vec2 pos;

void main(){
  gl_Position = vec4(vertex.xy, 0, 1);
  pos = vertex;
}


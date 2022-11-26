#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm;

out vec2 UV;
out vec3 norm;
out vec3 FragPos;

uniform mat4 MVP;
uniform mat4 M;
uniform mat3 NormalMat;

void main(){
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  norm = NormalMat * vertexNorm;
  FragPos = (M * vec4(vertexPosition_modelspace,1)).xyz;
  UV = vertexUV;
}


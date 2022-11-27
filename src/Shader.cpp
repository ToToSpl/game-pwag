#include "Renderer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Game {

Shader compileShader(std::string filepath) {
  Shader shad;
  if (filepath.find(".vert") != std::string::npos) {
    shad.shaderId = glCreateShader(GL_VERTEX_SHADER);
    shad.type = shad.VERTEX;
  } else if (filepath.find(".frag") != std::string::npos) {
    shad.shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    shad.type = shad.FRAGMENT;
  } else {
    std::cout << "Incompatible shader passed!" << std::endl;
    abort();
  }

  std::string ShaderCode;
  std::ifstream ShaderStream(filepath, std::ios::in);
  if (ShaderStream.is_open()) {
    std::stringstream sstr;
    sstr << ShaderStream.rdbuf();
    ShaderCode = sstr.str();
    ShaderStream.close();
  } else {
    std::cout << "Shader: " << filepath << " not found!" << std::endl;
    abort();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  char const* SourcePointer = ShaderCode.c_str();
  glShaderSource(shad.shaderId, 1, &SourcePointer, NULL);
  glCompileShader(shad.shaderId);

  glGetShaderiv(shad.shaderId, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(shad.shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

  if (InfoLogLength > 0) {
    std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(shad.shaderId, InfoLogLength, NULL,
                       &ShaderErrorMessage[0]);
    printf("%s\n", &ShaderErrorMessage[0]);
    abort();
  }

  return shad;
}

ShaderProgram* compileProgram(std::string vertex, std::string fragment) {
  ShaderProgram* prog = new ShaderProgram;
  GLint Result = GL_FALSE;
  int InfoLogLength;

  prog->fragment = compileShader(fragment);
  prog->vertex = compileShader(vertex);

  prog->programId = glCreateProgram();
  glAttachShader(prog->programId, prog->vertex.shaderId);
  glAttachShader(prog->programId, prog->fragment.shaderId);
  glLinkProgram(prog->programId);

  glGetProgramiv(prog->programId, GL_LINK_STATUS, &Result);
  glGetProgramiv(prog->programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(prog->programId, InfoLogLength, NULL,
                        &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
    abort();
  }

  glDetachShader(prog->programId, prog->vertex.shaderId);
  glDetachShader(prog->programId, prog->fragment.shaderId);

  return prog;
}

} // namespace Game

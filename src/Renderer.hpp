#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace Game {

struct Shader {
  enum { VERTEX, FRAGMENT } type;
  GLuint shaderId;
};

Shader* compileShader(std::string filepath);

struct ShaderProgram {
  Shader *vertex, *fragment;
  GLuint programId;
};

ShaderProgram* compileProgram(Shader* vertex, Shader* fragment);

class Renderer {
private:
  GLFWwindow* _window;
  GLuint _VertexArrayID;
  bool _running = false;
  Shader *_stdShaderFrag, *_stdShaderVert;
  ShaderProgram* _stdShaderProg;

public:
  Renderer();
  ~Renderer();

public:
  bool init(std::string window_name);
  inline bool shouldRun() { return !glfwWindowShouldClose(_window); };
  float renderFrame();
};
} // namespace Game

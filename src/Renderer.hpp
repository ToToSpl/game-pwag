#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "EngineUniforms.hpp"
#include "Player.hpp"
#include "Scene.hpp"

namespace Game {

struct Shader {
  enum { VERTEX, FRAGMENT } type;
  GLuint shaderId;
};

Shader compileShader(std::string filepath);

struct ShaderProgram {
  Shader vertex, fragment;
  GLuint programId;
};

ShaderProgram* compileProgram(std::string vertex, std::string fragment);

class Renderer {
private:
  GLFWwindow* _window;
  GLuint _VertexArrayID;
  GLuint _currentProgID;
  bool _running = false;
  double _timeElapsed = 0.f;
  UniformsIDs _uniforms;
  GLuint _postEffectVertArr, _postEffectIndArr;
  glm::vec3 _lightDir, _lightPoint;
  float _lightPointMult;
  double _lightLastBlink = 0.f;
  unsigned int _lightPatternCurrent = 0;
  std::string _lightPattern;

  Scene& _scene;
  Player* _player;
  double _lastFrame;

private:
  ShaderProgram* _stdShaderProg;
  ShaderProgram* _wireframeProg;
  ShaderProgram* _postEffectProg;

public:
  Renderer(Scene& scene);
  ~Renderer();

private:
  void setupLight();

public:
  inline GLFWwindow* getWindow() { return _window; };
  inline void bindPlayer(Player* player) { _player = player; };

public:
  bool init(std::string window_name);
  inline void bindScene(Scene& scene) { _scene = scene; };
  inline bool shouldRun() { return !glfwWindowShouldClose(_window); };
  float renderFrame(float ts);
  void addDirectionalLight(glm::vec3 dir, float brightness);
  void addPointLight(glm::vec3 point, std::string pattern);
};
} // namespace Game

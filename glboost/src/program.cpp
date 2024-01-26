//  MIT License
//
//  Copyright (c) 2018 Francisco de Lanuza
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include "glboost/program.h"

#include <vector>
#include <stdexcept>

#include <logger.h>


using namespace glboost;

Program::Program() {
  _programID = glCreateProgram();
}

Program::Program(Shader& vertex_shader, Shader& fragment_shader) : Program() {
  // Validate shader types
  GLint type;
  glGetShaderiv(vertex_shader.id(), GL_SHADER_TYPE, &type);
  if(type != GL_VERTEX_SHADER)
    throw std::runtime_error("Invalid Shader: it should be a vertex shader");
  glGetShaderiv(fragment_shader.id(), GL_SHADER_TYPE, &type);
  if (type != GL_FRAGMENT_SHADER)
    throw std::runtime_error("Invalid Shader: it should be a fragment shader");

  // Attach shaders
  glAttachShader(_programID, vertex_shader.id());
  glAttachShader(_programID, fragment_shader.id());

  linkProgram();

  DebugLog("Renderer: CREATED");
}


Program::~Program() {
  glDeleteProgram(_programID);
  DebugLog("Renderer: DESTROYED");
}


void Program::attachShader(Shader &a_shader) {
  if(!_isLinked)
    glAttachShader(_programID, a_shader.id());
  else
    throw std::runtime_error("Cannot attach Shader: program is already linked");
}


void Program::linkProgram() {
  glLinkProgram(_programID);

  // Check the program
  GLint result { GL_FALSE };
  int info_log_length {0};
  glGetProgramiv(_programID, GL_LINK_STATUS, &result);
  glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> error_message(info_log_length + 1);
    glGetProgramInfoLog(_programID, info_log_length, NULL, &error_message[0]);

    if (result == GL_FALSE) {
      throw std::runtime_error("Error while linking programm. Error: " + std::string(&error_message[0]));
    }
  }

  _isLinked = true;

  // Detach shaders
  GLint shaders_count {0};
  glGetProgramiv(_programID, GL_ATTACHED_SHADERS, &shaders_count);
  std::vector<GLuint> shader_ids(shaders_count);
  glGetAttachedShaders(_programID, shaders_count, &shaders_count, &shader_ids[0]);
  for (auto shader : shader_ids) {
    glDetachShader(_programID, shader);
  }
}

GLuint Program::id() const{
  return _programID;
}

bool Program::isLinked() const{
  return _isLinked;
}


void Program::useProgram() const{
  glUseProgram(_programID);
}


void Program::releaseProgram() const{
  glUseProgram(0);
}

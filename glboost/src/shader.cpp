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

#include "glboost/shader.h"

#include <fstream>
#include <vector>
#include <stdexcept>

using namespace glboost;

Shader::Shader(GLenum type) {
  _shaderID = glCreateShader(type);
}

Shader::~Shader() {
  glDeleteShader(_shaderID);
}

GLuint Shader::id() const {
  return _shaderID;
}

void Shader::loadSource(const std::string &shader_file) {
  // Read the shader code from the file
  std::string shader_code;
  std::ifstream shader_stream(shader_file, std::ios::in);
  if (shader_stream.is_open()) {
    std::string line = "";
    while (getline(shader_stream, line)) {
      shader_code += "\n" + line;
    }
    shader_stream.close();
  }
  else {
    throw std::runtime_error("Cannot open shader file: " + shader_file);
  }

  loadSource(shader_code.c_str());
}

void Shader::loadSource(const char *shader_str) {
  glShaderSource(_shaderID, 1, &shader_str, NULL);
  glCompileShader(_shaderID);

  // Check Shader
  GLint result{ GL_FALSE };
  int info_log_length {0};
  glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &result);
  glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> error_message(info_log_length + 1);
    glGetShaderInfoLog(_shaderID, info_log_length, NULL, &error_message[0]);

    if (result == GL_FALSE) {
      throw std::runtime_error("Error while compiling shader:\n" + std::string(shader_str) + "\nCompilation error: " + &(error_message[0]) );
    }
  }
}

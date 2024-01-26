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

#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>

#include <GL/glew.h>

#include "glboost/shader.h"

namespace glboost {
  class Program
  {
  public:
    Program();
    Program(Shader& vertex_shader, Shader& fragment_shader);
    ~Program();

    Program(const Program& other) = delete;
    Program(Program&& other) = delete;
    Program& operator=(const Program& other) = delete;
    Program& operator=(const Program&& other) = delete;

    void attachShader(Shader &aShader);

    void linkProgram();

    GLuint id() const;

    bool isLinked() const;

    void useProgram() const;

    void releaseProgram() const;

  protected:
    GLuint   _programID  {0};
    bool     _isLinked   {false};

  };

}


#endif // PROGRAM_H

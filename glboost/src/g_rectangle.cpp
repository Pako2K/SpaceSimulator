#include <glboost/g_rectangle.h>

#include <memory>

#include <logger.h>
#include <geometry/basic_types.h>


#ifdef DEBUG_SHADER
static const std::string __VERT_SHADER_SRC__{ "shaders/simplest_vs.glsl" };
static const std::string __FRAG_SHADER_SRC__{ "shaders/unif_color_fs.glsl" };
#else
#include "../shaders/simplest_vs.glsl.h"
#include "../shaders/unif_color_fs.glsl.h"
#endif


using namespace glboost;
using namespace geometry;


void gRectangle::createRenderer() {
  if (_renderer_ptr.get()) return;

  Shader vertex_shader(GL_VERTEX_SHADER);
  Shader fragment_shader(GL_FRAGMENT_SHADER);
  try {
    vertex_shader.loadSource(__VERT_SHADER_SRC__);
    fragment_shader.loadSource(__FRAG_SHADER_SRC__);
    _renderer_ptr = std::make_unique<Program>(vertex_shader, fragment_shader);
  }
  catch (std::string &exc) {
    ErrorLog("ERROR: Cannot create GL program for the Rectangle renderer");
    throw exc;
  }

  DebugLog("Rectangle Renderer: CREATED");
}


gRectangle::gRectangle(const Position2D& pos, const Size2D& size) : _position{ pos }, _size{ size } {
  createRenderer();
    
  // Generate VAO's and VBO's
  glGenVertexArrays(1, &_vertex_array);
  glBindVertexArray(_vertex_array);

  glGenBuffers(1, &_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
  
  GLuint vPosition = glGetAttribLocation(_renderer_ptr->id(), "vPosition");
  const GLint VERTEX_NUM_ELEM{ 2 };
  const GLsizei SIZE_BYTES{ VERTEX_NUM_ELEM * sizeof(float) };
  glVertexAttribPointer(vPosition, VERTEX_NUM_ELEM, GL_FLOAT, GL_FALSE, SIZE_BYTES /*stride*/, (void*)0 /*offset*/);
  glEnableVertexAttribArray(vPosition);
    
  uploadVertices();

  glGenBuffers(1, &_elements_buffer);
  std::array<uint8_t, 24> elements{ 0, 4, 1, 4, 1, 5,
                                    1, 5, 2, 5, 2, 6,
                                    2, 6, 3, 6, 3, 7,
                                    3, 7, 0, 7, 0, 4 };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  // Get Uniform and set default value 
  _uColor = glGetUniformLocation(_renderer_ptr->id(), "uColor");  
}


gRectangle::~gRectangle() {
  glDeleteBuffers(1, &_vertex_buffer);
  glDeleteBuffers(1, &_elements_buffer);
  glDeleteVertexArrays(1, &_vertex_array);
}


void gRectangle::position(const Position2D& new_pos) {
  _position = new_pos; 
  uploadVertices();
}


void gRectangle::size(const Size2D& new_size) {
  _size = new_size; 
  uploadVertices();
}


void gRectangle::lineWidth(SizePxl new_width) { 
  _line_width = std::min(new_width, SizePxl(std::min(_size.w(), _size.h()) / 2));
  _line_width = std::max(_line_width, static_cast<SizePxl>(1));

  uploadVertices();
}


void gRectangle::border(bool flag) {
  // This ckheck is important: if border is re-enabled, the line width is set to default value (=1)
  if (_border != flag) {
    _border = flag;
    if (_border) {
      _line_width = 1;
      if (_fill)
        draw_fn_ptr = &gRectangle::drawAll;
      else
        draw_fn_ptr = &gRectangle::drawBorder;
    }
    else {
      _line_width = 0;
      if (_fill)
        draw_fn_ptr = &gRectangle::drawFilling;
      else
        draw_fn_ptr = &gRectangle::drawNothing;
    }
    uploadVertices();
  }
}


void gRectangle::fill(bool flag) { 
  _fill = flag; 
  if(_fill & _border)
    draw_fn_ptr = &gRectangle::drawAll;
  else if(_border)
    draw_fn_ptr = &gRectangle::drawBorder;
  else if (_fill)
    draw_fn_ptr = &gRectangle::drawFilling;
  else {
    draw_fn_ptr = &gRectangle::drawNothing;
  }
}


void gRectangle::drawBorder() {
  glUniform4fv(_uColor, 1, _border_color.data());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elements_buffer);
  glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, (void*) 0);
}


void gRectangle::drawFilling() {
  glUniform4fv(_uColor, 1, _fill_color.data());
  glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
}


void gRectangle::drawAll() {
  drawFilling();
  drawBorder();
}


void gRectangle::uploadVertices() {
  // Calculate Vertices 
  /*float up_border = float(_position.y());
  float down_border = float(_position.y() + _size.h());
  float left_border = float(_position.x());
  float right_border = float(_position.x() + _size.w());
  float up_filling = float(_position.y() + _line_width);
  float down_filling = float(_position.y() + _size.h() - _line_width);
  float left_filling = float(_position.x() + _line_width);
  float right_filling = float(_position.x() + _size.w() - _line_width);

  std::array<Point<2, float>, 8> vertices{ { {left_border, up_border},
                                             {left_border, down_border},
                                             {right_border, down_border},
                                             {right_border, up_border},
                                             {left_filling, up_filling},
                                             {left_filling, down_filling},
                                             {right_filling, down_filling},
                                             {right_filling, up_filling}  } };*/

  //GLint viewport[4];
  //glGetIntegerv(GL_VIEWPORT, viewport);
  NormalizedCoordinates::viewportSize();
  NormalizedCoordinates up_left{ _position };
  NormalizedCoordinates down_right{ _position + _size };
  NormalizedCoordinates up_left_filling{ _position + Size2D(_line_width, _line_width) };
  NormalizedCoordinates down_right_filling{ _position + (_size - Size2D(_line_width, _line_width)) };

  std::array<std::array<float, 2>, 8> vertices{ { {up_left.x(), up_left.y()},
                                                  {up_left.x(), down_right.y()},
                                                  {down_right.x(), down_right.y()},
                                                  {down_right.x(), up_left.y()},
                                                  {up_left_filling.x(), up_left_filling.y()},
                                                  {up_left_filling.x(), down_right_filling.y()},
                                                  {down_right_filling.x(), down_right_filling.y()},
                                                  {down_right_filling.x(), up_left_filling.y()} } };



  //// Transform from window coordinates to normalized coordinates
  //for (auto& vertex : vertices) {
  //  vertex.x() = 2.0f * vertex.x() / viewport[2] - 1;
  //  vertex.y() = -2.0f * vertex.y() / viewport[3] + 1;
  //}

  // Upload new vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data()->data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
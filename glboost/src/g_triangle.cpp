#include <glboost/g_triangle.h>

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



void gTriangle::createRenderer() {
  if (_renderer_ptr.get()) return;

  Shader vertex_shader(GL_VERTEX_SHADER);
  Shader fragment_shader(GL_FRAGMENT_SHADER);
  try {
    vertex_shader.loadSource(__VERT_SHADER_SRC__);
    fragment_shader.loadSource(__FRAG_SHADER_SRC__);
    _renderer_ptr = std::make_unique<Program>(vertex_shader, fragment_shader);
  }
  catch (std::string & exc) {
    ErrorLog("ERROR: Cannot create GL program for the Rectangle renderer");
    throw exc;
  }

  DebugLog("Rectangle Renderer: CREATED");
}


gTriangle::gTriangle(const Position2D& pos1, const Position2D& pos2, const Position2D& pos3) : _positions{ pos1, pos2, pos3 } {
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

  glBindVertexArray(0);

  // Get Uniform and set default value 
  _uColor = glGetUniformLocation(_renderer_ptr->id(), "uColor");
}


gTriangle::~gTriangle() {
  glDeleteBuffers(1, &_vertex_buffer);
  glDeleteVertexArrays(1, &_vertex_array);
}

void gTriangle::vertex(size_t pos, const Position2D& new_pos) {
  _positions.at(pos) = new_pos;
  uploadVertices();
}

void gTriangle::lineWidth(SizePxl new_width) {
  int line_width_range[2];
  glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, line_width_range);

  _line_width = std::min(new_width, SizePxl(line_width_range[1]));
}


void gTriangle::border(bool flag) {
  // This ckheck is important: if border is re-enabled, the line width is set to default value (=1)
  _border = flag;
  if (_border && _fill) 
    draw_fn_ptr = &gTriangle::drawAll;
  else if (_border)
    draw_fn_ptr = &gTriangle::drawBorder;
  else if (_fill)
    draw_fn_ptr = &gTriangle::drawFilling;
  else
    draw_fn_ptr = &gTriangle::drawNothing;
}


void gTriangle::fill(bool flag) {
  _fill = flag;
  if (_fill & _border)
    draw_fn_ptr = &gTriangle::drawAll;
  else if (_border)
    draw_fn_ptr = &gTriangle::drawBorder;
  else if (_fill)
    draw_fn_ptr = &gTriangle::drawFilling;
  else 
    draw_fn_ptr = &gTriangle::drawNothing;
}


void gTriangle::drawBorder() {
  glUniform4fv(_uColor, 1, _border_color.data());
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(_line_width);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glLineWidth(1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void gTriangle::drawFilling() {
  glUniform4fv(_uColor, 1, _fill_color.data());
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


void gTriangle::drawAll() {
  drawFilling();
  drawBorder();
}


void gTriangle::uploadVertices() {
  // Calculate Vertices 

  std::array<Point<float, 2>, 3> vertices{ { {float(_positions[0].x()), float(_positions[0].y())},
                                             {float(_positions[1].x()), float(_positions[1].y())},
                                             {float(_positions[2].x()), float(_positions[2].y())} } };

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Transform from window coordinates to normalized coordinates
  for (auto& vertex : vertices) {
    vertex.x() = 2.0f * vertex.x() / viewport[2] - 1;
    vertex.y() = -2.0f * vertex.y() / viewport[3] + 1;
  }

  // Upload new vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data()->data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
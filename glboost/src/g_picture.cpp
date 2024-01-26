#include <glboost/g_picture.h>

#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <logger.h>


#ifdef DEBUG_SHADER
static const std::string __VERT_SHADER_SRC__{ "shaders/texture_vs.glsl" };
static const std::string __FRAG_SHADER_SRC__{ "shaders/texture_fs.glsl" };
#else
#include "../shaders/texture_vs.glsl.h"
#include "../shaders/texture_fs.glsl.h"
#endif


using namespace glboost;


void gPicture::createRenderer() {
  if (_renderer_ptr.get()) return;

  try {
    Shader vertex_shader(GL_VERTEX_SHADER);
    Shader fragment_shader(GL_FRAGMENT_SHADER);
    vertex_shader.loadSource(__VERT_SHADER_SRC__);
    fragment_shader.loadSource(__FRAG_SHADER_SRC__);
    _renderer_ptr = std::make_unique<Program>(vertex_shader, fragment_shader);
  }
  catch (std::string &exc) {
    ErrorLog("ERROR: Cannot create GL program for the Texture renderer");
    throw exc;
  }

  DebugLog("Rectangle Renderer: CREATED");
}


gPicture::gPicture(const Position2D& pos, const std::string& img_file, bool keep_aspect) : gPicture{ pos, {0, 0}, img_file, keep_aspect } {}


gPicture::gPicture(const Position2D& pos, const Size2D& size, const std::string& img_file, bool keep_aspect) : _position{ pos }, _size{ size }, _keep_aspect{ keep_aspect } {
  createRenderer();

  // Get Uniform and set default value 
  _uAlfa = glGetUniformLocation(_renderer_ptr->id(), "uAlfa");

  // Generate texture
  glGenTextures(1, &_texture_id);
  
  auto img_size = image(img_file);
  // If size is not specified, take the size of the picture
  if (_size.w() == 0 && _size.h() == 0)
    _size = img_size;

  // Recalculate size
  if (_keep_aspect) {
    if ((_size.w() / _size.h()) > (img_size.w() / img_size.h()))
      _size.w() = _size.h() * (img_size.w() / img_size.h());
    else
      _size.h() = _size.w() / (img_size.w() / img_size.h());
  }
    
  // Generate VAO's and VBO's
  glGenVertexArrays(1, &_vertex_array);
  glBindVertexArray(_vertex_array);

  glGenBuffers(1, &_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);

  GLuint vPosition = glGetAttribLocation(_renderer_ptr->id(), "vPosition");
  const GLint POS_NUM_ELEM{ 2 };
  const GLsizei SIZE_BYTES{ POS_NUM_ELEM * sizeof(float) };
  glVertexAttribPointer(vPosition, POS_NUM_ELEM, GL_FLOAT, GL_FALSE, 2 * SIZE_BYTES /*stride*/, (void*)0 /*offset*/);
  glEnableVertexAttribArray(vPosition);

  GLuint vTexCoord = glGetAttribLocation(_renderer_ptr->id(), "vTexCoord");
  glVertexAttribPointer(vTexCoord, POS_NUM_ELEM, GL_FLOAT, GL_FALSE, 2 * SIZE_BYTES /*stride*/, (void*)(SIZE_BYTES)/*offset*/);
  glEnableVertexAttribArray(vTexCoord);

  uploadVertices();

  glBindVertexArray(0);
}


gPicture::~gPicture() {
  glDeleteTextures(1, &_texture_id);
  glDeleteBuffers(1, &_vertex_buffer);
  glDeleteVertexArrays(1, &_vertex_array);
}


void gPicture::position(const Position2D& new_pos) {
  _position = new_pos; 
  uploadVertices();
}


void gPicture::size(const Size2D& new_size) {
  // Recalculate size
  if (_keep_aspect) {
    if ((new_size.w() / new_size.h()) > (_size.w() / _size.h())) {
      _size.w() = new_size.h() * (_size.w() / _size.h());
      _size.h() = new_size.h();
    }
    else {
      _size.h() = new_size.w() / (_size.w() / _size.h());
      _size.w() = new_size.w();
    }
  }
  uploadVertices();
}


void gPicture::opaqueness(float alfa) {
  _alfa = alfa;
}


Size2D gPicture::image(const std::string& img_file) {
  glBindTexture(GL_TEXTURE_2D, _texture_id);

  int width, height, num_channels;
  unsigned char *image = stbi_load(img_file.data(), &width, &height, &num_channels, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Store the texture with Alfa value!
  if (num_channels == 4)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  stbi_image_free(image);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Size2D(width, height);
}


void gPicture::draw() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  _renderer_ptr->useProgram();
  glUniform1f(_uAlfa, _alfa);
  glBindVertexArray(_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisable(GL_BLEND);
}


void gPicture::uploadVertices() {
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Transform functions from window coordinates to normalized coordinates
  std::function<float(int)> tranf_x = [=](int coord) {return 2.0f * coord / viewport[2] - 1; };
  std::function<float(int)> tranf_y = [=](int coord) {return -2.0f * coord / viewport[3] + 1; };

  // Calculate Vertices 
  float up_border = tranf_y(_position.y());
  float down_border = tranf_y(_position.y() + _size.h());
  float left_border = tranf_x(_position.x());
  float right_border = tranf_x(_position.x() + _size.w());

  std::array<float, 16> vertices{ left_border, up_border, 0, 0,
                                 left_border, down_border, 0, 1.0f,
                                 right_border, down_border, 1.0f, 1.0f,
                                 right_border, up_border, 1.0f, 0 };

  // Upload new vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
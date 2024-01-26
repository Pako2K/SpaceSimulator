#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <glboost/program.h>
#include <glboost/shader.h>

#include <glboost/graphic_types.h>
#include <glboost/font.h>


namespace glboost
{
  class TextRenderer
  {
    public:
      static TextRenderer& getRenderer();
      void release();

      /**
        *  Draw a text
        */
      void draw(const std::string &text, const Font &font, const Position2D &position, const Color4i &color);

      TextRenderer(const TextRenderer &other) = delete;
      TextRenderer& operator=(const TextRenderer &other) = delete;

      SizePxl sizePxl(const std::string &text, const Font& font) const;

    protected:
      static TextRenderer *_text_renderer;

      GLuint _vao;
      GLuint _vbuffer;

      std::unique_ptr<Program> _program;

      GLint _uTextColor;

      TextRenderer();

      ~TextRenderer();

    private:
      static size_t _clients;
      struct vertex
      {
        GLshort pos[2]; // vertex position
        GLfloat text[2]; // texture coordinates
      };
      const GLsizei VERTEX_SIZE {sizeof(vertex::pos) + sizeof(vertex::text)};
  };
} // END namespace gltext

#endif // TEXT_RENDERER_H

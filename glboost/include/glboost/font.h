#ifndef FONT_H
#define FONT_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <memory>

#ifndef GLEW_STATIC
  #define GLEW_STATIC
#endif // GLEW_STATIC

#include <GL/glew.h>

#include <glboost/graphic_types.h>


namespace glboost
{
  class Font
  {
    public:
      // Character code (limited to UTF-8)
      using CharCode = uint8_t;

      struct Glyph
      {
        // Coordinates of the quad, relative to the text baseline = (font baseline - font descender)
        Position2D   quad_coord[4];
        // Coordinates in the font texture
        GLfloat      text_coord[4][2];
        // Offset to advance to next glyph
        Position2D   advance;
      };

      static void     addFontsDir   (const std::string& fonts_dir);
      static void     fontsFileExt  (const std::string& ext);
      static Font&    get           (const std::string& font_name, SizePxl font_size);

      void release();

      // Destructor (public so it can be called from the caching map)
       ~Font();

       Font(const Font &other) = delete;
       Font& operator=(const Font &other) = delete;

      inline GLuint textureId() const { return _texture_id; }

      inline const Font::Glyph& glyph(CharCode chr_code) const {
        return _glyphs.at(chr_code);
      }


    protected:
      static std::map<std::string, std::unique_ptr<Font>> _fonts_cache;
      static std::map<std::string, uint8_t>               _fonts_clients;
      static std::queue<std::string>                      _deletion_candidates;

      std::string  _name;
      SizePxl      _size;
      GLuint       _texture_id;

      std::map<CharCode, Font::Glyph> _glyphs;

      // Constructor
      Font(const std::string& font_name, SizePxl font_size);

      inline static std::string uid(const std::string& font_name, SizePxl font_size) {
        return font_name + std::to_string(font_size);
      }

      inline std::string uid() {
        return uid(_name, _size);
      }


    private:
      static std::vector<std::string> _font_dirs;

      static std::string              _font_file_ext;
  };

} // END namespace glboost

#endif // FONT_H

#include <glboost/font.h>

#include <limits>
#include <stdexcept>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <logger.h>


#define MIN_CHAR_CODE 32
#define MAX_CHAR_CODE ( std::numeric_limits<CharCode>::max() )

#if defined(_WIN32) || defined(_WIN64)
  #define FONTS_DIR "C:/Windows/Fonts/"
#else
  #define FONTS_DIR "/usr/share/fonts/truetype/"
#endif

#define FONT_EXT    ".ttf"

#define MAX_CACHE_SIZE 10


using namespace glboost;

std::vector<std::string>        Font::_font_dirs {FONTS_DIR};
std::string                     Font::_font_file_ext {FONT_EXT};
std::map<std::string, std::unique_ptr<Font>>   Font::_fonts_cache;
std::map<std::string, uint8_t>                 Font::_fonts_clients;
std::queue<std::string>                        Font::_deletion_candidates;

void Font::addFontsDir(const std::string& fonts_dir) {
  _font_dirs.push_back(fonts_dir);
}

void Font::fontsFileExt(const std::string& ext) {
  _font_file_ext = ext;
}

Font& Font::get(const std::string& font_name, SizePxl font_size) {
  //glFont *font;
  std::string font_uid { uid(font_name, font_size) };

  //Check whether the font is already loaded in the cache
  auto map_it { _fonts_cache.find(font_uid) };
  if ( map_it != _fonts_cache.end() ) {
    _fonts_clients[font_uid]++;
  }
  // If not, create the new font, if there is still place in the cache
  else if ( _fonts_cache.size() < MAX_CACHE_SIZE ) {
    _fonts_cache.insert(std::make_pair(font_uid,  std::unique_ptr<Font>( new Font(font_name, font_size) )));
    _fonts_clients.insert(std::make_pair(font_uid, uint8_t(1)));
    map_it = _fonts_cache.find(font_uid);
  }
  else {
    // Check if there are cndidates for deletion
    while ( !_deletion_candidates.empty() ) {
      // delete the font if it was not already deleted and has no clients
      auto to_delete_ptr = _fonts_clients.find(_deletion_candidates.front());
      if ( to_delete_ptr != _fonts_clients.end() && to_delete_ptr->second == 0 ) {
        _fonts_cache.erase(_deletion_candidates.front());
        _fonts_clients.erase(_deletion_candidates.front());
        _deletion_candidates.pop();
        // and add the new font
        _fonts_cache.insert(std::make_pair(font_uid,  std::unique_ptr<Font>( new Font(font_name, font_size) )));
        _fonts_clients.insert(std::make_pair(font_uid, uint8_t(1)));
        map_it = _fonts_cache.find(font_uid);
        break;
      }
      else {
        // Remove from the deletion candidates queue
        _deletion_candidates.pop();
      }
    }
  }

  if ( map_it == _fonts_cache.end() )
    throw std::runtime_error("FONT CANNOT BE ALLOCATED. FONT CACHE IS FULL AND IN USE.");
  else
    return *map_it->second;
}


void Font::release() {
  _fonts_clients[uid()]--;
  if ( _fonts_clients[uid()] == 0 ) {
    _deletion_candidates.push(uid());
  }
}


Font::Font(const std::string& font_name, SizePxl font_size) : _name{font_name}, _size{font_size} {
  DebugLog("glFont CONSTRUCTOR CALLED " << font_name << " - " << font_size);

  // Initialize Freetype
  FT_Library ft_lib;
  if (FT_Init_FreeType(&ft_lib))
    throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");


  FT_Face face;
  FT_Error error;
  uint8_t indx {0};
  do{
    error = FT_New_Face(ft_lib, std::string(_font_dirs[indx] + font_name + _font_file_ext).data(), 0, &face);
    indx++;
  } while ( error == FT_Err_Cannot_Open_Resource && indx < _font_dirs.size() );

  if ( error ) {
    if ( error == FT_Err_Cannot_Open_Resource)
      throw std::runtime_error("Font file cannot be opened: " + _font_dirs[--indx] + font_name + _font_file_ext);
    else if ( error == FT_Err_Unknown_File_Format )
      throw std::runtime_error("Unknown font file format");
    else
      throw std::runtime_error("Unknown error in Font file");
  }

  if ( FT_Set_Pixel_Sizes(face, 0, font_size) )
    throw std::runtime_error("Font Size not allowed");


  // Create ONE texture for all the glyphs. The height of the texture is the maximum height of the font.
  // Each glyph bitmap is aligned at the top of the texture, and follows the previous glyph bitmap.
  // For each glyph it is necessary to store additionally the relative coordinates of its bitmap within the texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction --> maybe this should be in the read method?????

  //    1 - Reserve memory for the font bitmap: use the max glyph height as vertical size and calculate the total width
  //        by adding the width of each glyph
  uint16_t texture_height { 0 };
  size_t   texture_width  { 0 };
  PositionPxl  descender { 0 };
  FT_ULong num_glyphs = std::min(FT_ULong(face->num_glyphs), FT_ULong(MAX_CHAR_CODE));
  for (FT_ULong chr = MIN_CHAR_CODE; chr <= num_glyphs; chr++) {
    // Load character glyph
    if ( !FT_Load_Char(face, chr, FT_LOAD_RENDER) ) {
      texture_width += face->glyph->bitmap.width;
      texture_height = std::max(texture_height, decltype(texture_height)(face->glyph->bitmap.rows));
    }
    // Store Descender
    descender = std::max(descender, PositionPxl(face->glyph->bitmap.rows - face->glyph->bitmap_top));
  }

  // Create 2D texture allocating only the memory
  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glTexImage2D( GL_TEXTURE_2D,
                0, // level
                GL_RED,
                static_cast<GLsizei>(texture_width),
				static_cast<GLsizei>(texture_height),
                0, // border width (This value must be 0)
                GL_RED,
                GL_UNSIGNED_BYTE,
                nullptr );
  // Set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Horizontal position of each glyph within the texture, in pixels
  size_t text_pos_X { 0 };
  // Store the glyphs information
  for (FT_ULong chr = MIN_CHAR_CODE; chr <= num_glyphs; chr++) {
    // Load character glyph
    if (FT_Load_Char(face, chr, FT_LOAD_RENDER)) {
      DebugLog("ERROR: Failed to load Glyph " << chr << " (" << font_name << ")");
      _glyphs.insert(std::pair<CharCode, Font::Glyph>(static_cast<CharCode>(chr), Font::Glyph { { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
                                                                                { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
                                                                                { 0, 0 } } ));
      continue;
    }

    // Store the glyph in the texture
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, // level
					static_cast<GLint>(text_pos_X), //xoffset​
                    0, // yoffset​
                    face->glyph->bitmap.width, // width​
                    face->glyph->bitmap.rows, // height​
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer );

    // Texture coord.
    GLfloat text_X_left   = GLfloat(text_pos_X) / texture_width;
    GLfloat text_X_right  = text_X_left + GLfloat(face->glyph->bitmap.width) / texture_width;
    GLfloat text_Y_top    = GLfloat(face->glyph->bitmap.rows) / texture_height;
    // Quad coord.
    PositionPxl quad_X_left    = PositionPxl(face->glyph->bitmap_left);
    PositionPxl quad_X_right   = quad_X_left + PositionPxl(face->glyph->bitmap.width);
    // Origin of coordinates : Upper Left Corner. The positive y points downwards in the screen
    PositionPxl quad_Y_bottom = - descender - PositionPxl(face->glyph->bitmap_top);
    PositionPxl quad_Y_top    = quad_Y_bottom + PositionPxl(face->glyph->bitmap.rows);

    Font::Glyph character = { { { quad_X_left, quad_Y_bottom },
                                  { quad_X_left, quad_Y_top },
                                  { quad_X_right, quad_Y_top },
                                  { quad_X_right, quad_Y_bottom }
                                },
                                { { text_X_left, 0 },
                                  { text_X_left, text_Y_top },
                                  { text_X_right, text_Y_top },
                                  { text_X_right, 0 } },
                                { PositionPxl(face->glyph->advance.x >> 6), PositionPxl(face->glyph->advance.y >> 6) } };

    _glyphs.insert(std::pair<CharCode, Font::Glyph>(static_cast<CharCode>(chr), character));

    // Set new position in the buffer for next glyph
    text_pos_X += face->glyph->bitmap.width;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  FT_Done_Face(face);
  FT_Done_FreeType(ft_lib);
}


Font::~Font() {
  DebugLog("glFont DESTROYED " << _name << " - " << _size);

  //Delete font texture
  glDeleteTextures(1, &_texture_id);
}



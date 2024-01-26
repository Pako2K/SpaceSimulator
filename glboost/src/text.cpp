#include "glboost/text.h"


#define DEFAULT_COLOR      0, 0, 0, 255 // OPAQUE BLACK
#define DEFAULT_FONT_NAME  "arial"
#define DEFAULT_FONT_SIZE  12

using namespace glboost;

// Initialization of static members
Color4i     Text::_s_def_color      {DEFAULT_COLOR};
std::string   Text::_s_def_font_name  {DEFAULT_FONT_NAME};
SizePxl   Text::_s_def_font_size  {DEFAULT_FONT_SIZE};


#undef DEFAULT_COLOR
#undef DEFAULT_FONT_NAME
#undef DEFAULT_FONT_SIZE


void Text::updateFont() {
  _font->release();
  _font = &Font::get(_font_name, _font_size);
}

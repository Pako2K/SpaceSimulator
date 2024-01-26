#ifndef TEXT_H
#define TEXT_H

#include <memory>
#include <string>

#include <logger.h>

#include <glboost/graphic_types.h>
#include <glboost/font.h>
#include <glboost/text_renderer.h>


namespace glboost
{
  class Text
  {
    public:
      /**
        *  \brief  Constructor: creates a text in the specified position, using default font and color
        *          Origin of ccordinates: Upper Left Corner
        */
      Text(std::string text, PositionPxl pos_X, PositionPxl pos_Y)
            : Text(text, Position2D{pos_X, pos_Y}) {}

      /**
        * Constructor: creates a text in the specified position, using default font and color
        */
      Text(std::string text, Position2D position)
        : _text{text}, _position{position}, _font{&Font::get(_s_def_font_name, _s_def_font_size)},
        _renderer{ TextRenderer::getRenderer() } { 
        DebugLog("Text CONSTRUCTOR called."); 
      }

      ~Text() {
        _renderer.release();
        _font->release();
        DebugLog("Text DESTROYED.");
      }

      /**
        * Renders the text
        */
      void show() const { _renderer.draw(_text, *_font, _position, _color); }

      /**
        * GETTERS
        */
      const std::string& text() const      { return _text; }
      const Position2D&  position() const  { return _position; }
      const Color4i&     color() const    { return _color; }
      const std::string& fontName() const  { return _font_name; }
      const SizePxl&     fontSize() const  { return _font_size; }
      SizePxl            sizePxl() const { return _renderer.sizePxl(_text, *_font); }

      /**
        * SETTERS
        */
      static void defColor    (Color4i new_color)  { _s_def_color = new_color; }
      static void defFontName (std::string new_name) { _s_def_font_name = new_name; }
      static void defFontSize (SizePxl new_size) { _s_def_font_size = new_size; }
      void text     (std::string new_text)  { _text = new_text; }
      void position (Position2D new_pos) { _position = new_pos; }
      void position (PositionPxl pos_X, PositionPxl pos_Y) { _position = Position2D{pos_X, pos_Y}; }
      void font (std::string new_name, SizePxl new_size)  {
        if ( new_name != _font_name || new_size != _font_size ) {
          _font_name = new_name;
          _font_size = new_size;
          updateFont();
        }
      }
      void font (std::string new_name)  {
        if ( new_name != _font_name ) {
          _font_name = new_name;
          updateFont();
        }
      }
      void font (SizePxl new_size)  {
        if ( new_size != _font_size ) {
          _font_size = new_size;
          updateFont();
        }
      }
      void color    (Color4i new_color)   { _color = new_color; }
      void color    (Color3i new_color)    { for ( char i = 0; i < 3; i++ ) _color[i] = new_color[i]; }

    protected:
      static std::string  _s_def_font_name;
      static SizePxl  _s_def_font_size;
      static Color4i    _s_def_color;

      std::string             _text {""};
      Position2D           _position {0, 0};
      std::string             _font_name {_s_def_font_name};
      SizePxl             _font_size {_s_def_font_size};
      Font                  *_font;
      Color4i               _color {_s_def_color};

      TextRenderer          &_renderer;

    private:
      void updateFont();
  };

} // END namespace gltext

#endif // TEXT_H

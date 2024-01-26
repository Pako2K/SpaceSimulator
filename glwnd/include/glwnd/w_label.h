#ifndef W_LABEL_H
#define W_LABEL_H

#include <string>
#include <memory>

#include <glwnd/core/w_control.h>
#include <glboost/graphic_types.h>
#include <glboost/text.h> 
#include <glboost/g_rectangle.h>


namespace glwnd
{
  // Enumerations
  enum HorAlignmentEnum { ALIGN_LEFT, ALIGN_HOR_CENTER, ALIGN_RIGHT };
  enum VerAlignmentEnum { ALIGN_BOTTOM, ALIGN_VERT_CENTER, ALIGN_UP };

  /** \brief  A Wlabel is a rectangle containing a text. 
    *         No interaction is possible, though the position, dimensions, colors, and text can be changed programmatically
    */

  class WLabel : protected WControl
  {
  public:
    // Constructors: THE BIG-6
    /**
      * Constructor: creates a label in the main window with a text in the specified position (upper left corner), using default font and color
      */
    WLabel(const std::string& text, const glboost::Position2D& pos, const glboost::Size2D& size);

    /**
      * Constructor: creates a label in the main window with a text in the specified position, using default font and color
      */
    WLabel(const std::string& text, glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height);

    /**
      * Constructor: creates a label in the specified parent container with a text in the specified relative position, using default font and color
      */
    WLabel(WContainer& parent, const std::string& text, const glboost::Position2D& pos, const glboost::Size2D& size);

    /**
      * Constructor: creates a label in the specified parent container with a text in the specified relative position, using default font and color
      */
    WLabel(WContainer& parent, const std::string& text, glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height);

    WLabel(const WLabel&) = delete;
    WLabel(WLabel&&) = delete;
    WLabel& operator=(const WLabel&) = delete;
    WLabel& operator=(WLabel&&) = delete;
    ~WLabel() {};


    using WObject::show;
    using WObject::hide;
    using WObject::isVisible;

    /**
      * GETTERS
      */
    virtual const glboost::Position2D&  position() const override { return WObject::position(); }
    virtual const glboost::Size2D&      size() const override { return WObject::size(); }

    const std::string&       text() const { return _text->text(); }
    const std::string&       fontName() const { return _text->fontName(); }
    glboost::SizePxl         fontSize() const { return _text->fontSize(); }
    const glboost::Color4i&  fontColor() const { return _text->color(); }
    glboost::Color4i         backColor() { return _bkg_rect->fillColor().denormalized(); }
    glboost::Color4i         borderColor() { return _bkg_rect->borderColor().denormalized(); }

    /**
      * SETTERS
      */
    virtual void  position(const glboost::Position2D& new_pos) override;
    virtual void  size(const glboost::Size2D& new_size) override;

    void text(const std::string& new_text);
    void font(const std::string& new_name, glboost::SizePxl new_size);
    void fontName(const std::string& new_name);
    void fontSize(glboost::SizePxl new_size);
    void fontColor(const glboost::Color4i& new_color) { _text->color(new_color); }
    void fontColor(const glboost::Color3i& new_color) { _text->color(new_color); }
    void fontColor(const glboost::Color4f& new_color) { _text->color(new_color.denormalized()); }
    void fontColor(const glboost::Color3f& new_color) { _text->color(new_color.denormalized()); }
    void backColor(const glboost::Color4i& new_color) { _bkg_rect->fillColor(new_color.normalized()); }
    void backColor(const glboost::Color3i& new_color) { _bkg_rect->fillColor(new_color.normalized().toRGBA(1)); }
    void backColor(const glboost::Color4f& new_color) { _bkg_rect->fillColor(new_color); }
    void backColor(const glboost::Color3f& new_color) { _bkg_rect->fillColor(new_color.toRGBA(1)); }

    void borderColor(const glboost::Color4i& new_color) { _bkg_rect->borderColor(new_color.normalized()); }
    void borderColor(const glboost::Color3i& new_color) { _bkg_rect->borderColor(new_color.normalized().toRGBA(1)); }
    void borderColor(const glboost::Color4f& new_color) { _bkg_rect->borderColor(new_color); }
    void borderColor(const glboost::Color3f& new_color) { _bkg_rect->borderColor(new_color.toRGBA(1)); }

    void alignHor(HorAlignmentEnum alignment);
    void alignVer(VerAlignmentEnum alignment);


  protected:
    virtual void draw() override;


  private:
    // Horizontal margin (Left or right depending on the alignement). Default 5 pixels
    const glboost::PositionPxl       _HOR_MARGIN{ 5 };

    // Vertical margin (Up or down depending on the alignement). Default 5 pixels
    const glboost::PositionPxl       _VER_MARGIN{ 5 };

    // Text in the label
    std::unique_ptr<glboost::Text>   _text;

    // Rectangle surrounding the text
    std::unique_ptr<glboost::gRectangle> _bkg_rect;

    // Scissor box coordinates (viewport coordinates)
    GLint                            _scissor_coord[4];

    // horizontal alignment
    HorAlignmentEnum                 _hor_alignment{ ALIGN_LEFT };

    // Vertical alignment
    VerAlignmentEnum                 _ver_alignment{ ALIGN_BOTTOM };

    // Initialization called by conatructors
    void initialize(const std::string& text, const glboost::Position2D& pos, const glboost::Size2D& size);

    // Recalculates text position after a font or text change
    void textPositionHor();
    void textPositionVer();

    virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
    virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
    virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
    virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
    virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
    virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
    virtual void mouseMoved(const glboost::Position2D& pos)  override {};
  };
  
}


#endif // W_LABEL
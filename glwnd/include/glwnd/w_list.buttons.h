#ifndef W_LIST_BUTTONS_H
#define W_LIST_BUTTONS_H

#include "w_list.h"

#include <glwnd/core/w_abs_button.h>
#include <glboost/g_rectangle.h>
#include <glboost/g_triangle.h>

namespace glwnd
{
  /** \brief  A LabelSEButton is a button which shows the symbol to rewind to the start or end.
    */
  class WList::LabelSEButton : public WAbsButton
  {
  public:
    // Constructors: THE BIG-6
    LabelSEButton(const glboost::Position2D& pos, const glboost::Size2D& size, bool to_start)
      : WAbsButton{ pos, size } {
      initializeButton(to_start);
    }

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position
      */
    LabelSEButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, bool to_start)
      : WAbsButton{ parent, pos, size } {
      initializeButton(to_start);
    }
    
    /**
      * SETTERS
      */
    void borderColor(const glboost::Color4f& new_color);
    void backgroundColor(const glboost::Color4f& new_color);
    void frontColor(const glboost::Color4f& new_color);

    /**
      * GETTERS
      */
    const glboost::Color4f& borderColor() const { return _button_shape->borderColor(); }
    const glboost::Color4f& backgroundColor() const { return _button_shape->fillColor(); }
    const glboost::Color4f& frontColor() const { return _front_color; }


  protected:
    virtual void customButtonPressed() override;
    virtual void customButtonReleased() override;
    virtual void draw() override;


  private:
    // Rectangle with border, to be shown when the button is clicked and before it is released
    std::unique_ptr<glboost::gRectangle>  _button_shape;
    std::unique_ptr<glboost::gRectangle>  _vert_bar;
    std::unique_ptr<glboost::gTriangle>   _arrow;

    glboost::Color4f     _front_color{ 0, 0, 0, 1 };

    // Initialization called by constructors
    void initializeButton(bool to_start);
  };




  /** \brief  A LabelPagingButton is a button which shows the symbol to advance or go back one page in the list.
  */
  class WList::LabelPagingButton : public WAbsButton
  {
  public:
    // Constructors: THE BIG-6
    LabelPagingButton(const glboost::Position2D& pos, const glboost::Size2D& size, bool go_back)
      : WAbsButton{ pos, size } {
      initializeButton(go_back);
    }

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position
      */
    LabelPagingButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, bool go_back)
      : WAbsButton{ parent, pos, size } {
      initializeButton(go_back);
    }

    /**
      * SETTERS
      */
    void borderColor(const glboost::Color4f& new_color);
    void backgroundColor(const glboost::Color4f& new_color);
    void frontColor(const glboost::Color4f& new_color);

    /**
      * GETTERS
      */
    const glboost::Color4f& borderColor() const { return _button_shape->borderColor(); }
    const glboost::Color4f& backgroundColor() const { return _button_shape->fillColor(); }
    const glboost::Color4f& frontColor() const { return _front_color; }


  protected:
    virtual void customButtonPressed() override;
    virtual void customButtonReleased() override;
    virtual void draw() override;


  private:
    // Rectangle with border, to be shown when the button is clicked and before it is released
    std::unique_ptr<glboost::gRectangle>  _button_shape;
    std::unique_ptr<glboost::gTriangle>   _arrows[2];

    glboost::Color4f     _front_color{ 0, 0, 0, 1 };

    void initializeButton(bool go_back);
  };

}

#endif //W_LIST_BUTTONS_H
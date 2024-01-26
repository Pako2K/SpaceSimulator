#ifndef W_ICON_BUTTON_H
#define W_ICON_BUTTON_H

#include <array>

#include <glwnd/core/w_abs_button.h>
#include <glwnd/core/event_listener_set.h>
#include <glboost/g_rectangle.h>
#include <glboost/g_picture.h>


namespace glwnd
{
  /** \brief  A WIconButton is a button which is graphically drawn by 2 icons: one while released, and the other one while pressed
    */
  class WIconButton : public WAbsButton
  {
  public:
    // Constructors: THE BIG-6
   /**
     * Constructor: creates a button in the main window in the specified position (upper left corner), using the given icons for the Released and Clicked states
     */
    WIconButton(const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon);

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position, using the given icons for the Released and Clicked states
      */
    WIconButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon);

    virtual ~WIconButton();

    /**
      * GETTERS
      */
    const glboost::Color4f& color() { return _rectangle.fillColor(); }
    using WAbsButton::position;
    using WAbsButton::size;

    /**
      * SETTERS
      */
    virtual void  position(const glboost::Position2D& new_pos) override;
    virtual void  size(const glboost::Size2D& new_size) override;
    void          color(const glboost::Color4f& new_color) { _rectangle.fill(true); _rectangle.fillColor(new_color); }


  protected:
    virtual void customButtonPressed() override {};
    virtual void customButtonReleased() override {};
    virtual void draw() override;


  private:
    // Rectangle with border, to be shown when the button is clicked and before it is released
    glboost::gRectangle  _rectangle;

    // Icons for the released and clicked states
    std::array<glboost::gPicture*, 2> _icons;

    // Initialization called by constructors
    void initializeButton();
  };

}


#endif // W_ICON_BUTTON_H

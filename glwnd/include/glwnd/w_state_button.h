#ifndef W_STATE_BUTTON_H
#define W_STATE_BUTTON_H

#include <array>

#include <glwnd/core/w_control.h>
#include <glwnd/core/event_listener_set.h>
#include <glboost/g_rectangle.h>
#include <glboost/g_picture.h>


namespace glwnd
{
  // States
  enum ButtonState { BUTTON_UNSELECTED, BUTTON_SELECTED };
  

  /** \brief  A WCheckButton is a button which changes the state to on/off every time it is clicked.
    *         The state is graphically represented by icons (jpg or png files), to be provided in the constructor.
    */
  class WStateButton : protected WControl
  {
    // Listeners will receive only the "selected" event, without any parameters, besides the given context
    USE_LISTENERS(ButtonSelected);

    // Listeners will receive only the "un-selected" event, without any parameters, besides the given context
    USE_LISTENERS(ButtonUnselected);

    // Listeners will receive the new state as parameter, besides the given context
    USE_LISTENERS(ButtonStateChanged, ButtonState);
    

  public:
     // Constructors: THE BIG-6
    /**
      * Constructor: creates a button in the main window in the specified position (upper left corner), using the given icons for the Released and Clicked states
      */
    WStateButton(const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon);

    /**
      * Constructor: creates a button in the main window in the specified position, using the given icons for the Released and Clicked states
      */
    WStateButton(glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, const std::string& released_icon, const std::string& clicked_icon);

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position, using the given icons for the Released and Clicked states
      */
    WStateButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon);

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position, using the given icons for the Released and Clicked states
      */
    WStateButton(WContainer& parent, glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, const std::string& released_icon, const std::string& clicked_icon);

    WStateButton(const WStateButton&) = delete;
    WStateButton(WStateButton&&) = delete;
    WStateButton& operator=(const WStateButton&) = delete;
    WStateButton& operator=(WStateButton&&) = delete;
    ~WStateButton();


    using WObject::show;
    using WObject::hide;
    using WObject::isVisible;

    /**
      * GETTERS
      */
    virtual const glboost::Position2D&  position() const override { return WObject::position(); }
    virtual const glboost::Size2D&      size() const override { return WObject::size(); }
    ButtonState                         state() { return _state; }
    const glboost::Color4f&             color() { return _rectangle.fillColor(); }

    /**
      * SETTERS
      */
    virtual void  position(const glboost::Position2D& new_pos) override;
    virtual void  size(const glboost::Size2D& new_size) override;
    void          color(const glboost::Color4f& new_color) { _rectangle.fill(true); _rectangle.fillColor(new_color); }
    void          switchState();


  protected:
    virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override;
    virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override;
    virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
    virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
    virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
    virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
    virtual void mouseMoved(const glboost::Position2D& pos)  override {};
    virtual void draw() override;
    

  private:
    // State of the button
    ButtonState          _state{ BUTTON_UNSELECTED };

    // Rectangle with border, to be shown when the button is clicked and before it is released
    glboost::gRectangle  _rectangle;

    // Icons for the released and clicked states
    std::array<glboost::gPicture*, 2> _icons;

    // Initialization called by constructors
    void initialize(const glboost::Position2D& pos, const glboost::Size2D& size);
  };

}


#endif // W_STATE_BUTTON_H

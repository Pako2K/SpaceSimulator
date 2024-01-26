#ifndef W_ABS_BUTTON_H
#define W_ABS_BUTTON_H

#include <glwnd/core/w_control.h>
#include <glwnd/core/event_listener_set.h>


namespace glwnd
{
  /** \brief  A WAbsButton defines the functionality of a standard button which simply generates a call to the listener functions every time it is clicked.
    *         The "event" is triggered when the button is released.
    *         Derived classes must implement the graphical behaviour: how the button is drawn when it is clicked or released
    */
  class WAbsButton : protected WControl
  {
    // Listeners will receive the "clicked" and "repeat click" events, without any parameters, besides the given context
    USE_LISTENERS(ButtonClicked);
    USE_LISTENERS(ButtonRepeatClicked);

  public:
    // Constructors: THE BIG-6
    /**
     * Constructor: creates a button in the main window in the specified position (upper left corner)
     */
    WAbsButton(const glboost::Position2D& pos, const glboost::Size2D& size)
      : WControl{ pos, size, true } {}

    /**
      * Constructor: creates a button in the specified parent container in the specified relative position
      */
    WAbsButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size)
      : WControl{ parent, pos, size, true } {}

    WAbsButton(const WAbsButton&) = delete;
    WAbsButton(WAbsButton&&) = delete;
    WAbsButton& operator=(const WAbsButton&) = delete;
    WAbsButton& operator=(WAbsButton&&) = delete;
    virtual ~WAbsButton() {};


    using WObject::show;
    using WObject::hide;
    using WObject::isVisible;

    /**
      * GETTERS
      */
    const glboost::Position2D& position() const final { return WObject::position(); }
    const glboost::Size2D& size() const final { return WObject::size(); }

    /**
      * SETTERS
      */
    virtual void click() final;


  protected:
    // States
    enum ButtonState : uint8_t { BUTTON_RELEASED, BUTTON_PRESSED };

    // State of the button
    ButtonState      _state{ BUTTON_RELEASED };
  
    void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) final;
    void buttonRepeatClicked(const glboost::Position2D& pos) final;
    void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) final;
    void buttonDoubleClicked(const glboost::Position2D& pos) final {};
    void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) final {};
    void dragged(MouseButton button, const glboost::Position2D& pos) final {};
    void mouseMoved(const glboost::Position2D& pos) final {};


    // Override these methods to prepare the conditions for the draw command
    virtual void customButtonPressed() = 0;
    virtual void customButtonReleased() = 0;

    virtual void draw() override = 0;

  };

}


#endif // W_ABS_BUTTON_H


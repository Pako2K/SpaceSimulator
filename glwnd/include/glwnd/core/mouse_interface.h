#ifndef MOUSE_INTERFACE_H
#define MOUSE_INTERFACE_H

#include <glboost/graphic_types.h>


namespace glwnd
{
  enum MouseButton{LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8};

  /**
    *  \brief  Mouse Events Interface
    *          Virtual methods to be re-implemented by derived classes which handle mouse events
    *          Usually, the main window is capturing these events and handling them down to the objects implementing this interface
    *          Button and modifier values are based on output of GLFW Mouse Callbacks
    *          IMPORTANT: Positions ar relative to the WObject
    */
  class MouseInterface
  {
  public:
 
    virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) = 0;

    // This only works for the left button, without modifiers
    virtual void buttonRepeatClicked(const glboost::Position2D& pos) = 0;
       
    virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) = 0;

    virtual void buttonDoubleClicked(const glboost::Position2D& pos) = 0;

    virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) = 0;

    virtual void dragged(MouseButton button, const glboost::Position2D& pos) = 0;

    virtual void mouseMoved(const glboost::Position2D& pos) = 0;

    virtual ~MouseInterface() {}
    

  protected:
    /**
      *  \brief Protected empty default constructor
      */
    MouseInterface() {}
            
  };

}

#endif // MOUSE_INTERFACE_H
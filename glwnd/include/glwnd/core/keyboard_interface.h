#ifndef KEYBOARD_INTERFACE_H
#define KEYBOARD_INTERFACE_H


#include <GLFW/glfw3.h>


namespace glwnd
{
   
  /**
    *  \brief  Keyboard Events Interface
    *          Virtual methods to be re-implemented by derived classes which handle keyboard events
    *          Usually, the main window is capturing these events and handling them down to the objects implementing this interface
    *          Key, scancode and modifier values are based on output of GLFW Key Callback
    */
  class KeyboardInterface
  {
  public:
    
    virtual void keyPressed(int key, int scancode, int modifiers) {}

    virtual void keyRepeated(int key, int scancode, int modifiers) {}

    virtual void keyReleased(int key, int scancode, int modifiers) {}

    virtual ~KeyboardInterface() {}

    /**
      * Use default copy/move constructors and assignments
      */
    // KeyboardInterface(const KeyboardInterface&);
    // KeyboardInterface(KeyboardInterface&&);
    // KeyboardInterface& operator=(const KeyboardInterface&);
    // KeyboardInterface& operator=(KeyboardInterface&&);

  protected:
    /**
      *  \brief Protected empty default constructor
      */
    KeyboardInterface() {}
  };

}

#endif // KEYBOARD_INTERFACE_H
#ifndef W_CONTROL_H
#define W_CONTROL_H


#include <glwnd/core/w_object.h>
#include <glwnd/core/w_container.h>


namespace glwnd
{

  class WControl : public WObject
  {
  public:
    // Destructor
    virtual ~WControl();

    // Copy constructor
    WControl(const WControl& contr);

    // Move constructor not possible since external references are used (in the container)
    WControl(WControl&& contr) = delete;
    WControl& operator=(const WControl&) = delete;
    WControl& operator=(WControl&&) = delete;


  protected:
    /**
      *  \brief Protected constructor: assigns the control to the main container
      */
    WControl(glboost::Position2D a_pos, glboost::Size2D a_size, bool has_mouse_interface);
    /**
      *  \brief Protected constructor: assigns the control to a container
      */
    WControl(WContainer& parent, glboost::Position2D a_pos, glboost::Size2D a_size, bool has_mouse_interface);
    
    // A position change leads to a recalculation of the location of this control on the container
    virtual void  position(const glboost::Position2D& new_pos) override;
    
    // A size change leads to a recalculation of the location of this control on the container
    virtual void  size(const glboost::Size2D& new_size) override;
    

  protected:
    // Parent container
    WContainer&  _parent;

    // Container will handle mouse events to this control if this flag is true
    bool         _has_mouse_interface;
  };

}


#endif // W_CONTROL_H
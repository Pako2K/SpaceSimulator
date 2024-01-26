#ifndef W_OBJECT_H
#define W_OBJECT_H

#include <glboost/graphic_types.h>
#include <glwnd/core/mouse_interface.h>


namespace glwnd
{
  /**
    *  \brief  Abstract class to be implemented by any class which should be drawn in a Window. It assumes that objects are rectangular.
    *          Any object has:
    *             - visibility (true /false)
    *             - a position relative to the parent (in the window coordinates with origin at the upper left corner)
    *             - a size, width and height
    *          Objects can:
    *              - be shown or hidden
    *              - change its position, relative to the parent, or size
    *          Derived classes must:
    *              - Implement the Mouse Interface.
    *              - Implement the draw() method. This method will be called if the object is visible. Drawing is triggered by calling objDraw().
    */
  class WObject : public MouseInterface
  {
  public:
    // Copy constructor
    WObject(const WObject& obj);

    // Move constructor
    WObject(WObject&& obj);

    WObject& operator=(const WObject&) = delete;
    WObject& operator=(WObject&&) = delete;

    // Virtual Destructor
    virtual ~WObject();

    // Get position relative to the parent. See _position
    virtual const glboost::Position2D&  position() const { return _position; }
    
    // Get the size. See _size
    virtual const glboost::Size2D&      size() const { return _size; }

    // Return true if the object is visible
    virtual bool                        isVisible() const { return  _draw_fn_ptr == &WObject::draw; }

    // Drawing command: if visible, executes the customizable draw method.
    void                                objDraw() { (this->*_draw_fn_ptr)(); }

  

  protected:  
    // Position relative to the parent. For a rectangular WObject this is the Upper Left corner. For an elliptical Wobject it is the center
    glboost::Position2D     _position{ 0, 0 };

    // Size of the object. For a rectangular WObject, is the width and height. For an elliptical WObject is the semimajor and semiminor axes.
    glboost::Size2D         _size{ 0, 0 };

    /**
      *  \brief Protected constructor. Initializes position and size
      */
    WObject(const glboost::Position2D& a_pos, const glboost::Size2D& a_size);
          
    // Makes the object visible. 
    virtual void    show();

    // Makes the object invisible. 
    virtual void    hide();

    // Set position relative to the parent. See _position
    virtual void    position(const glboost::Position2D& new_position);

    // Set the size. See _size
    virtual void    size(const glboost::Size2D& new_size);
  

  private:
    // Pointer to the drawing function, depending on the visibility. INITIAL VALUE: draw()
    void        (WObject::* _draw_fn_ptr)(void) { &WObject::draw };

    // Virtual method to be re-implemented using OpenGL to draw the object
    virtual void  draw() = 0;

    // Empty draw method to be called when the the object is hidden
    void          hiddenDraw() {}
  };

}

#endif // W_OBJECT_H
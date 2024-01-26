#ifndef W_CONTAINER_H
#define W_CONTAINER_H

#include <vector>

#include <glboost/graphic_types.h>

#include <glwnd/core/w_object.h>
#include <glwnd/core/mouse_interface.h>
#include <glwnd/core/w_rect_surface.h>


namespace glwnd
{
  /**
    *  \brief  Abstract Class for containers. It's a WObject which contains other WObject's (either WControls or WCcontainers).
    *          It draws the contained WObject's, after drawing itself
    *          It handles events down to the contained WObjects or itself
    *          They have a surface divided in regions in order to handle the mouse events efficiently
    *          A WContainer can have a parent WContainer (all have a parent except the first created WContainer: the Main Window)
    */
  class WContainer : public WObject
  {
  public:
    friend class EventDispatcher;

    class EventDispatcher : public MouseInterface
    {
    public:
      EventDispatcher(WContainer& container) : _this_container{container} {}

      virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override;

      virtual void buttonRepeatClicked(const glboost::Position2D& pos) override;

      virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override;

      virtual void buttonDoubleClicked(const glboost::Position2D& pos) override;

      virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos)override;

      virtual void dragged(MouseButton button, const glboost::Position2D& pos) override;

      virtual void mouseMoved(const glboost::Position2D& pos) override;


    private:
      WContainer& _this_container;
            
      // Returns the object which must handle the event and the relative position of the event.
      // Depending on the event, it can change the focus
      std::pair<MouseInterface*, glboost::Position2D> dispatchTo(const glboost::Position2D& pos, bool changeFocus);
    };

    EventDispatcher  event_dispatcher;
    

    // Get the main container
    static WContainer&  mainContainer();

    // Get Object with focus
    static WObject&     focusObj() { return *_focus_obj; }


    // Virtual destructor
    virtual ~WContainer();
        

    // Get position on the window coordinates
    glboost::Position2D     absPosition() const { return _abs_position; }
    
    // Get Parent Container
    WContainer*             parentContainer() const { return _parent; }

    // Get list of children objects
    std::vector<WObject*>&  children() { return _children; }

    // Add child WObject. If it has mouse interface, add to the Surface
    void                    addChild(WObject& child, bool has_mouse_interface);

    // Update child WObject (visibility, position or size might have changed)
    void                    updateChild(WObject& child);

    // Remove child WObject. Called from the WObject destructor
    void                    removeChild(WObject& child);


  protected:
    /**
      *  \brief Protected constructor: assigns the container to the main container, if this is not the main container itself (the first one)
      */
    WContainer(glboost::Position2D a_pos, glboost::Size2D a_size, bool has_mouse_interface);

    /**
      *  \brief Protected constructor: assigns the container to a specific parent container
      */
    WContainer(WContainer& parent, glboost::Position2D a_pos, glboost::Size2D a_size, bool has_mouse_interface);

    WContainer(const WContainer&) = delete;
    WContainer(WContainer&&) = delete;

    WContainer& operator=(const WContainer&) = delete;
    WContainer& operator=(WContainer&&) = delete;
        
    // A position change leads to a recalculation of the location on the parent container
    virtual void  position(const glboost::Position2D& new_pos) override;

    // A size change leads to a recalculation of the location of the children on the Surface and the location on the parent container
    virtual void  size(const glboost::Size2D& new_size) override;


  private:
    // Root or main container
    static WContainer*          _main_container;

    // Object with the focus 
    static WObject*             _focus_obj;

    // Absolute position of the object with the focus 
    static glboost::Position2D  _focus_obj_abs_pos;
    
    // Absolute position on the window coordinates 
    glboost::Position2D         _abs_position{ 0, 0 };

    // Parent Container
    WContainer*                 _parent{ nullptr };

    // List of children objects
    std::vector<WObject*>       _children;

    // Surface were the children objects are located
    WRectSurface                _surface;
               
    // Draw all the children.
    virtual void draw() override {
      drawContainer();
      for (auto child_ptr : _children)
        child_ptr->objDraw();
    }

    // Drawing method of the container itself. To be implemented by the derived classes
    virtual void drawContainer() = 0;
  };

}

#endif // W_CONTAINER_H
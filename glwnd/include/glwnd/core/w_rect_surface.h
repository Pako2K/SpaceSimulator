#ifndef W_RECT_SURFACE_H
#define W_RECT_SURFACE_H

#include <vector>

#include <glboost/graphic_types.h>
#include <glwnd/core/w_object.h>


namespace glwnd
{  
  /**
   *  \brief  Window rectangular surface divided in regions
   *          Each region contains a collection of references to objects
   *          Objects can be added or updated
   *          The Surface size can be changed which will trigger a racalculation of the regions
   */
  class WRectSurface
  {
  public:
    static const glboost::SizePxl   DEF_MIN_REGION_SIZE{ 50 };
    static const uint16_t           DEF_NUM_SEGMENTS{ 10 };
    
    /**
      *  \brief  Constructor. By default divides the surface in 20 columns and the proportional number of rows, according to the surface dimensions
      *          A minimum size of each region can be deined (by defualt 100 pixels)
      */
    WRectSurface(glboost::Size2D surface_size, glboost::SizePxl min_region_size = DEF_MIN_REGION_SIZE);

    WRectSurface(const WRectSurface&) = delete;
    WRectSurface(WRectSurface&&) = delete;
    WRectSurface& operator=(const WRectSurface&) = delete;
    WRectSurface& operator=(WRectSurface&&) = delete;

    ~WRectSurface() {}

    /**
     *  \brief Set new size
     */
    void      size(const glboost::Size2D& new_size);

    /**
      *  \brief Add object to one or more regions. Does not check whether the object was already added
      */
    void      addObject(WObject& w_obj);

    /**
      *  \brief Remove object
      */
    void      removeObject(WObject& w_obj);
    
    /**
      *  \brief Update object: removes the object and re-adds it again
      */
    void      updateObject(WObject& w_obj);

    // Assumes the position is within the region. Otherwise it will return an out_of_range exception
    // If there are several objects in the same region it returns the first match
    WObject*  getObject(const glboost::Position2D& pos);


  private:
    // Each region contains a list of objects which can receive mouse events
    struct Region {
      std::vector<WObject*>   objs;
    };
    
    glboost::SizePxl                  _min_region_size;

    // Region size (static): all regions have the same size!
    glboost::Size2D                   _region_size{ 0, 0 };

    // Matrix of regions
    std::vector<std::vector<Region>>  _regions;

    // List with all the Objects, including the ones outside the surface 
    // (they could get into the regions if they are updated or the surface size changes)
    std::vector<WObject*>             _all_objs;


    void  setRegions(glboost::Size2D surface_size);

    void  addToRegions(WObject& w_obj);

    void  removeFromRegions(const WObject& w_obj);
  };

}

#endif //W_RECT_SURFACE_H
#ifndef CIRCLE_H
#define CIRCLE_H

#include <geometry/shapes2D/ellipse.h>


namespace geometry {

  /**
    * \brief  Template class for a circle
    *         Template parameter:  TYPE --> type used for the coordinates
    *         A circle is a special type of ellipse with eccentrcity = 0
    *         Parameters:
    *             - radius (=semimajor axis),  default = 1
    *             - [OPT] geometric center of the circle (default = 0,0,0)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) --> It will be rounded to a multiple of 4, min = 8
    */
  template <typename TYPE = float>
  class Circle : public Ellipse<TYPE>
  {
  public:
    /**
      * \brief  Default circle constructor. It creates a circle of unit radius, centered at the origin
      */
    Circle() : Ellipse<TYPE>() {}

    /**
      * \brief  Circle constructor. It creates a circle of the specified radius, centered at the given point
      */
    Circle(Length<TYPE> radius, const Point3<TYPE>& center = Point3<TYPE>()) : Ellipse<TYPE>(radius, radius, center) {}

    /**
     * \brief  Ellipse destructor.
     */
    ~Circle() {};

    /**
      * \brief  Get radius
      */
    Length<TYPE> radius() const { return Ellipse<TYPE>::_a; }

    /**
      * \brief  Get length of the circle.
      */
    Length<TYPE> perimeter() const override { return Circle::perimeter(Ellipse<TYPE>::_a); }

    using Ellipse<TYPE>::area;
    

    /**
      * \brief  Calculates length of a circle with the given radius.
      */
    static Length<TYPE> perimeter(Length<TYPE> radius) {
      return static_cast<TYPE>(2 * PI * radius);
    }

    /**
      * \brief  Calculates area of a circle with the given radius.
      */
    static TYPE area(Length<TYPE> radius) { return static_cast<TYPE>(PI)* radius * radius; }
  
    /**
      * \brief  output stream insertion operator
      */
    friend std::ostream& operator<<(std::ostream& os, const Circle<TYPE>& circle) {
      os << "{ *** Circle radius = " << circle.radius() << " (" << circle.size() << " vertices) ***" << std::endl;
      os << " Center: " << circle.center();
      os << static_cast<const Shape<TYPE>&>(circle);
      os << "}" << std::endl;

      return os;
    }
  
  private:
    using Ellipse<TYPE>::a;
    using Ellipse<TYPE>::b;
    using Ellipse<TYPE>::e;
    using Ellipse<TYPE>::focus;
  };
}

#endif

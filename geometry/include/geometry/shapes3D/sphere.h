#ifndef SPHERE_H
#define SPHERE_H

#include <geometry/shapes3D/ellipsoid.h>


namespace geometry {

  /**
    * \brief  Template class for a sphere
    *         Template parameter:  TYPE --> type used for the coordinates
    *         A sphere is a special type of ellipsoid with a = b = c
    *         Parameters:
    *             - radius (=semi axis X),  default = 1
    *             - [OPT] geometric center of the sphere (default = 0,0,0)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) 
    */
  template <typename TYPE = float>
  class Sphere : public Ellipsoid<TYPE>
  {
  public:
    /**
      * \brief  Default sphere constructor. It creates a sphere of unit radius, centered at the origin
      */
    Sphere() : Ellipsoid<TYPE>() {}

    /**
      * \brief  Sphere constructor. It creates a sphere of the specified radius, centered at the given point
      */
    Sphere(Length<TYPE> radius, const Point3<TYPE>& center = Point3<TYPE>()) : Ellipsoid<TYPE>(radius, radius, radius, center) {}

    /**
     * \brief  Ellipse destructor.
     */
    ~Sphere() {};

    /**
      * \brief  Get radius
      */
    Length<TYPE> radius() const { return Ellipsoid<TYPE>::_a; }

    TYPE area() const override { return Sphere::area(this->_a); }

    using Ellipsoid<TYPE>::volume;
        
    /**
      * \brief  Calculates area of a sphere with the given radius.
      */
    static TYPE area(Length<TYPE> radius) { return static_cast<TYPE>(PI) * radius* radius; }

    /**
      * \brief  output stream insertion operator
      */
    friend std::ostream& operator<<(std::ostream& os, const Sphere<TYPE>& sphere) {
      os << "{ *** Sphere radius = " << sphere.radius() << " (" << sphere.size() << " vertices) ***" << std::endl;
      os << " Center: " << sphere.center();
      os << static_cast<const Shape<TYPE>&>(sphere);
      os << "}" << std::endl;

      return os;
    }

  private:
    using Ellipsoid<TYPE>::a;
    using Ellipsoid<TYPE>::b;
    using Ellipsoid<TYPE>::c;
    
  };
}

#endif

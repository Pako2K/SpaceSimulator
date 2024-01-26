#ifndef CYLINDER_H
#define CYLINDER_H

#include <geometry/constants.h>
#include <geometry/shapes2D/circle.h>

namespace geometry {
    
  /**
    * \brief  Template class for a Cylinder
    *         Template parameter:  TYPE --> type used for the coordinates
    *         A Cylinder is a 3D figure.
    *         The internal CS is always centered at the geometric center of the base, with X axis and Y contained in the Cylinder's base
    *             and the Z axis perpendicular to the base and pointing to the center of the Cylinder's top.
    *         Initially the Cylinder's internal CS is aligned with the reference CS and centered at the (optionally) given center.
    *         Parameters:
    *            - radius (default = 1)
    *            - height (default = 1)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) --> It will be rounded to a multiple of 8, min = 16
    *           (1 vertex for each 45° in the base circle and the top circle of the cylinder)
    */
  template <typename TYPE = float>
  class Cylinder : public Shape<TYPE> {
  public:
    /**
      * \brief  Minimun number of vertices
      */
    static const size_t MIN_VERTICES{ 16 };

    /**
      * \brief  Default cylinder constructor. It creates a cylinder of unit radius, unit height and base centered at the origin
      */
    Cylinder() : Cylinder(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    /**
      * \brief  Generic Cylinder constructor. It creates a Cylinder of a given base radius, given height and centered at the given point
      */
    Cylinder(Length<TYPE> radius, Length<TYPE> height, const Point3<TYPE>& base_center = Point3<TYPE>()) : _radius{ radius }, _height{ height } {
      this->_cs.translate(base_center);
    }

    /**
      * \brief  cylinder destructor.
      */
    ~Cylinder() {}

    /**
      * \brief  Get Cylinder radius.
      */
    Length<TYPE> radius() const { return _radius; }

    /**
      * \brief  Get height.
      */
    Length<TYPE> height() const { return _height; }

    /**
      * \brief  Get base circle center.
      */
    Point3<TYPE> baseCenter() const { return this->_cs.center(); }

    /**
      * \brief  Get base circle normal.
      */
    UnitVec3<TYPE> baseNormal() const { return -this->_cs[Axis::Z]; }

    /**
      * \brief  Get top circle center.
      */
    Point3<TYPE> topCenter() const { return this->_cs.center() + _height * this->_cs[2]; }

    /**
      * \brief  Get top circle normal.
      */
    UnitVec3<TYPE> topNormal() const { return this->_cs[Axis::Z]; }
        
    /**
      * \brief  Get area of the Cylinder.
      */
    TYPE area() const override { return Cylinder::area(_radius, _height); }

    /**
      * \brief  Get volume of the Cylinder.
      */
    TYPE volume() const override { return Cylinder::volume(_radius, _height); }

    /**
      * \brief  Calculates area of a Cylinder of the given radius and height.
      */
    static TYPE area(Length<TYPE> radius, Length<TYPE> height) { return static_cast<TYPE>(TWO_PI) * radius * (radius + height); }

    /**
      * \brief  Calculates volume of a cylinder of the given radius and height.
      */
    static TYPE volume(Length<TYPE> radius, Length<TYPE> height) { return static_cast<TYPE>(PI) * radius * radius * height; }

    /**
      * \brief  output stream insertion operator
      */
    friend std::ostream& operator<<(std::ostream& os, const Cylinder<TYPE>& cylinder) {
      os << "{ *** Cylinder R=" << cylinder.radius() << " H=" << cylinder.height() << " (" << cylinder.size() << " vertices) ***" << std::endl;
      os << " Base Center: " << cylinder.baseCenter();
      os << " Base Normal: " << cylinder.baseNormal() << std::endl;
      os << " Top Center: " << cylinder.topCenter();
      os << " Top Normal: " << cylinder.topNormal() << std::endl;
      os << static_cast<const Shape<TYPE>&>(cylinder);
      os << "}" << std::endl;

      return os;
    }


  protected:
    /**
      * \brief  BASE RADIUS
      */
    Length<TYPE> _radius;

    /**
      * \brief  HEIGHT
      */
    Length<TYPE> _height;
    
    /**
      * \brief  Recalculates the vertices and normals of the cylinder, based on the BASE number of vertices (the tip counted separately)
      *         The actual number of vertices will be rounded down to a multiple of 4, due to the simmetry of the cylinder
      * @throw  range_error exception if the number of vertices is not 16 or bigger
      */
    virtual void setVerticesAndNormals(size_t num_vertices) override {
      if (num_vertices < MIN_VERTICES)
        throw std::range_error(std::string("Minimum number of vertices is ") + std::to_string(MIN_VERTICES));

      // Round to the nearest (floor) multiple of 8
      num_vertices -= num_vertices % (MIN_VERTICES / 2);

      Circle<TYPE> base(_radius);
      base.vertices(num_vertices / 2);
      
      // USING THE INTERNAL CS CENTERED AT THE CENTER OF THE BASE!!
      for (size_t i = 0; i < base.size(); ++i) {
        this->_rel_vertices.push_back({ base.vertices()[i] });
        this->_rel_normals.push_back({ base.normals()[i] });
      }
      base.translate({ 0, 0, _height });
      for (size_t i = 0; i < base.size(); ++i) {
        this->_rel_vertices.push_back({ base.vertices()[i] });
        this->_rel_normals.push_back({ base.normals()[i] });
      }
    }

    virtual void specificScale(TYPE factor) {
      _radius *= factor;
      _height *= factor;
    }


  private:
    // This method can't be used!
    Length<TYPE> perimeter() const override { return 0.0; }

  }; // class Cylinder

} // namespace geometry


#endif // CYLINDER_H

#ifndef CONE_H
#define CONE_H


#include <geometry/constants.h>
#include <geometry/shapes2D/circle.h>

namespace geometry {

  /**
    * \brief  Template class for a cone
    *         Template parameter:  TYPE --> type used for the coordinates
    *         A Cone is a 3D figure.
    *         The internal CS is always centered at the geometric center of the base, with X axis and Y contained in the cone's base 
    *             and the Z axis perpendicular to the base and pointing to the tip of the cone.
    *         Initially the cone's internal CS is aligned with the reference CS and centered at the (optionally) given center.
    *         Parameters:
    *            - radius (default = 1)
    *            - height (default = 1)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) --> It will be rounded to a multiple of 4 plus 1, min = 9
    *           (1 vertex for each 45° in the base circle plus the tip of the cone)
    */
  template <typename TYPE = float>
  class Cone : public Shape<TYPE>
  {
  public:
    /**
      * \brief  Minimun number of vertices
      */
    static const size_t MIN_VERTICES{ 9 };
    
    /**
      * \brief  Default cone constructor. It creates a cone of unit radius, unit height and based centered at the origin
      */
    Cone() : Cone(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    /**
      * \brief  Generic cone constructor. It creates a cone of a given base radius, given height and centered at the given point
      */
    Cone(Length<TYPE> radius, Length<TYPE> height, const Point3<TYPE>& base_center = Point3<TYPE>()) : _radius{ radius }, _height{ height } {
       this->_cs.translate(base_center);
    }

    /**
      * \brief  Cone destructor.
      */
    ~Cone() {}
     
    /**
      * \brief  Get base circle radius.
      */
    Length<TYPE> radius() const { return _radius; }

    /**
      * \brief  Get height.
      */
    Length<TYPE> height() const { return _height; }

    /**
      * \brief  Get base circle center.
      */
    const Point3<TYPE> & baseCenter() const { return this->_cs.center(); }

    /**
      * \brief  Get base circle normal.
      */
    UnitVec3<TYPE> baseNormal() const { return -this->_cs[Axis::Z]; }
    
    /**
      * \brief  Get area of the cone.
      */
    TYPE area() const override { return Cone::area(_radius, _height); }

    /**
      * \brief  Get volume of the cone.
      */
    TYPE volume() const override { return Cone::volume(_radius, _height); }

    /**
      * \brief  Calculates area of a cone of the given radius and height.
      */
    static TYPE area(Length<TYPE> radius, Length<TYPE> height) { return static_cast<TYPE>(PI) * radius * (radius + sqrt(radius * radius + height * height)); }

    /**
      * \brief  Calculates volume of a cone of the given radius and height.
      */
    static TYPE volume(Length<TYPE> radius, Length<TYPE> height) { return static_cast<TYPE>(PI) * radius * radius * height / 3; }

    /**
      * \brief  output stream insertion operator
      */
    friend std::ostream& operator<<(std::ostream& os, const Cone<TYPE>& cone) {
      os << "{ *** Cone R=" << cone.radius() << " H=" << cone.height() << " (" << cone.size() << " vertices) ***" << std::endl;
      os << " Base Center: " << cone.baseCenter();
      os << " Base Normal: " << cone.baseNormal() << std::endl;
      os << static_cast<const Shape<TYPE>&>(cone);
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
      * \brief  Recalculates the vertices and normals of the cone, based on the BASE number of vertices (the tip counted separately)
      *         The actual number of vertices will be rounded down to a multiple of 4 plus 1, due to the simmetry of the cone
      * @throw  range_error exception if the number of vertices is not 9 or bigger
      */
    virtual void setVerticesAndNormals(size_t num_vertices) override  {
      if (num_vertices < MIN_VERTICES)
        throw std::range_error(std::string("Minimum number of vertices is ") + std::to_string(MIN_VERTICES));

      // Round to the nearest (floor) multiple of 4 plus 1
      num_vertices -= (num_vertices - 1) % 4;

      Circle<TYPE> base(_radius);

      base.vertices(num_vertices - 1);

      // USING THE INTERNAL CS CENTERED AT THE CENTER OF THE BASE!!
      TYPE xy_normal_comp{ static_cast<TYPE>(1.0f) / std::sqrt(static_cast<TYPE>(1.0f) + _radius * _radius / _height / _height) };
      TYPE z_normal_comp{ _radius / _height * xy_normal_comp };
      TYPE delta_angle = static_cast<TYPE>(TWO_PI) / base.size();
      for (size_t i = 0; i < base.size(); ++i) {
        this->_rel_vertices.push_back({base.vertices()[i]});
        this->_rel_normals.push_back({ xy_normal_comp * cos(i * delta_angle), xy_normal_comp * sin(i * delta_angle), z_normal_comp });
      }
      // Tip 
      this->_rel_vertices.push_back({ 0, 0, _height });
      this->_rel_normals.push_back({ Vec3<TYPE>::UnitZ() });
    }

    virtual void specificScale(TYPE factor) {
      _radius *= factor;
      _height *= factor;
    }


  private:
    // This method can't be used!
    Length<TYPE> perimeter() const override { return 0.0; }

  }; // class Cone

} // namespace geometry


#endif // CONE_H

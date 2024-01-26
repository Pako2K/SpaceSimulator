#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <geometry/constants.h>
#include <geometry/shapes2D/ellipse.h>

namespace geometry {

  /**
    * \brief  Template class for an Ellipsoid
    *         Template parameter:  TYPE --> type used for the coordinates
    *         The internal CS is always centered at the geometric center of the Ellipsoid
    *         Initially the Ellipsoid's internal CS is aligned with the reference CS and centered at the (optionally) given center.
    *         Parameters:
    *            - horizontal semi axis, aligned with X  (default = 1)
    *            - horizontal semi axis = vertical semi axis, aligned with Y (default = 1)
    *            - vertical semimi axis aligned with Z (default = 1)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) --> MIN: 26 vertices, 8 for for each horizontal ellipse
    *             plus 2 vertices for the poles (1 vertex for each 45° in the horizontal ellipses and the vertical ellipses)
    */
  template <typename TYPE = float>
  class Ellipsoid: public Shape<TYPE> {
  public:
    /**
      * \brief  Minimun number of vertices
      */
    static const size_t MIN_VERTICES{ 26 };

    /**
      * \brief  Default Ellipsoid constructor. It creates a sphere of radius 1
      */
    Ellipsoid() : Ellipsoid(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    /**
      * \brief  Generic Ellipsoid constructor. It creates a Ellipsoid of a given semi axes
      */
    Ellipsoid(Length<TYPE> a, Length<TYPE> b, Length<TYPE> c, const Point3<TYPE>& center = Point3<TYPE>()) : _a{ a }, _b{ b }, _c{ c } {
      this->_cs.translate(center);
    }

    /**
      * \brief  Ellipsoid destructor.
      */
    ~Ellipsoid() {}

    /**
      * \brief  Get Ellipsoid semi axis X.
      */
    Length<TYPE> a() const { return _a; }

    /**
      * \brief  Get Ellipsoid semi axis Y.
      */
    Length<TYPE> b() const { return _b; }

    /**
      * \brief  Get Ellipsoid semi axis Z.
      */
    Length<TYPE> c() const { return _c; }
    
    /**
      * \brief  Get area of the Ellipsoid.
      */
    TYPE area() const override { 
      return Ellipsoid::area(_a, _b, _c); 
    }

    /**
      * \brief  Get volume of the Ellipsoid.
      */
    TYPE volume() const override { return Ellipsoid::volume(_a, _b, _c); }

    /**
      * \brief  Calculates area of a Cylinder of the given radius and height.
      */
    static TYPE area(Length<TYPE> a, Length<TYPE> b, Length<TYPE> c) {
      // This is an approximation with an error >1% !!!
      constexpr TYPE p{ static_cast<TYPE>(1.67) };
      return static_cast<TYPE>(4 * PI)* pow((pow(a,p)*pow(b,p) + pow(a, p) * pow(c, p)+ pow(b, p) * pow(c, p)) / 3, static_cast<TYPE>(1 / p));
    }

    /**
      * \brief  Calculates volume of a cone of the given radius and height.
      */
    static TYPE volume(Length<TYPE> a, Length<TYPE> b, Length<TYPE> c) { return static_cast<TYPE>(4 * PI / 3) * a * b * c; }

    /**
      * \brief  output stream insertion operator
      */
    friend std::ostream& operator<<(std::ostream & os, const Ellipsoid<TYPE>& ellipsoid) {
      os << "{ *** Ellipsoid a=" << ellipsoid.a() << " b=" << ellipsoid.b() << " c=" << ellipsoid.c() << " (" << ellipsoid.size() << " vertices) ***" << std::endl;
      os << " Center: " << ellipsoid.center();
      os << static_cast<const Shape<TYPE>&>(ellipsoid);
      os << "}" << std::endl;

      return os;
    }


  protected:
    /**
      * \brief  Semi axis along X
      */
    Length<TYPE> _a;

    /**
      * \brief  Semi axis along Y
      */
    Length<TYPE> _b;

    /**
      * \brief  Semi axis along Z
      */
    Length<TYPE> _c;

    /**
      * \brief  Recalculates the vertices and normals of the ellipsoid,
      * @throw  range_error exception if the number of vertices is not 26 or bigger
      */
    virtual void setVerticesAndNormals(size_t num_vertices) override {
      if (num_vertices < MIN_VERTICES)
        throw std::range_error(std::string("Minimum number of vertices is ") + std::to_string(MIN_VERTICES));

      // Round so each ellipse has at least 8 vertices
      size_t vert_per_ellipse{ 8 };
      size_t num_ellipses{ 3 };
      while ((num_vertices - 2) > (vert_per_ellipse + 4) * (num_ellipses + 2)) {
        vert_per_ellipse += 4;
        num_ellipses += 2;
      }
      num_vertices = 2 + vert_per_ellipse * num_ellipses;

      // Starting at the South pole
      this->_rel_vertices.push_back({ 0, 0, -_c });
      this->_rel_normals.push_back({ -Vec3<TYPE>::UnitZ() });

      // Add vertices and normals of the intermediate ellipses until the north pole
      const Angle<TYPE> delta_angle{ static_cast<TYPE>(TWO_PI) / vert_per_ellipse };
      for (size_t i = 1; i <= num_ellipses; ++i) {
        Angle<TYPE> angle{ -static_cast<TYPE>(HALF_PI) + i * delta_angle };
        TYPE cos_angle{ cos(angle) };
        TYPE sin_angle{ sin(angle) };
        Ellipse<TYPE> hor_ellipse(_a * cos_angle, _b * cos_angle);
        hor_ellipse.vertices(vert_per_ellipse);
        hor_ellipse.translate({ 0, 0, static_cast<TYPE>(_c) * sin_angle });

        // XY coordinate of the normal vector (not normalized!) in the plane containing this vertex and the poles
        TYPE normal_xy{ static_cast<TYPE>(_c) * cos_angle };
        for (size_t ver = 0; ver < hor_ellipse.size(); ++ver) {
          this->_rel_vertices.push_back({ hor_ellipse.vertices()[ver] });

          // Z coordinate of the Normal vector (not normalized!) in the plane containing this vertex and the poles
          TYPE normal_z{ sqrt(pow(hor_ellipse.vertices()[ver].x(), 2) + pow(hor_ellipse.vertices()[ver].y(), 2)) * sin_angle };

          // The normal in the xy plane has the same direction
          this->_rel_normals.push_back({ normal_xy * hor_ellipse.normals()[ver].x(), normal_xy * hor_ellipse.normals()[ver].y(), normal_z });
        }
      }

      // Finishing at the North pole
      this->_rel_vertices.push_back({ 0, 0, _c });
      this->_rel_normals.push_back({ Vec3<TYPE>::UnitZ() });
    }

    virtual void specificScale(TYPE factor) {
      _a *= factor;
      _b *= factor;
      _c *= factor;
    }


    private:
      // This method can't be used!
      Length<TYPE> perimeter() const override { return 0.0; }

  }; // class Ellipsoid

} // namespace geometry


#endif // CYLINDER_H

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <stdexcept>

#include <geometry/shape.h>

namespace geometry {

  /**
    * \brief  Eccentricity type. Validates that the eccentricty is in the range [0,1)
    */
  template <typename TYPE = float>
  class Eccentricity {
  public:
    Eccentricity(TYPE e) : _e{ e } {
      if (e < 0 || e >= static_cast<TYPE>(1))
        throw std::range_error("Invalid Eccentricity");
    };
    TYPE e() const { return _e; };
    TYPE& operator = (TYPE new_ecc) { _e = new_ecc; };
    operator TYPE() const { return _e; };

  private:
    TYPE _e;
  };


  /** 
    * \brief  Template class for an ellipse
    *         Template parameter:  TYPE --> type used for the coordinates
    *         An ellipse is a 2D figure. 
    *         The internal CS is always centered at the geometric center of the ellipse, with the X axis aligned with the positive semimajor axis, 
    *             the Y axis alingned with the positive semiminor axis and the Z axis perpendicular to the ellipse's plane
    *         Initially the ellipse's internal CS is aligned with the reference CS and centered at the (optionally) given center
    *         A special case of an ellipse is a circle ( semimajor axis = semiminor axis, or eccentricity = 0)
    *         Parameters:
    *             - semimajor axis (default = 1)
    *             - semiminor axis(default = 1) or eccentricity (default = 0)
    *             - [OPT] geometric center of the ellipse (default = 0,0,0)
    *
    *         Num_vertices (default = 0). Must be initialized with a call to vertices(size_t) --> It will be rounded to a multiple of 4, min = 8
    */
  template <typename TYPE = float>
  class Ellipse : public Shape<TYPE>
  {
  public:
    /**
      * \brief  Minimun number of vertices
      */
    static const size_t MIN_VERTICES{ 8 };

    /**
      * \brief  Default ellipse constructor. It creates a circle of unit radius, centered at the origin
      */
    Ellipse() : Ellipse(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    /**
      * \brief  Ellipse constructor. It creates an ellipse based on a semimajor axis and eccentricity, centered at the given point
      */
    Ellipse(Length<TYPE> semimajor, Eccentricity<TYPE> e, const Point3<TYPE>& center = Point3<TYPE>()) : Ellipse(semimajor, semimajor * sqrt(static_cast<TYPE>(1.0) - e * e), center) {}

    /**
      * \brief  Ellipse constructor. It creates an ellipse based on a semimajor and a semiminor axis, centered at the given point
      * @throw  range_error if semimajor or semiminor axis is not bigger than 0, and semimajor axis is not bigger than semiminor axis
      */
    Ellipse(Length<TYPE> semimajor, Length<TYPE> semiminor, const Point3<TYPE>& center = Point3<TYPE>()) :
      _a{ semimajor }, _b{ semiminor }, _e{ std::sqrt(static_cast<TYPE>(1.0) - semiminor * semiminor / semimajor / semimajor) } {

      if (semimajor <= 0)
        throw std::range_error("Invalid Semimajor axis");
      if (semiminor <= 0 || semiminor > semimajor)
        throw std::range_error("Invalid Semiminor axis");

      this->_cs.translate(center);
    }

    /**
      * \brief  Ellipse destructor.
      */
    ~Ellipse() {};
  
    /**
      * \brief  Get semimajor axis.
      */
    Length<TYPE> a() const { return _a; }

    /**
      * \brief  Get semiminor axis.
      */
    Length<TYPE> b() const { return _b; }
    
    /**
      * \brief  Get eccentricity.
      */
    TYPE e() const { return TYPE(_e); }

    /**
      * \brief  Get the geometric center of the ellipse.
      */
    Point3<TYPE> center() const { return this->_cs.center(); }

    /**
      * \brief  Get the focus of the ellipse.
      */
    Point3<TYPE> focus() const {
        // First vertex vector
        Point3<TYPE> first_vert;
        if (this->_vertices.size())
          first_vert = this->_vertices[0];
        else
          first_vert = this->_cs.center() + _a * this->_cs[0];

        // Vector from the center to the first vertex 
        Vec3<TYPE> center2vertex = first_vert.vec() - this->_cs.center().vec();

        return this->_cs.center() + _a * _e / center2vertex.norm() * center2vertex;;
    }

    /**
      * \brief  Get length of the ellipse.
      */
    Length<TYPE> perimeter() const override{ return Ellipse::perimeter(_a, _b); }

    /**
      * \brief  Get area of the ellipse.
      */
    TYPE area() const override { return Ellipse::area(_a, _b); } 

    /**
      * \brief  Calculates approximate length of an ellipse of the given semimajor and semiminor axes.
      */
    static Length<TYPE> perimeter(Length<TYPE> semimajor, Length<TYPE> semiminor) {
      // APPROXIMATION!!!
      TYPE h = (semimajor - semiminor) / (semimajor + semiminor);
      TYPE h2 = h * h;

      return static_cast<TYPE>(PI) * (semimajor + semiminor) * sqrt(1 + (3 * h2) / (10 + (sqrt(4 - 3 * h2))));
    }

    /**
      * \brief  Calculates area of an ellipse of the given semimajor and semiminor axes.
      */
    static TYPE area(Length<TYPE> semimajor, Length<TYPE> semiminor) { return static_cast<TYPE>(PI) * semimajor * semiminor; }

    /**
      * \brief  output stream insertion operator 
      */
    friend std::ostream& operator<<(std::ostream& os, const Ellipse<TYPE>& ellipse) {
      os << "{ *** Ellipse a = " << ellipse.a() << ", b = " << ellipse.b() << " (" << ellipse.size() << " vertices) ***" << std::endl;
      os << " Center: " << ellipse.center();
      os << " Eccentricity: " << ellipse.e() << std::endl;
      os << " Focus: " << ellipse.focus();
      os << static_cast<const Shape<TYPE>&>(ellipse);
      os << "}" << std::endl;

      return os;
    }


  protected:
    /**
      * \brief  SEMIMAJOR AXIS
      */
    Length<TYPE> _a;

    /**
      * \brief  SEMIMINOR AXIS
      */
    Length<TYPE> _b;

    /**
      * \brief  ECCENTRICITY
      */
    Eccentricity<TYPE> _e;
    
    /**
      * \brief  Recalculates the vertices and normals of the ellipse
      *         The actual number of vertices will be rounded down to a multiple of 4, due to the simmetry of the ellipse
      * @throw  range_error exception if the number of vertices is not 8 or bigger
      */
    void setVerticesAndNormals(size_t num_vertices) override {
      if (num_vertices < MIN_VERTICES)
        throw std::range_error(std::string("Minimum number of vertices is ") + std::to_string(MIN_VERTICES));

      // Round to the nearest (floor) multiple of 4 
      num_vertices -= num_vertices % 4;
      
      // Calculate vertices based on the upper quadrant, using equally distributed angles from the ellipse's center (GOOD APPROXIMATION FOR LOW e)
      //        alfa = angle from the center
      //        delta_alfa = 2*PI / num_vertices
      //        num_iterations (for the upper half) = num_vertices / 2
      const Angle<TYPE> delta_angle = static_cast<TYPE>(TWO_PI) / num_vertices;
      const size_t num_iterations = num_vertices / 2;
      for (size_t i = 0; i < num_iterations; ++i) {
        // Vertex = a * cos(alfa), b * sin(alfa)
        this->_rel_vertices.push_back({ TYPE(_a) * cos(delta_angle * i), TYPE(_b) * sin(delta_angle * i), 0 });
        // Normal vector = b * cos(alfa), a * sin(alfa)
        this->_rel_normals.push_back({ TYPE(_b) * cos(delta_angle * i), TYPE(_a) * sin(delta_angle * i), 0 });
      }
      for (size_t i = 0; i < num_iterations; ++i
        
        ) {
        this->_rel_vertices.push_back({ -this->_rel_vertices[i].vec() });
        this->_rel_normals.push_back({ -this->_rel_normals[i].vec() });
      }
    }

    virtual void specificScale(TYPE factor) {
      _a *= factor;
      _b *= factor;
    }


  private:
    // This method can't be used!
    TYPE volume() const override { return 0.0; }

  }; // class Ellipse

} // namespace geometry


#endif // ELLIPSE_H

#ifndef REG_POLYGON_H
#define REG_POLYGON_H

#include <geometry/shape.h>


namespace geometry {

  /** CLASS Regular Polygon
    * Template params:
    *                 TYPE --> type used for the coordinates
    * A Polygon is a 2D figure. Additional dimensions will be completed with coordinate value 0.0
    * The polygon is circumscribed ina circle of radius r and centered at the origin
    * Parameters:
    *            - num_vertices
    *            - radius 
    */

  template <typename TYPE = float>
  class RegPolygon : public Shape<TYPE>
  {
  public:
    // Default RegPolygon constructor
    RegPolygon(size_t num_vertices) : RegPolygon(num_vertices, static_cast<TYPE>(1), Point3<TYPE>()) {}

    // Generic RegPolygon constructor
    RegPolygon(size_t num_vertices, Length<TYPE> radius, const Point3<TYPE>& center = Point3<TYPE>()) : _radius{ radius } {
      if (num_vertices < 3)
        throw std::invalid_argument("Number of vertices must be greater than 2");

      this->translate(center.vec());
      Shape<TYPE>::vertices(num_vertices);
    }

    ~RegPolygon() {};

    Length<TYPE> radius() const { return _radius; }
    
    Length<TYPE> perimeter() const override { return RegPolygon::perimeter(this->size(), _radius); }
    TYPE area() const override { return RegPolygon::area(this->size(), _radius); }
    
    static Length<TYPE> perimeter(size_t num_vertices, Length<TYPE> radius) { return  num_vertices * static_cast<TYPE>(2) * radius * sin(static_cast<TYPE>(PI) / num_vertices); }
    static TYPE area(size_t num_vertices, Length<TYPE> radius) { return  num_vertices * static_cast<TYPE>(0.5)* radius * radius * sin(static_cast<TYPE>(TWO_PI) / num_vertices); }

    friend std::ostream& operator<<(std::ostream & os, const RegPolygon<TYPE> & pol) {
      os << "{\n*** RegPolygon N = " << pol.size() << " R = " << pol.radius() << " ***" << std::endl;
      os << " Center: " << pol.center();
      os << static_cast<const Shape<TYPE>&>(pol);
      os << "}" << std::endl;

      return os;
    }

    using Shape<TYPE>::vertices;


  protected:
    Length<TYPE> _radius;

    virtual void setVerticesAndNormals(size_t num_vertices) override {
      // Calculate the vertices starting from the Y axis
      Angle<TYPE> delta_angle = static_cast<TYPE>(TWO_PI) / num_vertices;
      for (size_t i = 0; i < num_vertices; ++i) {
        // Watch out: starting from the Y axis!!!
        this->_rel_vertices.push_back({ -static_cast<TYPE>(_radius) * sin(i * delta_angle), static_cast<TYPE>(_radius) * cos(i * delta_angle), 0 });
      }

      // Normals to the sides, starting with the vertices 0 and 1
      for (size_t i = 0; i < num_vertices - 1; ++i) {
        Vec3<TYPE> side_vec = this->_rel_vertices[i].vec() - this->_rel_vertices[i + 1].vec();
        this->_rel_normals.push_back({ -side_vec[Axis::Y], side_vec[Axis::X], 0 });
      }
      Vec3<TYPE> side_vec = this->_rel_vertices[num_vertices - 1].vec() - this->_rel_vertices[0].vec();
      this->_rel_normals.push_back({ -side_vec[Axis::Y], side_vec[Axis::X], 0 });
    }

    virtual void specificScale(TYPE factor) {
      _radius *= factor;
    }


  private:
    // These methods can't be used!
    virtual void vertices(size_t num_vertices) override {};
    TYPE volume() const override { return 0; }

  }; // class RegPolygon

}


#endif // REG_POLYGON_H

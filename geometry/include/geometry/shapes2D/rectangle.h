#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <geometry/shape.h>


namespace geometry {

  /** CLASS Rectangle
    * Template params:
    *                 T --> type used for the coordinates
    * A Rectangle is a 2D figure. Additional dimensions will be completed with coordinate value 0.0
    * The Rectangle is centered at the origin
    * Parameters:
    *            - width (default = 1)
    *            - height (default = 1)
    */

  template <typename TYPE = float>
  class Rectangle : public Shape<TYPE>
  {
  public:
    // Default constructor
    Rectangle() : Rectangle(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    // Square constructor
    Rectangle(Length<TYPE> width, const Point3<TYPE>& center) : Rectangle(width, width, center) {}

    // Generic rectangle constructor
    Rectangle(Length<TYPE> width, Length<TYPE> height, const Point3<TYPE>& center) : _width{ width }, _height{ height } {
      this->translate(center.vec());
      Shape<TYPE>::vertices(4);
    }

    ~Rectangle() {};
    
    Length<TYPE> width() const { return _width; }
    Length<TYPE> height() const { return _height; }

    Length<TYPE> perimeter() const override { return Rectangle::perimeter(_width, _height); }
    TYPE area() const override { return Rectangle::area(_width, _height); }
    

    static Length<TYPE> perimeter(Length<TYPE> width, Length<TYPE> height) { return static_cast<TYPE>(2.0) * (width + height); }
    static TYPE area(Length<TYPE> width, Length<TYPE> height) { return  width * height; }
    
    friend std::ostream& operator<<(std::ostream& os, const Rectangle<TYPE>& rec) {
      os << "{\n*** Rectangle W = " << rec.width() << " H = " << rec.height() << " ***" << std::endl;
      os << " Center: " << rec.center();
      os << static_cast<const Shape<TYPE>&>(rec);
      os << "}" << std::endl;

      return os;
    }

    using Shape<TYPE>::vertices;
    

  protected:
    Length<TYPE> _width;
    Length<TYPE> _height;

    virtual void setVerticesAndNormals(size_t num_vertices) override {
      // Calculate the vertices starting from bottom-left (anticlockwise)
      this->_rel_vertices.push_back({ -_width / static_cast<TYPE>(2), -_height / static_cast<TYPE>(2), 0 });
      this->_rel_vertices.push_back({ _width / static_cast<TYPE>(2), -_height / static_cast<TYPE>(2), 0 });
      this->_rel_vertices.push_back({ -this->_rel_vertices[0].vec() });
      this->_rel_vertices.push_back({ -this->_rel_vertices[1].vec() });

      // Normals to the sides, starting with the bottom side
      this->_rel_normals.push_back({ -Vec3<TYPE>::UnitY() });
      this->_rel_normals.push_back({ Vec3<TYPE>::UnitX() });
      this->_rel_normals.push_back({ Vec3<TYPE>::UnitY() });
      this->_rel_normals.push_back({ -Vec3<TYPE>::UnitX() });
    }

    virtual void specificScale(TYPE factor) {
      _width *= factor;
      _height *= factor;
    }


  private:
    // These methods can't be used!
    virtual void vertices(size_t num_vertices) override {};
    TYPE volume() const override { return 0; }

  }; // class Rectangle

} 


#endif // RECTANGLE_H

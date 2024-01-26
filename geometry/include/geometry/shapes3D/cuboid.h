#ifndef CUBOID_H
#define CUBOID_H

#include <geometry/shapes2D/rectangle.h>

namespace geometry {

  /** CLASS Cuboid
    * Template params:
    *                 TYPE --> type used for the coordinates
    * A Cuboid is a 3D figure.
    * The Cuboid is centered at the origin
    * Parameters:
    *            - width (X) (default = 1)
    *            - depth (Y) (default = 1)
    *            - height (Z) (default = 1)
    * The cuboid is defined by 8 vertices but only 6 normals: bottom, 4 sides (-y, x, y and -x) and top
    */

  template <typename TYPE = float>
  class Cuboid : public Shape<TYPE>
  {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");

  public:
    Cuboid() : Cuboid(static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), static_cast<Length<TYPE>>(1), Point3<TYPE>()) {}

    // Cube Constructor
    Cuboid(Length<TYPE> width, const Point3<TYPE>& center = Point3<TYPE>()) : Cuboid(width, width, width, center) {}

    Cuboid(Length<TYPE> width /*X*/, Length<TYPE> depth /*Y*/, Length<TYPE> height /*Z*/, const Point3<TYPE>& center = Point3<TYPE>()) : _width{width}, _depth{ depth }, _height{height} {
      this->_cs.translate(center.vec());
      Shape<TYPE>::vertices(8);
    }

    ~Cuboid(){};

    Length<TYPE> width() const { return _width; }
    Length<TYPE> depth() const { return _depth; }
    Length<TYPE> height() const { return _height; }
        
    TYPE area() const override { return Cuboid::area(_width, _height, _depth); }
    TYPE volume() const override { return Cuboid::volume(_width, _height, _depth); }

    static TYPE area(Length<TYPE> width, Length<TYPE> depth, Length<TYPE> height) { return  static_cast<TYPE>(2.0) * (width * depth + width * height + depth * height); }
    static TYPE volume(Length<TYPE> width, Length<TYPE> depth, Length<TYPE> height) { return  width * depth * height; }

    friend std::ostream& operator<<(std::ostream& os, const Cuboid<TYPE>& cub) {
      os << "{\n *** Cuboid W=" << cub.width() << " D=" << cub.depth() << " H=" << cub.height() << " ***" << std::endl;
      os << " Center: " << cub.center();
      os << static_cast<const Shape<TYPE>&>(cub);
      os << "}" << std::endl;

      return os;
    }

    using Shape<TYPE>::vertices;

  protected:
    Length<TYPE> _width;
    Length<TYPE> _depth;
    Length<TYPE> _height;

    virtual void setVerticesAndNormals(size_t num_vertices) {
      // Calculate the vertices starting from bottom-left (anticlockwise)
      //    Bottom & up Faces
      Rectangle<TYPE> bottom(_width, _depth, this->_cs.center() + Vec3<TYPE>(0, 0, -TYPE(_height) / static_cast<TYPE>(2.0)));

      for (uint8_t i = 0; i < 4; ++i) {
        this->_rel_vertices.push_back({ bottom.vertices()[i] });
      }
      this->_rel_normals.push_back({ -Vec3<TYPE>::UnitZ() });

      bottom.translate(Vec3<TYPE>(0, 0, _height));
      for (uint8_t i = 0; i < 4; ++i) {
        this->_rel_vertices.push_back({ bottom.vertices()[i] });
        this->_rel_normals.push_back({ bottom.normals()[i] });
      }
      this->_rel_normals.push_back({ Vec3<TYPE>::UnitZ() });
    };

    virtual void specificScale(TYPE factor) {
      _width *= factor;
      _depth *= factor;
      _height *= factor;
    }


  private:
    // These methods can't be used!
    virtual void vertices(size_t num_vertices) override {}
    Length<TYPE> perimeter() const override { return 0; }

  }; // class Cuboid

} // namespace geometry


#endif // CUBOID_H

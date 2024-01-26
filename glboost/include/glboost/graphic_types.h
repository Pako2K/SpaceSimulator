
#ifndef GRAPHIC_TYPES_H
#define GRAPHIC_TYPES_H

#include <array>

#include <GL/glew.h>

#include <geometry/basic_types.h>
#include <geometry/point.h>


namespace glboost
{
  using SizePxl = uint16_t;

  // Size in Window Coordinates (Origin in UL corner, Unit = pixel)
  class Size2D 
  {
  public:
    Size2D(SizePxl width, SizePxl height) : _size{ width, height } {}

    SizePxl& w() { return _size[0]; }
    const SizePxl& w() const { return _size[0]; }
    SizePxl& h() { return _size[1]; }
    const SizePxl& h() const { return _size[1]; }

    Size2D operator+(const Size2D& a_size) const { return Size2D(w() + a_size.w(), h() + a_size.h()); }
    Size2D& operator+=(const Size2D & a_size) { 
      w() += a_size.w(); 
      h() += a_size.h(); 
      return *this; 
    }

    Size2D operator-(const Size2D& a_size) const { return Size2D(w() - a_size.w(), h() - a_size.h()); }
    Size2D& operator-=(const Size2D & a_size) { 
      w() -= a_size.w(); 
      h() -= a_size.h(); 
      return *this; 
    }


  protected:
    std::array<SizePxl, 2> _size;
  };


  using PositionPxl = int16_t;

  // Position in Window Coordinates (Origin in UL corner, Unit = pixel)
  class Position2D : public geometry::Point<PositionPxl, 2>
  {
  public:
    using geometry::Point<PositionPxl, 2>::Point;

    Position2D operator+(const Position2D& a_pos) const { return Position2D(x() + a_pos.x(), y() + a_pos.y()); }
    Position2D& operator+=(const Position2D& a_pos) {
      x() += a_pos.x();
      y() += a_pos.y();
      return *this;
    }
    Position2D operator+(const Size2D& a_size) const { return Position2D(x() + a_size.w(), y() + a_size.h()); }
    Position2D& operator+=(const Size2D & a_size) {
      x() += a_size.w();
      y() += a_size.h();
      return *this;
    }
  };
    

  // Color attribute (in scale 0 - 255, with support to 10-bit color attribute)
  using ColorAttr = uint16_t;

  // Color attribute (in scale 0 - 1, with support to 10-bit color attribute)
  using ColorAttrNormalized = float;

  
  template <class COLOR_TYPE, uint8_t DIM>
  class Color : public std::array<COLOR_TYPE, DIM>
  {
    static_assert(DIM == 3 || DIM == 4, "Invalid number of color attributes");
    static_assert(std::is_same<COLOR_TYPE, ColorAttr>::value || std::is_same<COLOR_TYPE, ColorAttrNormalized>::value,
      "Invalid Color Attribute type");

  public:
    Color(COLOR_TYPE red, COLOR_TYPE green, COLOR_TYPE blue)
      : std::array<COLOR_TYPE, DIM>{red, green, blue} {
      static_assert(DIM == 3, "Invalid constructor");
    }

    Color(COLOR_TYPE red, COLOR_TYPE green, COLOR_TYPE blue, COLOR_TYPE opaqueness)
      : std::array<COLOR_TYPE, DIM>{red, green, blue, opaqueness} {
      static_assert(DIM == 4, "Invalid constructor");
    }

    COLOR_TYPE& r() { return this->at(0); }
    const COLOR_TYPE& r() const { return this->at(0); }
    COLOR_TYPE& g() { return this->at(1); }
    const COLOR_TYPE& g() const { return this->at(1); }
    COLOR_TYPE& b() { return this->at(2); }
    const COLOR_TYPE& b() const { return this->at(2); }
    COLOR_TYPE& a() { 
      static_assert(DIM == 4, "Invalid method. Color has no attribute 'a'"); 
      return this->at(3); 
    }
    const COLOR_TYPE& a() const { 
      static_assert(DIM == 4, "Invalid method. Color has no attribute 'a'"); 
      return this->at(3); 
    }

    Color<ColorAttrNormalized, DIM> normalized() const {
      static_assert(std::is_same<COLOR_TYPE, ColorAttr>::value, "Color is already normalized");
      if constexpr(DIM == 3)
        return Color<ColorAttrNormalized, DIM>(ColorAttrNormalized(r()) / 255, ColorAttrNormalized(g()) / 255, ColorAttrNormalized(b()) / 255);
      else
        return Color<ColorAttrNormalized, DIM>(ColorAttrNormalized(r()) / 255, ColorAttrNormalized(g()) / 255, ColorAttrNormalized(b()) / 255, ColorAttrNormalized(a()) / 255);
    }

    Color<ColorAttr, DIM> denormalized() const {
      static_assert(std::is_same<COLOR_TYPE, ColorAttrNormalized>::value, "Color is already de-normalized");
      if constexpr (DIM == 3)
        return Color<ColorAttr, DIM>(ColorAttr(r() * 255), ColorAttr(g() * 255), ColorAttr(b() * 255));
      else
        return Color<ColorAttr, DIM>(ColorAttr(r() * 255), ColorAttr(g() * 255), ColorAttr(b() * 255), ColorAttr(a() * 255));
    }

    Color<COLOR_TYPE, 4> toRGBA(COLOR_TYPE a) const { 
      static_assert(DIM == 3, "Invalid method. Color is already RGBA");
      return Color<COLOR_TYPE, 4>(r(), g(), b(), a); 
    }

    Color<COLOR_TYPE, 3> toRGB() const {
      static_assert(DIM == 4, "Invalid method. Color is already RGB");
      return Color<COLOR_TYPE, 3>(r(), g(), b());
    }
  };

  using Color4i = Color<ColorAttr, 4>;
  using Color3i = Color<ColorAttr, 3>;
  using Color4f = Color<ColorAttrNormalized, 4>;
  using Color3f = Color<ColorAttrNormalized, 3>;


  class NormalizedCoordinates : protected geometry::Point<float, 2>
  {
  public:
    static const Size2D& viewportSize() {
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      _viewport_size.w() = viewport[2];
      _viewport_size.h() = viewport[3];
      return _viewport_size;
    }

    // Transform from window coordinates to normalized coordinates
    NormalizedCoordinates(const Position2D& screen_coord) : geometry::Point<float, 2>{ 2.0f * screen_coord.x() / _viewport_size.w() - 1 , -2.0f * screen_coord.y() / _viewport_size.h() + 1 } {}

    using geometry::Point<float, 2>::x;
    using geometry::Point<float, 2>::y;

    using geometry::Point<float, 2>::data;


  private:
    inline static Size2D _viewport_size{ viewportSize() };
  };
}


#endif // GRAPHIC_TYPES_H


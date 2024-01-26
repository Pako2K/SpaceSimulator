#ifndef POINT_H
#define POINT_H

#include <array>
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <ostream>

#include <geometry/basic_types.h>


namespace geometry {

  /**
    * \brief  Class representing a point
    */
  template <typename TYPE, uint8_t DIM>
  class Point
  {
    static_assert(std::is_arithmetic<TYPE>::value, "Class type must be a number type");
    static_assert(DIM > 0, "Number of dimensions must be at least 1");

  public:
    // Default constructor
    Point() {
      for (uint8_t i = 0; i < DIM; ++i)
        _coord[i] = 0;
    }

    // 2D specialized constructor
    Point(TYPE x, TYPE y) {
      static_assert(DIM == 2, "Constructor cannot be used: number of dimensions must be 2");
      _coord[Axis::X] = x;
      _coord[Axis::Y] = y;
    }

    // 3D specialized constructor
    Point(TYPE x, TYPE y, TYPE z) {
      static_assert(DIM == 3, "Constructor cannot be used: number of dimensions must be 3");
      _coord[Axis::X] = x;
      _coord[Axis::Y] = y;
      _coord[Axis::Z] = z;
    }

    // Initializer list based constructor 
    Point(std::initializer_list<TYPE> coords) {
      if (coords.size() > DIM)
        throw std::out_of_range("Too many coordinates");

      uint8_t i = 0;
      for (auto c : coords)
        _coord[i++] = c;
      while (i < DIM)
        _coord[i++] = 0;
    }

    // Constructor based on position vector 
    Point(const Vec<TYPE, DIM> & vector) {
      for (uint8_t i = 0; i < DIM; ++i)
        _coord[i] = vector[i];
    }


    // Constructor based on iterators
    template <typename InputIterator,
      typename = typename std::enable_if
      <std::is_convertible
      <typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value
      >::type
    >
      Point(InputIterator first, InputIterator last) {
      uint8_t i{ 0 };
      while (first != last && i < DIM) {
        _coord[i++] = *first;
        ++first;
      }
    }

    // Destructor
    ~Point() {}


    // Adds a vector to the point and returns the new point
    Point<TYPE, DIM> operator+(const Vec<TYPE, DIM> & vector) const {
      Point<TYPE, DIM> ret;
      for (uint8_t i = 0; i < DIM; ++i)
        ret[i] = _coord[i] + vector[i];
      return ret;
    }

    // Adds a vector to this point 
    Point<TYPE, DIM>& operator+=(const Vec<TYPE, DIM> & vector) {
      for (uint8_t i = 0; i < DIM; ++i)
        _coord[i] += vector[i];
      return *this;
    }

    // Substracts a point from the point and returns a vector
    Vec<TYPE, DIM> operator-(const Point<TYPE, DIM> & point) const {
      Vec<TYPE, DIM> ret;
      for (uint8_t i = 0; i < DIM; ++i)
        ret[i] = _coord[i] - point[i];
      return ret;
    }

    // Substracts a vector from the point and returns the new point
    Point<TYPE, DIM> operator-(const Vec<TYPE, DIM> & vector) const {
      Point<TYPE, DIM> ret;
      for (uint8_t i = 0; i < DIM; ++i)
        ret[i] = _coord[i] - vector[i];
      return ret;
    }

    // Substracts a vector from this point 
    Point<TYPE, DIM>& operator-=(const Vec<TYPE, DIM> & vector) {
      for (uint8_t i = 0; i < DIM; ++i)
        _coord[i] -= vector[i];
      return *this;
    }


    // Asigns a vector to this point 
    Point<TYPE, DIM>& operator=(const Vec<TYPE, DIM> & vector) {
      for (uint8_t i = 0; i < DIM; ++i)
        _coord[i] = vector[i];
      return *this;
    }

    // Comparison
    bool operator==(const Point<TYPE, DIM> & point) const {
      for (uint8_t i = 0; i < DIM; ++i) {
        if (_coord[i] != point._coord[i])
          return false;
      }
      return true;
    }

    // Comparison
    bool operator!=(const Point<TYPE, DIM> & point) const {
      return !operator==(point);
    }

    // Indexed access operator
    TYPE& operator[](uint8_t pos) { return _coord[pos]; }

    // Indexed access operator
    const TYPE& operator[](uint8_t pos) const { return _coord[pos]; }

    TYPE x() const { return _coord[Axis::X]; }

    TYPE& x() { return _coord[Axis::X]; }

    TYPE y() const {
      static_assert(DIM >= 2, "Method cannot be used: number of dimensions must be at least 2");
      return _coord[Axis::Y];
    }

    TYPE& y() {
      static_assert(DIM >= 2, "Method cannot be used: number of dimensions must be at least 2");
      return _coord[Axis::Y];
    }

    TYPE z() const {
      static_assert(DIM >= 3, "Method cannot be used: number of dimensions must be at least 3");
      return _coord[Axis::Z];
    }

    TYPE& z() {
      static_assert(DIM >= 3, "Method cannot be used: number of dimensions must be at least 3");
      return _coord[Axis::Z];
    }

    // Returns the position vector
    Vec<TYPE, DIM> vec() const {
      Vec<TYPE, DIM> ret;
      for (uint8_t i = 0; i < DIM; ++i)
        ret[i] = _coord[i];
      return ret;
    }

    // Returns the position vector
    operator Vec<TYPE, DIM>() const {
      Vec<TYPE, DIM> ret;
      for (uint8_t i = 0; i < DIM; ++i)
        ret[i] = _coord[i];
      return ret;
    }

    // Returns the number of dimensions
    constexpr uint8_t size() const { return DIM; }

    // Returns pointer to the array of coordinates
    const TYPE* data() const { return _coord.data(); }

    // Get the distance to another point
    Length<TYPE> distance(Point<TYPE, DIM> a_point) const { return (a_point - *this).norm(); }

    // Print the point coordinates
    friend std::ostream& operator<<(std::ostream & os, const Point<TYPE, DIM> & p) {
      os << "{ ";
      uint8_t i;
      for (i = 0; i < DIM - 1; ++i)
        os << p._coord[i] << ", ";
      os << p._coord[i] << " } " << std::endl;

      return os;
    }


  protected:
    std::array<TYPE, DIM> _coord;

  }; // class Point

  /**
    *  \brief Alias for 3D Point
    */
  template <typename TYPE>
  using Point3 = Point<TYPE, 3>;
}

#endif // POINT_H

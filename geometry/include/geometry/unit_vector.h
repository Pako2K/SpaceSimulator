#ifndef UNIT_VECTOR_H
#define UNIT_VECTOR_H

#include <type_traits>
#include <iterator>
#include <ostream>
#include <stdexcept>

#include <geometry/basic_types.h>


namespace geometry {
  /**
    * \brief  Class representing a unit vector
    *         A unit vector cannot be transformed or changed since it might not be a unit vector anymore.
    */
  template <typename TYPE, uint8_t DIM>
  class UnitVec
  {
    static_assert(std::is_arithmetic<TYPE>::value, "Class type must be a number type");
    static_assert(DIM > 0, "Number of dimensions must be at least 1");
  public:
    // 2D specialized constructor
    UnitVec(TYPE x, TYPE y) : _vector{ x, y } {
      static_assert(DIM == 2, "Constructor cannot be used: number of dimensions must be 2");
      if (_vector.norm() > 0)
        _vector.normalize();
      else
        throw std::invalid_argument("Null vector cannot be transformed to unit vector");
    }

    // 3D specialized constructor
    UnitVec(TYPE x, TYPE y, TYPE z) : _vector{ x, y, z } {
      static_assert(DIM == 3, "Constructor cannot be used: number of dimensions must be 3");
      if (_vector.norm() > 0)
        _vector.normalize();
      else
        throw std::invalid_argument("Null vector cannot be transformed to unit vector");
    }

    // Initializer list based constructor 
    UnitVec(std::initializer_list<TYPE> coords) {
      if (coords.size() > DIM)
        throw std::out_of_range("Too many coordinates");

      uint8_t i = 0;
      for (auto c : coords)
        _vector[i++] = c;
      while (i < DIM)
        _vector[i++] = 0;

      if (_vector.norm() > 0)
        _vector.normalize();
      else
        throw std::invalid_argument("Null vector cannot be transformed to unit vector");
    }

    // Constructor based on vector 
    UnitVec(const Vec<TYPE, DIM> & vector) : _vector{ vector } {
      if (_vector.norm() > 0)
        _vector.normalize();
      else
        throw std::invalid_argument("Null vector cannot be transformed to unit vector");
    }

    // Constructor based on iterators
    template <typename InputIterator,
      typename = typename std::enable_if
      <std::is_convertible
      <typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value
      >::type
    >
      UnitVec(InputIterator first, InputIterator last) {
      uint8_t i{ 0 };
      while (first != last && i < DIM) {
        _vector[i++] = *first;
        ++first;
      }

      if (_vector.norm() > 0)
        _vector.normalize();
      else
        throw std::invalid_argument("Null vector cannot be transformed to unit vector");
    }


    Vec<TYPE, DIM>& operator=(const Vec<TYPE, DIM> & vec) {
      _vector = vec;
      _vector.normalize();
      return _vector;
    }

    Vec<TYPE, DIM> vec() const { return _vector; }

    // Returns the number of dimensions
    constexpr uint8_t size() const { return DIM; }

    bool operator==(const UnitVec & unit_vec) const { return _vector == unit_vec.vec(); }
    bool operator==(const Vec<TYPE, DIM> & vec) const { return _vector == vec; }

    bool operator!=(const UnitVec & unit_vec) const { return _vector != unit_vec.vec(); }
    bool operator!=(const Vec<TYPE, DIM> & vec) const { return _vector != vec; }

    Vec<TYPE, DIM> operator+(const UnitVec & unit_vec) const { return _vector + unit_vec.vec(); }
    Vec<TYPE, DIM> operator+(const Vec<TYPE, DIM> & vec) const { return _vector + vec; }

    UnitVec<TYPE, DIM> operator-() const { return UnitVec(-_vector); }

    Vec<TYPE, DIM> operator-(const UnitVec & unit_vec) const { return _vector - unit_vec.vec(); }
    Vec<TYPE, DIM> operator-(const Vec<TYPE, DIM> & vec) const { return _vector - vec; }

    Vec<TYPE, DIM> operator*(TYPE number) const { return number * _vector; }
    friend Vec<TYPE, DIM> operator*(TYPE number, const UnitVec<TYPE, DIM> & unit_vec) { return number * unit_vec._vector; }

    TYPE operator[](size_t pos) const { return _vector[pos]; }

    TYPE* data() const { return _vector.data(); }

    Vec<TYPE, DIM> cross(const UnitVec & unit_vec) const { return _vector.cross(unit_vec.vec()); }
    Vec<TYPE, DIM> cross(const Vec<TYPE, DIM> & vec) const { return _vector.cross(vec); }

    TYPE dot(const UnitVec & unit_vec) const { return _vector.dot(unit_vec.vec()); }
    TYPE dot(const Vec<TYPE, DIM> & vec) const { return _vector.dot(vec); }

    TYPE x() const { return _vector.x(); }

    TYPE y() const { return _vector.y(); }

    TYPE z() const { return _vector.z(); }

    // Print the unit vector coordinates
    friend std::ostream& operator<<(std::ostream & os, const UnitVec<TYPE, DIM> & unit_vec) {
      os << "{ ";
      uint8_t i;
      for (i = 0; i < DIM - 1; ++i)
        os << unit_vec[i] << ", ";
      os << unit_vec[i] << " } " << std::endl;

      return os;
    }


  protected:
    Vec<TYPE, DIM> _vector;
  };

  /**
    *  \brief Alias for 3D Unit Vector
    */
  template <typename TYPE>
  using UnitVec3 = UnitVec<TYPE, 3>;

}

#endif // UNIT_VECTOR_H

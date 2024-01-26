#ifndef GEOMETRY_BASIC_TYPES_H
#define GEOMETRY_BASIC_TYPES_H

#include <array>
#include <type_traits>
#include <iterator>
#include <ostream>

#include <Eigen/Core>

#include <geometry/constants.h>


namespace geometry {
  /**
    * \brief  Enumeration of 3D axes names, including "negative" ones
    */
  enum Axis : uint8_t  { X, Y, Z };

  /**
    * \brief  Length type, representing an unidimensional size (a positive real number)
    */
  template <typename TYPE = float>
  class Length
  {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");
  public:
    Length() {}
    Length(TYPE a_length) : _length{ a_length } {
      if (_length < 0) 
        throw std::invalid_argument("Size must be positive"); 
    }

    operator TYPE() const { return _length; }

    Length<TYPE>& operator= (TYPE new_length) {
      _length = new_length;
      return *this;
    }

    TYPE operator* (Length<TYPE> a_length) {
      return _length * a_length._length;
    }

    template <typename NUM_TYPE>
    Length<TYPE> operator* (NUM_TYPE factor) {
      static_assert(std::is_arithmetic<NUM_TYPE>::value, "Type must be a numeric type");
      return Length<TYPE>(static_cast<TYPE>(_length * factor));
    }

    template <typename NUM_TYPE>
    friend Length<TYPE> operator* (NUM_TYPE factor, Length<TYPE> a_length) {
      static_assert(std::is_arithmetic<NUM_TYPE>::value, "Type must be a numeric type");
      return Length<TYPE>(static_cast<TYPE>(a_length._length * factor));
    }

    template <typename NUM_TYPE>
    Length<TYPE>& operator*= (NUM_TYPE factor) {
      static_assert(std::is_arithmetic<NUM_TYPE>::value, "Type must be a numeric type");
      if (factor < 0)
        throw std::invalid_argument("Multiplier must be positive");
      _length *= factor;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Length<TYPE>& a_length) {
      os << a_length._length;
      return os;
    }


  protected:
    TYPE _length{ 0 };
  };



  /**
    * \brief  Angle Type, representing an angle in radians
    */
  template <typename TYPE = float>
  class Angle
  {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");
  public:
    Angle(){}
    Angle(TYPE angle) : _angle{angle} {}

    // ****** Casting Operators *************************************
    operator TYPE() const { return _angle; }
    TYPE operator() () const { return _angle; }
    Angle<TYPE>& operator= (TYPE new_angle) {
      _angle = new_angle;
      return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Angle<TYPE>& angle) {
      os << angle._angle;
      return os;
    }
    // *****************************************************************

    // ****** Arithmetic Operators *************************************
    friend Angle<TYPE> operator+(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return Angle<TYPE>(a1._angle + a2._angle); }
    friend Angle<TYPE> operator+=(Angle<TYPE>& a1, const Angle<TYPE>& a2) { 
      a1._angle += a2._angle;
      return a1;
    }
    friend Angle<TYPE> operator-(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return Angle<TYPE>(a1._angle - a2._angle); }
    friend Angle<TYPE> operator-=(Angle<TYPE> & a1, const Angle<TYPE> & a2) {
      a1._angle -= a2._angle;
      return a1;
    }
    // *****************************************************************

    // ****** Comparison Operators *************************************
    friend bool operator==(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle == a2._angle; }
    friend bool operator!=(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle != a2._angle; }
    friend bool operator>(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle > a2._angle; }
    friend bool operator<(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle < a2._angle; }
    friend bool operator>=(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle >= a2._angle; }
    friend bool operator<=(const Angle<TYPE>& a1, const Angle<TYPE>& a2) { return a1._angle <= a2._angle; }
    // *****************************************************************

    // Returns an angle between 0 and 2*PI
    Angle& reduce() {
      _angle = reduced();
      return *this;
    }

    // Returns an angle between 0 and 2*PI
    TYPE reduced() const {
      if (_angle < 0)
        return TWO_PI + std::fmod(_angle, TWO_PI);
      else
        return std::fmod(_angle, TWO_PI);
    }

    // Returns the angle in degrees
    TYPE degrees() const { return _angle * RAD_2_DEG; }

    
  private:
    TYPE _angle{ 0 };
  };
  
  // Converts the angle from degrees to radians
  template <typename TYPE>
  Angle<TYPE> radians(TYPE angle_degrees) { return Angle<TYPE>(angle_degrees / RAD_2_DEG); }


  
  /**
  * \brief  Alias for a Vector
  */
  template <typename T, uint8_t DIM>
  using Vec = Eigen::Matrix<T, DIM, 1>;

  /**
    * \brief  Alias for a 3D Vector
    */
  template <typename T>
  using Vec3 = Eigen::Matrix<T, 3, 1>;

  /**
    * \brief  Alias for a matrix NxN
    */
  template <typename T, uint8_t DIM>
  using Mat = Eigen::Matrix<T, 3, 3>;

  /**
    * \brief  Alias for a matrix 3x3
    */
  template <typename T>
  using Mat3 = Eigen::Matrix<T, 3, 3>;

  /**
    * \brief  Alias for a matrix 4x4
    */
  template <typename T>
  using Mat4 = Eigen::Matrix<T, 4, 4>;

  /**
    * \brief  Alias for a homogeneous transformation matrix 4x4
    */
  template <typename T>
  using Transf = Eigen::Transform<T, 3, Eigen::Affine>;

}

#endif // GEOMETRY_BASIC_TYPES_H

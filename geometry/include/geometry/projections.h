#ifndef PROJECTIONS_H
#define PROJECTIONS_H

#include <Eigen/Geometry>

#include <geometry/basic_types.h>


namespace geometry {

  /**
    * Orthogonal projection
    * (Assumes that the frustrum input parameters are in the observer CS, except near and far which are absolute values in the direction of the view,
    *  which goes in the negative Z direction)
    */
  template <typename TYPE>
  inline Eigen::Matrix<TYPE, 4, 4> orthoProjection(TYPE left, TYPE right, TYPE bottom, TYPE top, TYPE near, TYPE far);

  /**
    * Perspective projection matrix
    * Input:
    *     field_angle: vertical field of view (in radians)
    *     field_ratio: field of view ratio (horizontal/vertical)
    *     near and far: absolute values in the direction of the view, which goes in the negative Z direction in the observer CS
    */
  template <typename TYPE>
  inline Eigen::Matrix<TYPE, 4, 4> perspectiveProjection(Angle<TYPE> field_angle, TYPE field_ratio, TYPE near, TYPE far);





  template <typename TYPE>
  inline Eigen::Matrix<TYPE, 4, 4> orthoProjection(TYPE left, TYPE right, TYPE bottom, TYPE top, TYPE near, TYPE far) {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");

    Eigen::Matrix<TYPE, 4, 4> ortho;

    // Column major
    ortho << static_cast<TYPE>(2) / (right - left),                  0,                                0,           - (right + left) / (right - left),
                        0,                      static_cast<TYPE>(2) / (top - bottom),                 0,           - (top + bottom) / (top - bottom),
                        0,                                        0,              -static_cast<TYPE>(1) / (far - near),    - near / (far - near),
                        0,                                        0,                                0,                   static_cast<TYPE>(1);

    return ortho;
  }


  template <typename TYPE>
  inline Eigen::Matrix<TYPE, 4, 4> perspectiveProjection(Angle<TYPE> field_angle, TYPE field_ratio, TYPE near, TYPE far) {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");
    if( field_ratio < static_cast<TYPE>(1) )
        throw std::invalid_argument("Field of view ratio must be grater than 1.");

    Eigen::Matrix<TYPE, 4, 4> perspective;
    TYPE tan_half_field = tan(field_angle / static_cast<TYPE>(2));

    perspective << static_cast<TYPE>(1) / (field_ratio * tan_half_field),        0,                                0,                           0,
                                  0,                        static_cast<TYPE>(1) / (tan_half_field),               0,                           0,
                                  0,                                          0,                      -far / (far - near),      - near * far / (far - near),
                                  0,                                          0,                      -static_cast<TYPE>(1),                    0;

    return perspective;
  }


} // namespace geometry


#endif // PROJECTIONS_H

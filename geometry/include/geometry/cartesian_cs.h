#ifndef CARTESIAN_CS_H
#define CARTESIAN_CS_H


#include <type_traits>
#include <stdexcept>
#include <limits>

#include <Eigen/Geometry> 

#include <geometry/basic_types.h>
#include <geometry/point.h>
#include <geometry/unit_vector.h>


namespace geometry {
   
  /**
    * \brief  Cartesian Coordinates System in 3D.
    *         Contains the position and the 3 normalized orthogonal axes of a coordinate system relative to the default Cartesian CS
    *         Provides several methods to retrieve transformation matrices
    */
  template <typename TYPE = float>
  class CartesianCS
  {
    static_assert(std::is_floating_point<TYPE>::value, "TYPE must be a floating point type");
    static const uint8_t  DIM{ 3 };
  public:

    // Default cartesian CS axis
    static inline std::array<UnitVec3<TYPE>, 3> defCartAxis{ { {static_cast<TYPE>(1), 0, 0}, {0, static_cast<TYPE>(1), 0} , {0, 0, static_cast<TYPE>(1)} } };
  
    
    /// ***********************************************************************************************************************
    /// *********************************************** Operations ************************************************************

    /**
      *  \brief Constructor of a DEFAULT CARTESIAN COORDINATE SYSTEM centered at the given point
      *  @param
      */
    CartesianCS(const Point3<TYPE>& center = Point3<TYPE>(0, 0, 0));


    /**
      *  \brief Constructor of an ARBITRARY CARTESIAN COORDINATE SYSTEM centered in a given point
      *         Based on 2 given orthogonal vectors (e.g. the internal axis "X" and "Y") and a point. The 2 vectors don't have to be normalized.
      *         The axis Z is calculated.
      *  @param  axis_name_1  the name of the first provided axis
      *  @param  axis_1       the coordinates of the vector aligned with the axis "axis_name_1" of this CS
      *  @param  axis_name_2  the name of the second provided axis
      *  @param  axis_2       the coordinates of the vector aligned with the axis "axis_name_2" of this CS
      *  @param  center the coordinates of the point where this CS is centered, relative to the default CS
      *  @throw  invalid_argument if the provided vectors are not orthogonal within a predefined ACCURACY FACTOR of the Type precision
      */
    CartesianCS(Axis axis_name_1, const Vec3<TYPE>& axis_1, Axis axis_name_2, const Vec3<TYPE>& axis_2, const Point3<TYPE> &center = Point3<TYPE>(0, 0, 0));


    /**
      *  \brief Destructor
      */
    ~CartesianCS() {}


    /**
      *  \brief  Returns the center in the default CS
      *  @param
      *  @return a point with the center coordinates
      */
    inline const Point3<TYPE>& center() const;

    /**
      *  \brief  Returns the axis vector in the default CS
      *  @param  index of the axis
      *  @return a vector with the axis coordinates in the default CS
      *  @throw  an exception if the index is not 0, 1 or 2
      */
    inline const UnitVec3<TYPE>& operator[](uint8_t pos) const;

    /**
      *  \brief  Returns the transformation matrix to go from coordinates in the default CS to this coordinates system.
      *  @return the affine transformation matrix
      */
    inline const Transf<TYPE>& transfMatrix() const;

    /**
      *  \brief  Returns the Alfa Euler angle between the default CS and this CS.
      *  @return the Alfa Euler angle
      */
    inline const Angle<TYPE> eulerAlfa() { updateEulerAngles(); return _alfa; }

    /**
      *  \brief  Returns the Beta Euler angle between the default CS and this CS.
      *  @return the Beta Euler angle
      */
    inline const Angle<TYPE> eulerBeta() { updateEulerAngles(); return _beta; }

    /**
      *  \brief  Returns the Gamma Euler angle between the default CS and this CS.
      *  @return the Gamma Euler angle
      */
    inline const Angle<TYPE> eulerGamma() { updateEulerAngles(); return _gamma; }

    /**
      *  \brief Translates the center of the CS to a new point, updating the transformation matrix
      *  @param  center   the coordinates of the point where this CS is re-centered, relative to the default WCS
      */
    inline void translate(const Point3<TYPE>& new_center);

    /**
      *  \brief Translates the center of the CS by the specified distance in the diff vector, updating the transformation matrix
      *  @param  diff   vector containing the coordinate distances to the new center, relative to the default WCS
      */
    inline void translate(const Vec3<TYPE>& diff);

    /**
      *  \brief  (Intrinsic) Rotation of the CS around a cartesian axis of this CS intersecting the center of this CS. 
      *  @param  cart_axis  Cartesian axis of this CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void rotateInt(Axis cart_axis, Angle<TYPE> angle);

    /**
      *  \brief  (Intrinsic) Rotation of the CS around an arbitrary axis represented in this CS coordinates and intersecting the center of this CS.
      *  @param  axis  Axis of this CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void rotateInt(const Vec3<TYPE>& axis, Angle<TYPE> angle);

    /**
      *  \brief  (Extrinsic) Rotation of the CS around an arbitrary axis in the default CS and the center of this CS
      *  @param  axis  Axis in the default CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void rotateExt(const Vec3<TYPE>& axis, Angle<TYPE> angle);

    /**
      *  \brief  Rotation of the CS center around one of the cartesian axis of this CS but intersecting the center of the reference CS
      *          For the CS axes there is no change. This is just a translation of the CS center.
      *  @param  cart_axis  Cartesian axis of the CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void revolveInt(Axis cart_axis, Angle<TYPE> angle);

    /**
      *  \brief  Rotation of the CS center around an arbitrary axis of this CS but intersecting the center of the reference CS
      *          For the CS axes there is no change. This is just a translation of the CS center.
      *  @param  axis  Axis of the CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void revolveInt(const Vec3<TYPE>& axis, Angle<TYPE> angle);

    /**
      *  \brief  Rotation of the CS center around an arbitrary axis of the reference CS intersecting the center of the reference CS
      *          For the CS axes there is no change. This is just a translation of the CS center.
      *  @param  axis  Axis in the reference CS around which the rotation is applied
      *  @param  angle Rotation angle
      */
    inline void revolveExt(const Vec3<TYPE>& axis, Angle<TYPE> angle);

    /**
      *  \brief  Rotation of this CS center around an axis of the reference CS intersecting the reference point. 
      *          For the CS axes there is no change. This is just a translation of the CS center.
      *  @param  ref_point  Reference point for the rotation in the reference CS coordinates
      *  @param  axis       Axis in the reference CS around which the rotation is applied
      *  @param  angle      Rotation angle
      */
    inline void revolveExt(Point3<TYPE> ref_point, const Vec3<TYPE>& axis, Angle<TYPE> angle);

    /**
      *  \brief Re-aligns the axes according to the 2 specified ones, updating the transformation matrix
      *  @param  axis_name_1  the name of the first provided axis
      *  @param  axis_1       the coordinates of the vector aligned with the axis "axis_name_1" of this CS
      *  @param  axis_name_2  the name of the second provided axis
      *  @param  axis_2       the coordinates of the vector aligned with the axis "axis_name_2" of this CS
      *  @throw  invalid_argument if the provided vectors are not orthogonal within a predefined ACCURACY FACTOR of the Type precision
      */
    inline void alignTo(Axis axis_name_1, const Vec3<TYPE>& axis_1, Axis axis_name_2, const Vec3<TYPE>& axis_2);

    /**
      *  \brief  Friend insertion operator. Prints in the specified output stream the info about a  Cartesian CS
      *  @param   os  a reference to an output stream object
      *  @param   cs  a reference to a Cartesian Coordinates System. The Euler angles will be updated.
      *  @return  the reference to the output stream
      */
    friend std::ostream& operator<<(std::ostream& os, CartesianCS<TYPE>& cs) {
      os << "{ " << std::endl;
      os << "  CENTER: { ";
      uint8_t i;
      for (i = 0; i < DIM - 1; ++i) {
        os << cs._center[i] << ", ";
      }
      os << cs._center[i] << " }" << std::endl;
      os << "  AXES: " << std::endl;
      for (i = 0; i < DIM; ++i) {
        os << "  { ";
        uint8_t j;
        for (j = 0; j < DIM - 1; ++j)
          os << cs[i][j] << ", ";
        os << cs[i][j] << " }" << std::endl;
      }
      // Call one the get methods to make sure that the angles are updated
      os << "  EULER ANGLES: " << std::endl;
      os << "    alfa (X - Line of Nodes) = " << cs.eulerAlfa() << std::endl;
      os << "    beta (Z - Z*) = " << cs._beta << std::endl;
      os << "    gamma (Line of Notes - X*) = " << cs._gamma << std::endl;
      os << "}" << std::endl;

      return os;
    }
    
    /// *********************************************** Operations (END) ******************************************************
    /// ***********************************************************************************************************************

  private:
    /// ***********************************************************************************************************************
    /// ******************************************** Data Members *************************************************************

    /**
      * \brief  Center of this Cartesian CS relative to the default one
      */
    Point3<TYPE>  _center;

    /**
      * \brief  Axes of this Cartesian CS relative to the default one
      */
    std::array<UnitVec3<TYPE>, DIM> _axes{ { {Vec3<TYPE>::UnitX()}, {Vec3<TYPE>::UnitY()}, {Vec3<TYPE>::UnitZ()} } };

    /**
      * \brief  Affine transformation matrix to go from Coordinates in the default CS to coordinates in this Cartesian CS.
      *         It's a combination of a rotation plus a translation
      */
    Transf<TYPE>  _transf_matrix;

    /**
      * \brief  Euler angle alfa: angle between the default CS axis X and the line of nodes (i.e. the intersection of the XY planes of both CS's)
      *         This can be considered the first rotation around the default CS Z axis
      */
    Angle<TYPE> _alfa{ 0 };

    /**
      * \brief  Euler angle beta: angle between the default CS Z and this CS axis Z
      *         This can be considered the second rotation around the line of nodes, or around the default CS X axis
      */
    Angle<TYPE> _beta{ 0 };

    /**
      * \brief  Euler angle gamma: angle between the line of nodes and this CS axis X
      *         This can be considered the third rotation around this CS axis Z, or around the default CS Z axis
      */
    Angle<TYPE> _gamma{ 0 };

    /**
      * \brief  This flag indicates wheter the Euler angles are up-to-date
      *         For performance reasons the Euler angles are only calculated when retrieved
      */
    bool _eulerUpdated{ true };

    /// ******************************************* Data Members (END) ********************************************************
    /// ***********************************************************************************************************************

    /// ***********************************************************************************************************************
    /// *********************************************** Operations ************************************************************
    void updateEulerAngles() {
      if (_eulerUpdated)
        return;

      // The line of nodes is given by the cross product of the 2 Z axes. 
      Vec3<TYPE> lineOfNodes = Vec3<TYPE>::UnitZ().cross(_axes[Axis::Z].vec());
      if (lineOfNodes.norm() <= std::numeric_limits<TYPE>::epsilon()) {
        _alfa = 0;
        lineOfNodes = Vec3<TYPE>::UnitX();
      }
      else {
        lineOfNodes.normalize();
        // The angle between the X axis and the line of nodes is alfa. X.dot(N) = cos(alfa) ==> N.x = cos(alfa)
        _alfa = acos(lineOfNodes[0]);

        // An the sign can be determined by the component Y of the line of nodes
        if (lineOfNodes[1] < 0)
          _alfa = -_alfa;
      }
      // The easiest angle to calculate is the angle between the 2 Z axes, beta. Z.dot(Z') = cos(beta) ==> Z'.z = cos(beta)
      _beta = acos(_axes[2][2]);

      // The angle between the line of nodes and this CS X axis is gamma. N.dot(X') = cos(gamma) 
      _gamma = acos(lineOfNodes.dot(_axes[Axis::X].vec()));
      // An the sign can be determined by the component Z of the X' axis
      if (_axes[0][2] < 0)
        _gamma = -_gamma;
      
      _eulerUpdated = true;
    }

    /// *********************************************** Operations (END) ******************************************************
    /// ***********************************************************************************************************************

  }; // class CartesianCS

  

  /* ################################################ TEMPLATE IMPLEMENTATION ######################################################*/
  /* ###############################################################################################################################*/

  #define ACCURACY_FACTOR   100.0

  #define SET_TRANSF_MATRIX _transf_matrix.matrix() << _axes[0].x(),     _axes[0].y(),      _axes[0].z(),      - _axes[0].dot(_center.vec()), \
                                                       _axes[1].x(),     _axes[1].y(),      _axes[1].z(),      - _axes[1].dot(_center.vec()), \
                                                       _axes[2].x(),     _axes[2].y(),      _axes[2].z(),      - _axes[2].dot(_center.vec()), \
                                                             0,                0,                 0,             static_cast<TYPE>(1)
  
  /// CONSTRUCTORS
  template <typename TYPE>
  CartesianCS<TYPE>::CartesianCS(const Point3<TYPE>& center) : _center{ center } {
    // Calculate and store the transformation matrix
    SET_TRANSF_MATRIX;
  }

  template <typename TYPE>
  CartesianCS<TYPE>::CartesianCS(Axis axis_name_1, const Vec3<TYPE>& axis_1, Axis axis_name_2, const Vec3<TYPE>& axis_2, const Point3<TYPE> &center) : _center{ center } {
    alignTo(axis_name_1, axis_1, axis_name_2, axis_2);
  }
      
  /// center()
  template <typename TYPE>
  const Point3<TYPE>& CartesianCS<TYPE>::center() const {
    return _center;
  }

  /// operator[]
  template <typename TYPE>
  const UnitVec3<TYPE>& CartesianCS<TYPE>::operator[](uint8_t pos) const {
    return _axes.at(pos);
  }

  /// transfMatrix()
  template <typename TYPE>
  const Transf<TYPE>& CartesianCS<TYPE>::transfMatrix() const {
    return _transf_matrix;
  }

  /// translate(const Point& new_center)
  template <typename TYPE>
  void CartesianCS<TYPE>::translate(const Point3<TYPE>& new_center) {
    _center = new_center;

    // Calculate and store the transformation matrix
    for (uint8_t i = 0; i < DIM; ++i)
      _transf_matrix(i, 3) = -_axes[i].dot(_center.vec());
  }

  /// translate(const Vec3& diff)
  template <typename TYPE>
  void CartesianCS<TYPE>::translate(const Vec3<TYPE>& diff) {
    translate(_center + diff);
  }
  
  /// rotateInt
  template <typename TYPE>
  void CartesianCS<TYPE>::rotateInt(Axis cart_axis, Angle<TYPE> angle) {
    rotateExt(_axes[cart_axis].vec(), angle);
  }

  /// rotateInt
  template <typename TYPE>
  void CartesianCS<TYPE>::rotateInt(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
    // Calculate axis in the reference CS
    const Vec3<TYPE> ext_axis{ axis.x() * _axes[Axis::X] + axis.y() * _axes[Axis::Y] + axis.z() * _axes[Axis::Z] };
    rotateExt(ext_axis, angle);
  }

  ///  rotateExt
  template <typename TYPE>
  void CartesianCS<TYPE>::rotateExt(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
    Eigen::AngleAxis<TYPE> rotation_matrix(angle, axis);

    Vec3<TYPE> new_axis_x = rotation_matrix * _axes[Axis::X].vec();
    Vec3<TYPE> new_axis_y = rotation_matrix * _axes[Axis::Y].vec();

    alignTo(Axis::X, new_axis_x, Axis::Y, new_axis_y);
  }

  /// revolveInt
  template <typename TYPE>
  void CartesianCS<TYPE>::revolveInt(Axis cart_axis, Angle<TYPE> angle) {
    Eigen::AngleAxis<TYPE> rotation_matrix(angle, _axes[cart_axis].vec());
    translate(Point3<TYPE>(rotation_matrix * _center.vec()));
  }

  /// revolveInt
  template <typename TYPE>
  void CartesianCS<TYPE>::revolveInt(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
    // Calculate axis in the reference CS
    const Vec3<TYPE> ext_axis{ axis.x() * _axes[Axis::X] + axis.y() * _axes[Axis::Y] + axis.z() * _axes[Axis::Z] };
    Eigen::AngleAxis<TYPE> rotation_matrix(angle, ext_axis);
    translate(Point3<TYPE>(rotation_matrix * _center.vec()));
  }

  /// revolveExt
  template <typename TYPE>
  void CartesianCS<TYPE>::revolveExt(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
    Eigen::AngleAxis<TYPE> rotation_matrix(angle, axis);
    translate(Point3<TYPE>(rotation_matrix * _center.vec()));
  }

  /// revolveExt
  template <typename TYPE>
  void CartesianCS<TYPE>::revolveExt(Point3<TYPE> ref_point, const Vec3<TYPE>& axis, Angle<TYPE> angle) {
    // Translate to a position which equals the relative position with the reference point
    translate(-ref_point.vec());
    // Execute rotation around the center
    revolveExt(axis, angle);
    // Undo the first translation
    translate(ref_point.vec());
  }


  /// alignTo(CoordAxis axis_name_1, const Vec3& axis_1, CoordAxis axis_name_2, const Vec3& axis_2)
  template <typename TYPE>
  void CartesianCS<TYPE>::alignTo(Axis axis_name_1, const Vec3<TYPE>& axis_1, Axis axis_name_2, const Vec3<TYPE>& axis_2) {
    // Validate Axis Names
    if (axis_name_1 == axis_name_2)
      throw std::invalid_argument("Axes vectors must be different.");

    // Validate Orthogonality
    if (abs(axis_1.dot(axis_2)) > static_cast<TYPE>(ACCURACY_FACTOR) * std::numeric_limits<TYPE>::epsilon()) {
      std::stringstream str;
      str << "Axes vectors must be orthogonal." << std::endl << axis_1 << std::endl 
          << " dot " << std::endl << axis_2 << std::endl << " = " << axis_1.dot(axis_2);
      throw std::invalid_argument(str.str());
    }

    // Calculate axis_3 and curate axis_2
    Vec3<TYPE> axis_3;
    Vec3<TYPE> axis_2_cur;
    if ((axis_name_2 - axis_name_1) == 1 || (axis_name_2 - axis_name_1) == -2) {
      axis_3 = axis_1.cross(axis_2);
      axis_2_cur = axis_3.cross(axis_1);
    }
    else {
      axis_3 = axis_2.cross(axis_1);;
      axis_2_cur = axis_1.cross(axis_3);
    }

    // Set CS axes
    _axes[axis_name_1] = axis_1;
    _axes[axis_name_2] = axis_2_cur;

    if( (axis_name_1 + axis_name_2) == 1 )
      _axes[Axis::Z] = axis_3;
    else if ((axis_name_1 + axis_name_2) == 2 )
      _axes[Axis::Y] = axis_3;
    else
      _axes[Axis::X] = axis_3;
    
    // Calculate and store the transformation matrix
    SET_TRANSF_MATRIX;

    _eulerUpdated = false;
  }
       
  #undef ACCURACY_FACTOR
  #undef SET_TRANSF_MATRIX
   
} // namespace geometry


#endif // CARTESIAN_CS_H


#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include <Eigen/Core>

#include <geometry/basic_types.h>
#include <geometry/point.h>
#include <geometry/unit_vector.h>
#include <geometry/cartesian_cs.h>


namespace geometry {

  /** 
    * \brief  ABSTRACT CLASS Shape. A collection of points (vertices) and normals to the vertices which define a 3-dimensional shape
    *         Each shape has its own Cartesian Coordinate System, which initialize matches the default CS
    *         Template params:
    *                 TYPE --> type used for the coordinates
    */
  template <typename TYPE = float>
  class Shape {
    static_assert(std::is_floating_point<TYPE>::value, "Type must be floating point type");

  public:
    /**
      * \brief  Default destructor
      */
    virtual ~Shape() {}

   
    /**
      * \brief  Set the number of vertices which are used to define the shape and recalculate all the vertices and normals
      * @param  [in] num_vertices: the new number of vertices
      * @throw  range_error exception if the number of vertices is too small
      */
    virtual void vertices(size_t num_vertices) {
      if (this->_vertices.size() != num_vertices) {
        this->_rel_vertices.clear();
        this->_rel_normals.clear();
        this->_vertices.clear();
        this->_normals.clear();
        this->_rel_vertices.reserve(num_vertices);
        this->_rel_normals.reserve(num_vertices);

        setVerticesAndNormals(num_vertices);
        this->_vertices = this->_rel_vertices;
        this->_normals = this->_rel_normals;
        absoluteCoords();
      }
    }

    /**
      * \brief  VIRTUAL METHOD. Translates the entire shape by the specified vector
      * @param[in] trans: translation vector
      */
    virtual void translate(const Vec3<TYPE>& trans) {
      // Update CS
      _cs.translate(trans);

      // Translate all vertices. Normals do not change
      auto it = _vertices.begin();
      while (it != _vertices.end()) {
        (*it++) += trans;
      }
    }

    /**
      * \brief VIRTUAL METHOD. Scales up or down the entire shape by the specified factor
      * @param[in] trans: scale factor
      */
    
    virtual void scale(TYPE factor) {
      specificScale(factor);

      // Udpate relative position of the vertices
      for (size_t i = 0; i < _rel_vertices.size(); ++i)
        _rel_vertices[i] = factor * _rel_vertices[i].vec();

      // Udpate absolute position of the vertices
      auto rot_mat = _cs.transfMatrix().inverse().rotation();
      for (size_t i = 0; i < _rel_vertices.size(); ++i)
        _vertices[i] = _cs.center() + rot_mat * _rel_vertices[i].vec();
    }

    /**
      * \brief  VIRTUAL METHOD. Rotates the entire shape, i.e. all vertices and normals, by the specified angle around the specified axis, which intersects the shapes's center
      * @param  [in] angle: rotation angle in radians
      * @param  [in] axis: vector defining the rotation axis
      */
    virtual void rotate(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
      // Rotate the internal CS and recalculate the absolute coordinates of vertices and normals
      _cs.rotateExt(axis, angle);
      absoluteCoords();
    }

    /**
      * \brief  VIRTUAL METHOD. Rotates the entire shape, i.e. all vertices and normals, by the specified angle around the specified axis, which intersects the center of the CS
      * @param  [in] angle: rotation angle in radians
      * @param  [in] axis: vector defining the rotation axis
      */
    virtual void revolve(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
      // Rotate the internal CS and recalculate the absolute coordinates of vertices and normals
      _cs.revolveExt(axis, angle);
      absoluteCoords();
    }

    /**
      * \brief  VIRTUAL METHOD. Rotates the entire shape, i.e. all vertices and normals, by the specified angle around the specified axis, which intersects the center of the CS
      *         It rotates the shape around its own center by the same angle
      * @param  [in] angle: rotation angle in radians
      * @param  [in] axis: vector defining the rotation axis
      */
    virtual void revolveSync(const Vec3<TYPE>& axis, Angle<TYPE> angle) {
      // Rotate the internal CS and recalculate the absolute coordinates of vertices and normals
      _cs.revolveExt(axis, angle);
      _cs.rotateExt(axis, angle);
      absoluteCoords();
    }

    /**
      * \brief  Get number of vertices (and normals) used to defined the shape
      * @return Number of vertices 
      */
    size_t size() const { return this->_vertices.size(); }

    /**
      * \brief  Get the shape's center
      * @return A point
      */
    const Point3<TYPE>& center() const { return this->_cs.center(); }

    /**
      * \brief  Get vertices used to defined the shape
      * @return A reference to the vertices vector
      */
    const std::vector<Point3<TYPE>>& vertices() const { return this->_vertices; }

    /**
      * \brief  Get vertices as a consecutive array of coordinates 
      * @return A pointer to the first coordinate of the first vertex
      */
    const TYPE* verticesData() const { return this->_vertices.data()->data(); }

    /**
      * \brief  Get normals used to defined the shape
      * @return A reference to the normals vector
      */
    const std::vector<UnitVec3<TYPE>>& normals() const { return this->_normals; }
    
    /**
      * \brief  Get normals as a consecutive array of vector coordinates
      * @return A pointer to the first coordinate of the first normal vector
      */
    const TYPE* normalsData() const { return this->_normals.data()->data(); }

    /**
      * \brief  PURE VIRTUAL METHOD: Get the length of the shape. To be implemented by the child class 
      */
    virtual Length<TYPE> perimeter() const = 0;

    /**
      * \brief  PURE VIRTUAL METHOD: Get the area of the shape. To be implemented by the child class
      */
    virtual TYPE area() const = 0;

    /**
      * \brief  PURE VIRTUAL METHOD: Get the volume of the shape. To be implemented by the child class
      */
    virtual TYPE volume() const = 0;


  protected:
    /**
      * \brief  Collection of shape defining vertices relative to the internal CS
      */
    std::vector<Point3<TYPE>> _rel_vertices;

    /**
      * \brief  Collection of normals to each vertex, relative to the internal CS.
      *         If the vertex is in a "corner" or tip, the normal is arbitrary  
      */
    std::vector<UnitVec3<TYPE>> _rel_normals;

    /**
      * \brief  Vertices in the absolute CS
      */
    std::vector<Point3<TYPE>> _vertices;

    /**
      * \brief  Normals in the absolute CS 
      */
    std::vector<UnitVec3<TYPE>> _normals;

    /**
      * \brief  Shape's own Coordinate System
      */
    CartesianCS<TYPE> _cs;

    /**
      * \brief  Inserts logging info into an output stream
      * @os [IN/OUT] os Reference to the output stream
      */
    friend std::ostream& operator<<(std::ostream& os, const Shape<TYPE>& shape) {
      os << " VERTICES: " << std::endl;
      auto it_v = shape._vertices.begin();
      while (it_v != shape._vertices.end())
        os << *(it_v++);
      os << " NORMALS: " << std::endl;
      auto it_n = shape._normals.begin();
      while (it_n != shape._normals.end())
        os << *(it_n++);

      return os;
    }
    
    /**
      * \brief  PURE VIRTUAL METHOD: the child class must implement this method to recalculate the vertices and normals for a given number of vertices
      *         Vertices and normals are the relative ones to the internal CS
      */
    virtual void setVerticesAndNormals(size_t num_vertices) = 0;

    /**
      * \brief  PURE VIRTUAL METHOD: the child class must implement this method to recalculate the specific shape parameters for a scaling operation.
      *         This method is called before updating the vertices
      */
    virtual void specificScale(TYPE factor) = 0;

    /**
      *  \breif  Calculate the aboslute values of vertices and normals
      */
    void absoluteCoords() {
      auto rot_mat = _cs.transfMatrix().inverse().rotation();
      for (size_t i = 0; i < _rel_vertices.size(); ++i)
        _vertices[i] = _cs.center() + rot_mat * _rel_vertices[i].vec();

      for (size_t i = 0; i < _rel_normals.size(); ++i)
        _normals[i] = rot_mat * _rel_normals[i].vec();
    }

  }; // class Shape

} // namespace geometry


#endif // SHAPE_H

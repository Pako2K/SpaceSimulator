#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <array>

#include <physics/units.h>
#include <geometry/cartesian_cs.h>
#include <physics/k_body.h>


namespace physics
{
  /**
    *  \brief  Observer class (aka as Camera). 
    *         Watch out: it must use type double, otherwise small rotations don't provide accurate results!
    *         The Observer position and orientation its defined by its own Coordinate System (OCS) relative to the space/world CS (WCS).
    *         This CS is defined by the following rules:
    *            - z axis: is always in the oposite direction of the observers sight
    *            - y axis: is always in the vertical of the observer (up)
    *            - y axis: is always pointing to the right, from the oberserver point of view 
    *         The observer default CS matches the WCS
    *         The observer position and orientation can be changed "manually" in the following ways:
    *            - smooth translation along any axis of the OCS. Translation unit can be adjusted and accelerated until a maximun velocity
    *            - smooth rotation along any axis of the OCS, centered in the OCS. Rotation unit can be adjusted and accelerated until a maximun velocity
    *            - smooth rotation along any axis of the OCS, centered in the WCS. Rotation unit can be adjusted and accelerated until a maximun velocity
    *            - re-align the axes of the OCS with the axes of the WCS, keeping the position
    *            - reposition to any axis of the WCS, keeping the distance between the WCS and the OCS and re-aligning the axes so the observer looks to the center
    *            - reposition to any point in the WCS, keeping the orientation of the OCS axes
    *  TBI       The observer position can be "locked" to follow a Body, so the Body appears static, but rotating
    *  TBI       The observer position can be "locked" to follow a Body and rotate around the body's rotation axis at the Body's rotation speed, so the Body appears static and not rotating
    *  TBI       The observer CS can be changed to match that of a SPACE SHIP Body
    *         Each observer (camera) has its own projection matrix, defined by:
    *            - Field of view
    *            - View aspect ratio
    *            - Near plane and Far plane
    */
  class Observer
  {
  public:
    using OBS_DEF_TYPE = double;

    /* *********************************************** Operations ************************************************************* */

    /**
      *  \brief Constructor 
      *         Initializes the Observer with position and orientation matching the WCS
      *  @param  id  Identifier of the observer default configuration (see observer.cfg)
      */
    Observer(size_t id);

    /**
      * Destructor of the observer
      */
    ~Observer();

    /**
      *  \brief   Assignment operation: DELETED
      */
    Observer & operator= (Observer &obs) = delete;

    /**
      *  \brief  Provides the observer's CS
      * \param
      *  \return a reference to observer's current CS
      */
    const geometry::CartesianCS<units::LENGTH_T>& obsCS() const { return _obs_CS; }

    /**
    *  \brief  Re-alignes the axes of the OCS with those of the WCS
    */
    void realign();


    /**
      *  \brief  Moves the observer to the specified position
      * @param [in] new_pos  Point where the obeserver is translated to
      */
    void reposition (const geometry::Point3<units::LENGTH_T>& new_pos);

    /**
      *  \brief  Moves the observer to the specified WCS axis, keeping the current distance to the WCS center
      *         The observer CS is realigned so the observer is looking at the center
      * @param [in] new_axis  Coordinate axis where the obeserver is translated to
      * @param [in] negative  Boolean Flag. If true, the observer is moved to the negative side of the axis
      */
    void reposition(geometry::Axis new_axis, bool negative = false);

    /**
      *  \brief  Move observer's position / orientation, i.e. the OCS
      *         The new position / orientation of the OCS depends on the added translations and rotations.
      *         Consecutively added translations and rotations build up an accelarated movement.
      */
    void move();

    /**
      *  \brief  Translate by a percentage of the distance to the followed point, along one of the OCS axis. 
      *          There are predefined maximum and minimum translation distances
      * @param [in] axis          Coordinate axis along which the obeserver is translated
      * @param [in] acceleration  Translation distance as a percentage of the distance between the observer an the followed point
      */
    void translate(geometry::Axis axis, float percentage);

    /**
      *  \brief  Rotate the observer around one of the OCS axis
      * @param [in] axis   Coordinate axis around which the obeserver is rotated
      * @param [in] angle  Rotation to be executed
      */
    void rotate(geometry::Axis axis, units::ANGLE_T angle);

    /**
      *  \brief  Rotate the observer around one of the OCS axis, centered at the WCS
      * @param [in] axis   Coordinate axis around which the obeserver is rotated
      * @param [in] angle  Rotation to be executed
      */
    void rotateWCS(geometry::Axis axis, units::ANGLE_T angle);

    /**
      *  \brief  Makes the observer to be coupled with a body, keeping the relative position.
      *          Position can still be changed, but the observer's velocity vector in the World CS will be the same as the body's one
      *  @param [in] a_body  The body to be followed
      */
    void coupleToBody(const KBody &a_body);

    /**
      *  \brief  Stops the coupling to a body
      */
    void uncoupleToBody();

    /**
      *  \brief  Retrieves the observer's angle of view (vertical).
      */
    void viewAngle(units::ANGLE_T view_angle);

    /**
      *  \brief  Retrieves the observer's angle of view (vertical).
      */
    units::ANGLE_T viewAngle() const { return _view_angle; };

    /**
      *  \brief  Retrieves a perspective transformation for the observer, according to the provided aspect ratio
      *  @parma  aspect_ratio  The viewer aspect ratio, which usually depends on the actual screen dimensions, so it cannot be predefined.
      */
    geometry::Transf<units::LENGTH_T> perspectiveTransf(OBS_DEF_TYPE aspect_ratio) const;
       
    /**
      *  \brief  Retrieves the "look at" transformation (View) for the observer
      */
    const geometry::Transf<units::LENGTH_T>& lookAtTransf() const { return _obs_CS.transfMatrix(); }


    /* *********************************************** Operations (END) ******************************************************* */

  protected:

    /**
      *  \brief  Translation axis
      */
    geometry::Axis _trans_axis;
    
    /**
      *  \brief  Observer's Coordinates System, relative to the world.
      */
    geometry::CartesianCS<units::LENGTH_T> _obs_CS;

    /**
      *  \brief  Observer's angle of view (vertical).
      */
    units::ANGLE_T _view_angle;

    /**
      *  \brief  Observer's near plane: closest distance an object can be observed.
      */
    units::LENGTH_T _near_plane;

    /**
      *  \brief  Observer's near plane: farthest distance an object can be observed.
      */
    units::LENGTH_T _far_plane;
    
    /**
      *  \brief  Perspective projection matrix from the observers point of view. This matrix must be updated when the field of view is changed.
      */
    geometry::Transf<units::LENGTH_T> _perspective_transf;

    /**
      *  \brief  Point in the space to be followed by the observer, keeping the relative position
      */
    const geometry::Point3<units::LENGTH_T>* _coupled_to;

    /**
      *  \brief  Position relative to the followed point
      */
    geometry::Vec3<units::LENGTH_T> _rel_position {0.0, 0.0, 0.0};


    /**
      *  \brief  Updates the perspective transformation for the observer.
      */
    void perspectiveTransf();


  private:

    static const geometry::Point3<units::LENGTH_T> _ORIGIN;
    
  }; // End Class Observer


} // End namespace physics



#endif // OBSERVER_H
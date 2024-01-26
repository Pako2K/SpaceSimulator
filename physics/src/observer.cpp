#include "physics/observer.h"

#include <algorithm>

#include <files/properties_file_reader.h>
#include <logger.h>
#include <maths/misc.h>

using namespace physics;
using namespace physics::units;


// Location of the config file with the observer default configuration 
static const std::string  __PROPS_FILE_NAME__{ "config/observer.cfg" };


const geometry::Point3<LENGTH_T> Observer::_ORIGIN;


/// CONSTRUCTOR()
Observer::Observer(size_t id) : _coupled_to{ &_ORIGIN } {
  // Load default properties from configuration file
  utils::PropertiesFileReader properties(__PROPS_FILE_NAME__);

  _view_angle = properties.property<OBS_DEF_TYPE>("OBS_" + std::to_string(id) + ".VIEW_ANGLE");
  _near_plane = properties.property<LENGTH_T>("OBS_" + std::to_string(id) + ".NEAR_PLANE");
  _far_plane = properties.property<LENGTH_T>("OBS_" + std::to_string(id) + ".FAR_PLANE");

  // Set perspective transformation
  perspectiveTransf();

  DebugLog("Observer: CONSTRUCTOR Called");
}

/// DESTRUCTOR()
Observer::~Observer() {
  DebugLog("Observer: DESTROYED");
}


void Observer::realign() {
  _obs_CS.alignTo(geometry::Axis::X, geometry::Vec3<LENGTH_T>::UnitX(), geometry::Axis::Y, geometry::Vec3<LENGTH_T>::UnitY());
}


void Observer::reposition(const geometry::Point3<LENGTH_T>& new_pos) {
  _rel_position = new_pos.vec() - (*_coupled_to).vec();
  _obs_CS.translate(new_pos);
}

void Observer::reposition(geometry::Axis new_axis, bool negative) {
  LENGTH_T distance = (_obs_CS.center().vec() - (*_coupled_to).vec()).norm();
  LENGTH_T sign{ 1 };
  if (negative) {
    distance = -distance;
    sign = -1;
  }

  switch (new_axis) {
  case geometry::Axis::X:
    _rel_position = geometry::Point3<LENGTH_T>(distance, 0, 0).vec();
    _obs_CS.translate((*_coupled_to) + geometry::Point3<LENGTH_T>(distance, 0, 0).vec());
    _obs_CS.alignTo(geometry::Axis::Z, sign * geometry::Vec3<LENGTH_T>::UnitX(), geometry::Axis::Y, geometry::Vec3<LENGTH_T>::UnitZ());
    break;
  case geometry::Axis::Y:
    _rel_position = geometry::Point3<LENGTH_T>(0, distance, 0).vec();
    _obs_CS.translate((*_coupled_to) + geometry::Point3<LENGTH_T>(0, distance, 0).vec());
    _obs_CS.alignTo(geometry::Axis::Z, sign * geometry::Vec3<LENGTH_T>::UnitY(), geometry::Axis::Y, geometry::Vec3<LENGTH_T>::UnitZ());
    break;
  case geometry::Axis::Z:
    _rel_position = geometry::Point3<LENGTH_T>(0, 0, distance).vec();
    _obs_CS.translate((*_coupled_to) + geometry::Point3<LENGTH_T>(0, 0, distance).vec());
    _obs_CS.alignTo(geometry::Axis::Z, sign * geometry::Vec3<LENGTH_T>::UnitZ(), geometry::Axis::Y, geometry::Vec3<LENGTH_T>::UnitY());
    break;
  }
}


void Observer::translate(geometry::Axis axis, float percentage) {
  static const  LENGTH_T MIN = 1e6;
  static const  LENGTH_T MAX = 1e13;
  LENGTH_T dist = percentage * (_obs_CS.center().vec() - _coupled_to->vec()).norm();
  // The increment/decrement in distance must be bewtween the MAX and MIN, in absolute value
  dist = maths::signedMax(dist, MIN);
  dist = maths::signedMin(dist, MAX);
  _rel_position += dist * _obs_CS[uint8_t(axis)];
}


void Observer::rotate(geometry::Axis axis, ANGLE_T angle) {
  // Execute Rotation
  _obs_CS.rotateInt(axis, angle);
}


void Observer::rotateWCS(geometry::Axis axis, ANGLE_T angle) {
  // Rotate around the _coupled_to point!
  if (_coupled_to)
    _obs_CS.revolveExt(*_coupled_to, geometry::CartesianCS<LENGTH_T>::defCartAxis[axis].vec(), angle);
  else
    _obs_CS.revolveExt(geometry::CartesianCS<LENGTH_T>::defCartAxis[axis].vec(), angle);

  // Store new relative position
  _rel_position = _obs_CS.center().vec() - _coupled_to->vec();

  // Execute Rotation to keep looking at the same point
  _obs_CS.rotateExt(geometry::CartesianCS<LENGTH_T>::defCartAxis[axis].vec(), angle);
}


void Observer::move() {
  // TRANSLATION //
  // Reposition relative to the coupled body or space point
  _obs_CS.translate(*_coupled_to + _rel_position);
}


geometry::Transf<LENGTH_T> Observer::perspectiveTransf(OBS_DEF_TYPE aspect_ratio) const {
  geometry::Transf<LENGTH_T> transf = _perspective_transf;
  transf(0, 0) = _perspective_transf(0, 0) / aspect_ratio;
  return transf;
}


void Observer::viewAngle(ANGLE_T view_angle) {
  _view_angle = view_angle;
  perspectiveTransf();
};


void Observer::perspectiveTransf() {
  LENGTH_T tanHalfFovy = tan(_view_angle / static_cast<LENGTH_T>(2));
  const OBS_DEF_TYPE ASPECT_RATIO = 1.0;
  _perspective_transf.matrix() << static_cast<LENGTH_T>(1) / (ASPECT_RATIO * tanHalfFovy), 0, 0, 0,
                                  0, static_cast<LENGTH_T>(1) / (tanHalfFovy), 0, 0,
                                  0, 0, -(_far_plane + _near_plane) / (_far_plane - _near_plane), -(static_cast<LENGTH_T>(2) * _far_plane * _near_plane) / (_far_plane - _near_plane),
                                  0, 0, -static_cast<LENGTH_T>(1), 0;
}


void Observer::coupleToBody(const KBody& a_body){
  _coupled_to = &a_body.position();

  // Observer position is now relative to the body
  _rel_position = _obs_CS.center().vec() - (*_coupled_to).vec();
}


void Observer::uncoupleToBody(){
  _coupled_to = &_ORIGIN;

  // Observer position is now relative to World CS
  _rel_position = _obs_CS.center().vec();
}

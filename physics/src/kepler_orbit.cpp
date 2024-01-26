#include <physics/kepler_orbit.h>

#include <logger.h>

using namespace physics;
using namespace physics::units;
using namespace geometry;

constexpr auto PRECISION_ANOMALY_CALC = 0.1;


/*   KeplerOrbit(const PBody& prim_body, const PBody& sec_body)   */
/******************************************************************/
KeplerOrbit::KeplerOrbit(const PBody& prim_body, const PBody& sec_body) {

  // 0. Check that both bodies are really gravitationally bound and not in collision
  Vec3<LENGTH_T> v_rel_position = sec_body.position().vec() - prim_body.position().vec();
  LENGTH_T l_rel_position = v_rel_position.norm();
    
  //      Collision check!
  if (l_rel_position <= prim_body.radius + sec_body.radius)
    throw ExcBodyCollision("COLLISION between " + prim_body.name() + " and " + sec_body.name());
  
  Vec3<SPEED_T> v_rel_velocity = sec_body.velocity() - prim_body.velocity();
  SPEED_T l_rel_velocity_squared = v_rel_velocity.squaredNorm();
  ENERGY_T kin_energy = 0.5 * sec_body.mass()() * l_rel_velocity_squared;
  ENERGY_T pot_energy = GRAV_CONST * prim_body.mass() * sec_body.mass()() / l_rel_position;

              /*
              if (primary.hasParent()){
                // Acceleration caused by the parent body:
                ACCELERATION_T acc_parent = GRAV_CONST * primary.mass() / l_rel_position / l_rel_position;
                // Acceleration caused by the grandparent body:
                geometry::Vec3<LENGTH_T> v_rel_position_GP = body.position().vec() - primary.parent()->position().vec();
                LENGTH_T l_rel_position_GP = v_rel_position_GP.norm();
                geometry::Vec3<LENGTH_T> v_rel_position_GP2P = primary.position().vec() - primary.parent()->position().vec();
                LENGTH_T l_rel_position_GP2P = v_rel_position_GP2P.norm();

                ACCELERATION_T acc_grandparent = GRAV_CONST * primary.parent()->mass()() * (1 / l_rel_position_GP / l_rel_position_GP - 1  / l_rel_position_GP2P / l_rel_position_GP2P);

                  if (acc_grandparent > acc_parent)
                    return orbit(primary.parent());
                }
                else*/ 
  //      Binding check!
  if (kin_energy >= pot_energy)
    throw ExcBodyNotBound("Body " + sec_body.name() + " is not gravitationally bound to " + prim_body.name());


  // 1. Calculate inclination of the orbital plane
  //    H = (R x V) is orthogonal to the orbital plane.
  //    cos i = H*(0,0,1) / length(H)
  geometry::Vec3<ANG_MOMEMTUM_MASSLESS_T> v_H = (v_rel_position.cross(v_rel_velocity));
  ANG_MOMEMTUM_MASSLESS_T l_H = v_H.norm();

  // Check whether the orbit is a straight line!
  if (l_H <= std::numeric_limits<ANG_MOMEMTUM_MASSLESS_T>::epsilon()) {
    //    sin i = v_rel_pos*(0,0,1) / l_rel_pos
    _i = acos(v_rel_position.z() / l_rel_position);

    //    cos asc_node = (v_rel_pos x (0,0,1))*(1,0,0) / length((v_rel_pos x (0,0,1))
    auto v_asc_node = v_rel_position.cross(geometry::Vec3<LENGTH_T>::UnitZ());
    _asc_node = acos(v_asc_node.x() / v_asc_node.norm());
    // There are 2 possible solutions asc_node or 2*PI-asc node.
    // TO determine which one is the correct one, the y component of asc_node vector (uz x H) must be checked:
    if (v_asc_node.y() < 0)
      _asc_node = TWO_PI - _asc_node;

    //    e = 1 (but semimajor axis is not infinite, as in a parabolic or hyperbolic curve)
    _e = 1;
     
    //    a = r*mu / (2*mu - r * v * v)
    REDUCED_MASS_T g_mass = sec_body.reduced_mass + prim_body.reduced_mass;
    _a = l_rel_position * g_mass / (2 * g_mass - l_rel_position * l_rel_velocity_squared);

    //    anomaly = PI (and it is always PI, in this case)
    _anomaly = PI;
    
    //    periapsis = 3 * PI / 2
    _periapsis = 3 * HALF_PI;

    return;
  }

  _i = acos(v_H.z() / l_H);
  // For a retrograde orbit (cos < 0) i = PI - i
  _i = std::min(_i, static_cast<ANGLE_T>(PI - _i));


  // 2. Calculate longitude of ascending node
  //    cos asc_node = ((0,0,1) x H )*(1,0,0) / length((0,0,1) x H)
  // When i=0 then asc_node = 0 (arbitrary value)
  if (_i > std::numeric_limits<ANGLE_T>::epsilon()) {
    auto v_asc_node = geometry::Vec3<ANG_MOMEMTUM_MASSLESS_T>::UnitZ().cross(v_H);
    _asc_node = acos(v_asc_node.x() / v_asc_node.norm());
    // There are 2 possible solutions asc_node or 2*PI-asc node.
    // TO determine which one is the correct one, the y component of asc_node vector (uz x H) must be checked:
    if (v_asc_node.y() < 0)
      _asc_node = TWO_PI - _asc_node;
  }


  // 3. Calculate eccentricity
  //    e = sqrt(H/mu*(H*v*v/mu-2*vt)+1)
  //    mu = G * (m1+m2)
  auto u_rel_position = v_rel_position.normalized();
  auto rad_velocity = u_rel_position.dot(v_rel_velocity);
  auto tan_velocity = sqrt(l_rel_velocity_squared - rad_velocity * rad_velocity);

  _mu = sec_body.reduced_mass + prim_body.reduced_mass;
  _e = sqrt(l_H / _mu * (l_H*l_rel_velocity_squared / _mu - 2 * tan_velocity) + 1);


  // 4. Calculate semimajor axis (e cannot be 1: already checked above: see 0. and 1.)
  //    a = p /(1-e*e), p = H*H / mu
  _a = l_H * l_H / _mu / (1 - _e * _e);


  // 5. Calculate true anomaly (Z-Zo) and periapsis (Zo)
  //    sin (Z-Zo) = vr / e * H / g_mass)
  //     If e > 0 
  if (_e > std::numeric_limits<LENGTH_T>::epsilon()) {
    // Watch out: it is possible that the argument of asin is slightly higher than 1!!
    if (rad_velocity >= 0.0)
      _anomaly = asin(std::min(1.0, rad_velocity / _e * l_H / _mu));
    else
      _anomaly = asin(std::max(-1.0, rad_velocity / _e * l_H / _mu));
    //     The solution to this equation can have 2 values: anomaly, PI - anomaly
    //     In order to determine the right one, it must be taken into account that:
    //          vt = mu / H*(1+e*cos (Z-Zo))
    //          if vt < mu/H then cos (Z-Zo) < 0 and anomaly € (PI/2, 3PI/2)
    if (tan_velocity < _mu / l_H)
      _anomaly = PI - _anomaly;
    if (_anomaly < 0.0)
      _anomaly += TWO_PI;

    // Calculate argument of periapsis
    //    cos (periapsis+anomaly) = r.normalized * u_asc_node (normalized vector in the direction of the asc. node)
    geometry::Vec3<ANGLE_T> u_asc_node{ cos(_asc_node), sin(_asc_node), 0.0 };
    ANGLE_T added_angle = acos(u_rel_position.dot(u_asc_node));
    // There are 2 possible solutions: added_angle  or 2*PI-added_angle.
    // TO determine which one is the correct one, it is possible to use the projection of r over a perpendicular
    //      vector to asc_node in the xy plane
    if (u_rel_position.dot(geometry::Vec3<ANGLE_T>(-u_asc_node.y(), u_asc_node.x(), 0)) < 0)
      added_angle = TWO_PI - added_angle;
    _periapsis = added_angle - _anomaly;
    if (_periapsis < 0.0)
      _periapsis += TWO_PI;
  }
  //     If e = 0 then orbit is a perfect circle and Zo is arbitrary (set to 0)
  else {
    //    cos Z = u_rel_pos * u_asc_node
    geometry::Vec3<ANGLE_T> u_asc_node{ cos(_asc_node), sin(_asc_node), 0.0 };
    _anomaly = acos(u_rel_position.dot(u_asc_node));

    //    The solution to this equation can have 2 values: anomaly, PI - anomaly
    //    Comparing the y component of the u_asc_node and the u_rel_position determines the right value: if u_rel_pos.y + u_asc_node.y < 0 then anomaly is between PI and 2*PI 
    if (u_rel_position.y() + u_asc_node.y() < 0)
      _anomaly = TWO_PI - _anomaly;
  }

  // Set the additional orbit parameters
  extParams();

  // Set the caratesian coordinates relative to the primary
  cartesianParams();

  return;
}

/*    std::pair<PBody::PositionType, PBody::VelocityType> forward(units::TIME_T delta_time)   */
/**********************************************************************************************/
std::pair<Vec3<LENGTH_T>, Vec3<SPEED_T>> KeplerOrbit::forward(TIME_T delta_time) {
  // Calculate anomaly in time+delta_time
  //    Store previous values of ecc_anomaly and time from periapsis (as a double!)
  ANGLE_T old_ecc_anomaly = _ecc_anomaly;
  ANGLE_T old_mean_anomaly = _mean_anomaly;
  double fact = sqrt(_a * _a * _a / _mu);
  double old_time_periapsis = fact * (old_ecc_anomaly - _e * sin(old_ecc_anomaly));

  //    a- New mean anomaly, M1 = M + 2*PI / T * delta_time
  ANGLE_T delta_M = TWO_PI / period().count() * delta_time.count();
  _mean_anomaly += delta_M;

  //    b- New ecc_anomaly (APPROXIMATION!!!), E1 = E + 2*PI/T/(1-e*cos(E))*delta_time
  _ecc_anomaly = _ecc_anomaly + delta_M / (1 - _e * cos(_ecc_anomaly));

  //    c- Check that the approximation is good enough
  //       (Compare times from new E and actual time from periapsis + delta_time)
  double new_time_periapsis = fact * (_ecc_anomaly - _e * sin(_ecc_anomaly));

  // Store the old state
  PBody::PositionType old_position = _position;
  PBody::VelocityType old_velocity = _velocity;

  if ( std::abs(new_time_periapsis - old_time_periapsis - delta_time.count()) > PRECISION_ANOMALY_CALC) {
    DebugLog("time_periapsis: " << old_time_periapsis);
    DebugLog("New time_periapsis: " << new_time_periapsis);
    ErrorLog("ANOMALY CALCULATION ERROR: delta_time and delta_periapsis: " << delta_time.count() << " - " << (new_time_periapsis - old_time_periapsis));
    if (delta_time.count() == 1)
      throw std::runtime_error("ANOMALY CALCULATION ERROR");
    // Rollback
    _mean_anomaly = old_mean_anomaly;
    _ecc_anomaly = old_ecc_anomaly;
    DebugLog("Calculating in 2 steps: " + std::to_string(delta_time.count() / 2 + delta_time.count() % 2) + ", " + std::to_string(delta_time.count() / 2));

    forward(delta_time / 2 + delta_time % 2);
    forward(delta_time / 2);
    
    DebugLog("Finished calculating in 2 steps.");
  }
  else {
    //    d- Calculate new true anomaly, tan (anomaly/2) = sqrt( (1+e)/(1-e) )*tan(E/2)
    _anomaly = 2 * atan(sqrt((1 + _e) / (1 - _e)) * tan(_ecc_anomaly / 2));
    if (_anomaly < 0)
      _anomaly = TWO_PI + _anomaly;

    _time_periapsis = TIME_T(TIME_T::rep(new_time_periapsis));

    // Set the new caratesian coordinates relative to the parent body
    cartesianParams();

  }

  // Return the change in the state
  return std::pair<Vec3<LENGTH_T>, Vec3<SPEED_T>>(_position.vec() - old_position.vec(), _velocity - old_velocity);
}


/*   void KeplerOrbit::extParams()   */
/*************************************/
void KeplerOrbit::extParams() {
  ANGLE_T cos_anomaly = std::cos(_anomaly);
  ANGLE_T e_cos_anomaly = _e * cos_anomaly;

  // Eccentric anomaly
  //    cos E = (e+cos(anomaly)) / (1+e*cos(anomaly))
  //    sin E = (sqr(1-e*e)*sin(anomaly)) / (1+e*cos(anomaly))
  // This provides an angle between 0 and PI
  _ecc_anomaly = acos((_e + cos_anomaly) / (1 + e_cos_anomaly));

  // In order to determine whether the angle is between PI and 2*PI, the sin is used
  if ((sqrt(1 - _e * _e) * std::sin(_anomaly) / (1 + e_cos_anomaly)) < 0)
    _ecc_anomaly = TWO_PI - _ecc_anomaly;

  // Mean anomaly
  //  M = E - e*sin E
  _mean_anomaly = _ecc_anomaly - _e * sin(_ecc_anomaly);

  // Time after last periapsis
  _time_periapsis = TIME_T(TIME_T::rep(sqrt(_a * _a * _a / _mu) * (_ecc_anomaly - _e * sin(_ecc_anomaly))));
}


/*    void KeplerOrbit::cartesianParams()   */
/********************************************/
void KeplerOrbit::cartesianParams() {
  // CALCULATE POSITION AND VELOCITY

  // BEGIN: Calculate position in the orbital plane:
  //      x axis pointing to the periapsis
  //      y in the direction of the movement

  //    Distance to the center 
  ///1 - e * e could be reused....
  LENGTH_T one_minus_e_e = 1 - _e * _e;
  LENGTH_T p = _a * one_minus_e_e;
  LENGTH_T cos_anomaly = cos(_anomaly);
  LENGTH_T sin_anomaly = sin(_anomaly);
  LENGTH_T one_plus_e_cos_anomaly = 1 + _e * cos_anomaly;
  LENGTH_T radius = p / one_plus_e_cos_anomaly;
    
  _position.x() = radius * cos_anomaly;
  _position.y() = radius * sin_anomaly;
  _position.z() = 0;


  // ROTATION Matrices, used to determine position and velocity
  //      1. Around the z axis by asc_node
  //      2. Around the asc_node axis by i
  //          (orb_u_z is the result of rotating u_z around the asc_node axis by i)
  //      3. Around the orb_u_z axis by periapsis
      //           IMPROVEMENTS!!!!! 
    //                - reuse rotation matrix
  Mat3<LENGTH_T> _rot_z_asc_node; // Rotation matrix around the z axis by asc_node
  Mat3<LENGTH_T> _rot_u_asc_node_i; // Rotation matrix around the asc_node axis by i
  Mat3<LENGTH_T> _rot_orb_z_periapsis; // Rotation matrix around the z axis of the orbit plane by periapsis
  _rot_z_asc_node = Eigen::AngleAxis<LENGTH_T>(_asc_node, Vec3<LENGTH_T>::UnitZ()).toRotationMatrix();
  Vec3<LENGTH_T> u_asc_node{ cos(_asc_node), sin(_asc_node), 0.0 };
  _rot_u_asc_node_i = Eigen::AngleAxis<LENGTH_T>(_i, u_asc_node).toRotationMatrix();
  Vec3<LENGTH_T> orb_u_z = _rot_u_asc_node_i * Vec3<LENGTH_T>::UnitZ();
  _rot_orb_z_periapsis = Eigen::AngleAxis<LENGTH_T>(_periapsis, orb_u_z).toRotationMatrix();


  // Rotate the coordinates 3 times:
  //      1. Around the z axis by asc_node
  //      2. Around the asc_node axis by i
  //      3. Around the orb_u_z axis by periapsis
  _position = _rot_orb_z_periapsis * (_rot_u_asc_node_i * (_rot_z_asc_node * _position.vec()));


  // BEGIN: Calculate velocity in the orbital plane:
  //      x axis pointing to the periapsis
  //      y in the direction of the movement

  //    Radial velocity (in the direction of the radius vector)
  SPEED_T rad_v = sqrt(_mu / p) * _e * sin_anomaly;
  //    Tangential velocity (tangential to the radius vector)
  SPEED_T tan_v = sqrt(_mu / p) * one_plus_e_cos_anomaly;

  _velocity.x() = rad_v * cos_anomaly - tan_v * sin_anomaly;
  _velocity.y() = rad_v * sin_anomaly + tan_v * cos_anomaly;
  _velocity.z() = 0;

  // END: Calculate velocity in the orbital plane

  // Rotate the coordinates 3 times:
  //      1. Around the z axis by asc_node
  //      2. Around the asc_node axis by i
  //          (orb_u_z is the result of rotating u_z around the asc_node axis by i)
  //      3. Around the orb_u_z axis by periapsis
  _velocity = _rot_orb_z_periapsis * (_rot_u_asc_node_i * (_rot_z_asc_node * _velocity));

}


Ellipse<LENGTH_T> KeplerOrbit::orbitVertices(size_t num_vertices) const {
  // Delete previous ellipse (if any) and create a new one 
  Ellipse<LENGTH_T> ellipse(_a, Eccentricity<LENGTH_T>(_e));
  ellipse.vertices(num_vertices);
  // Translate focus to the origin of the CS
  ellipse.translate(-ellipse.focus().vec());

  // Execute the 3 rotations
  //           IMPROVEMENTS!!!!! 
  //                - reuse rotation matrices
  //                -create rotation method for the Shape3D which takes directly the rotation matrix
  ellipse.rotate(Vec3<LENGTH_T>::UnitZ(), _asc_node);
  Vec3<LENGTH_T> u_asc_node{ cos(_asc_node), sin(_asc_node), 0.0 };
  ellipse.rotate(u_asc_node, _i);
  Mat3<LENGTH_T> _rot_u_asc_node_i = Eigen::AngleAxis<LENGTH_T>(_i, u_asc_node).toRotationMatrix();
  Vec3<LENGTH_T> orb_u_z = _rot_u_asc_node_i * Vec3<LENGTH_T>::UnitZ();
  ellipse.rotate(orb_u_z, _periapsis);

  return ellipse;
}






            //bool Body::move(TimeType delta_time) {

            //  bool parent_changed{ false };

            //  // If object has a parent, calculate movement based on the orbital parameters
            //  // Orbital parameters should be recalculated after the movement to take into account
            //  //      the interaction (acceleration) with other bodies
            //  if (_parent_body) {

            //    // 1. Calculate anomaly in time+delta_time
            //    //    a- New mean anomaly, M1 = M + 2*PI / T * delta_time
            //    VAR_TYPE delta_M = _2PI_ / _orbit->period(_mass, _parent_body->mass()) * delta_time;
            //    //    VAR_TYPE new_mean_anomaly = _orbit->mean_anomaly() + delta_M;
            //        //    b- New ecc_anomaly (APPROXIMATION!!!), E1 = E + 2*PI/T/(1-e*cos(E))*delta_time
            //    VAR_TYPE new_ecc_anomaly = _orbit->ecc_anomaly() + delta_M / (1 - _orbit->e() * cos(_orbit->ecc_anomaly()));
            //    //    c- Check that the approximation is good enough
            //    // TBD: compare times from new E and actual time from periapsis + delta_time
            //    //    d- Calculate new true anomaly, tan (anomaly/2) = sqrt( (1+e)/(1-e) )*tan(E/2)
            //    VAR_TYPE new_true_anomaly = 2 * atan(sqrt((1 + _orbit->e()) / (1 - _orbit->e())) * tan(new_ecc_anomaly / 2));
            //    if (new_true_anomaly < 0)
            //      new_true_anomaly = _2PI_ + new_true_anomaly;

            //    // Update anomalies in the orbital parameters
            //    _orbit->anomaly(new_true_anomaly);

            //    // Convert to Cartesian coordinates: position and velocity
            //    pair<VEC3, VEC3> cartesianVectors = _orbit->toCartesian(_mass, _parent_body->mass());

            //    VEC3 orb_position = cartesianVectors.first;
            //    VEC3 orb_velocity = cartesianVectors.second;

            //    _position = _parent_body->position() + orb_position;
            //    _velocity = _parent_body->velocity() + orb_velocity;


            //    // Orbital parameters should be recalculated after the movement to take into account the interaction (acceleration) with other bodies
            //    if (_acceleration.norm() > 0) {
            //      _position += 0.5*_acceleration*delta_time*delta_time;
            //      _velocity += _acceleration * delta_time;

            //      VEC3 rel_position = _position - _parent_body->position();
            //      VAR_TYPE rel_position_norm = rel_position.norm();
            //      VAR_TYPE kepler_acceleration_norm = GC * _parent_body->mass() / rel_position_norm / rel_position_norm;

            //      // Recalculate orbits
            //      try {
            //        const Body *parent_candidate;
            //        if (kepler_acceleration_norm < _max_perturbation) {
            //          parent_candidate = _max_perturbation_body;
            //          logger.trace(TraceLevel::info, "Parent Candidate: " + parent_candidate->name());
            //          parent_changed = true;
            //        }
            //        else
            //          parent_candidate = _parent_body;
            //        while (parent_candidate) {
            //          try {
            //            orbit(*parent_candidate);
            //            _former_parent_body = _parent_body;
            //            _parent_body = parent_candidate;
            //            parent_changed = (_former_parent_body != _parent_body); // watch out: after throwing an exception it could be that the new parent is still the same!
            //            parent_candidate = nullptr; // to leave the loop
            //          }
            //          catch (string & exc) {
            //            logger.trace(TraceLevel::error, "Parent Candidate FAILED: " + parent_candidate->name() + ". " + exc);
            //            if (exc == "OPEN CURVE ORBIT" && parent_candidate->hasParent()) {
            //              parent_candidate = &parent_candidate->parent();
            //              logger.trace(TraceLevel::info, "New Parent Candidate: " + parent_candidate->name());
            //            }
            //            else
            //              throw exc;
            //          }
            //        }
            //      }
            //      catch (string &exc) {
            //        logger.trace(TraceLevel::error, "While recalculating orbit for " + _name);
            //        throw exc;
            //      }
            //    }

            //    // Reset acceleration
            //    _acceleration *= 0.0;
            //    _max_perturbation = 0;
            //    _max_perturbation_body = nullptr;
            //  }
            //  else {
            //    // APPROXIMATION !!!
            //    // For the Sun no change due to interaction
            //    _position += _velocity * delta_time; // + 0.5*_acceleration*delta_time*delta_time;
            //                                       //_velocity += _acceleration*delta_time;
            //  }

            //  return parent_changed;
            //  /*
            //    _position += _velocity*delta_time + 0.5*_acceleration*delta_time*delta_time;
            //    _velocity += _acceleration*delta_time;
            //    _acceleration *= 0.0;
            //  */

            //  // Orbital parameters should be recalculated after the movement to take into account
            //  //      the interaction (acceleration) with other bodies
            //  /** TO BE DONE!*/
            //        // Recalculate orbits
            ////      try{
            ////        (*(it1++))->orbit(true);
            ////      }
            ////      catch(string &exc){
            ////        logger.trace(TraceLevel::error, exc);
            ////        logger.trace(TraceLevel::error, "While calculating orbit for " + (*(--it1))->name() + ". Time: " + to_string(_time));
            ////        throw exc;
            ////      }


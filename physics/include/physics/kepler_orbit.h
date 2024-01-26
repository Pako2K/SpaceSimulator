#ifndef KEPLER_ORBIT_H
#define KEPLER_ORBIT_H

#include <stdexcept>
#include <string>

#include <physics/p_body.h>
#include <physics/units.h>
#include <geometry/constants.h>
#include <geometry/shapes2D/ellipse.h>


namespace physics
{
  /**
    *  \brief  Class representing an orbit with Keplerian elements, including the "current" position (true anomaly).
    *          All the parameters are given in the non-inertial CS centered at the primary body
    *          The parameters defining a Kepler Orbit are
    *               - Orbit inclination: i
    *               - Longitude of ascending node: asc_node
    *               - Argument of periapsis: periapsis
    *               - Orbit Semimajor axis: a
    *               - Orbit eccentricity: e
    *               - True anomaly: anomaly
    *
    *          The orbit is centered at the "periapsis" focus, occupied by the parent body
    *          Additionally the following orbital parameters are provided:
    *               - Eccentric anomaly
    *               - Mean anomaly
    *               - Orbital period
    *               - Time after last periapsis
    *
    *          As well as the position and velocity relative to the parent body
    *
    *          It is also possible to get the representation of the full orbit as a set of points (contained in a vector)
    *
    *          (Angles are measured in radians, lengths are measured in meters and times in seconds)
    */
  class KeplerOrbit
  {
  public:
    /**
      *  \brief  Customized exception for a Body Collision between the primary and the secondary
      */
    class ExcBodyCollision : public std::runtime_error
    {
    public:
      ExcBodyCollision(std::string txt) : runtime_error(txt) {}
    };

    /**
      *  \brief  Customized exception not bound primary and secondary bodies
      */
    class ExcBodyNotBound : public std::runtime_error
    {

    public:
      ExcBodyNotBound(std::string txt) : runtime_error(txt) {}
    };

    /**
      *  \brief  Default Constructor (to be used for bodies with non keplerian orbits, like the Sun)
      *          It keeps the default value for all elements, and sets _mu to 1 to avoid 0/0 divisions when invoking the method to get the period
      */
    KeplerOrbit() : _mu{ 1.0 } {}

    /**
      *  \brief  Constructor: Orbit is calculated for the secondary body around the primary body.
      *                       It will be checked that the mass of the secondary is smaller than that of the primary
      *                       Positions and velocities of the 2 bodies must use the same CS
      *
      *  @param  prim_body      The main (primary) body
      *  @param  sec_body       The secondary body, for which the orbit is calculated.
      *
      *  @throw  BodyNotBound      If body and its parents are not gravitationally bound
      *  @throw  BodyCollision     If body and its parents are colliding
      */
    KeplerOrbit(const PBody& prim_body, const PBody& sec_body);

    /**
      *  \brief  Copy Constructor: DELETED
      */
    KeplerOrbit(const KeplerOrbit&) = delete;

    /**
      *  \brief  Move Constructor: DELETED
      */
    KeplerOrbit(KeplerOrbit&&) = delete;

    /**
      *  \brief  Assignment operator: DELETED
      */
    KeplerOrbit& operator=(const KeplerOrbit&) = delete;

    /**
      *  \brief  Move Assignment operator: DELETED
      */
    KeplerOrbit& operator=(KeplerOrbit&&) = delete;

    /**
      *  \brief  Recalculates the position of the secondary and the barycenter relative to the primary after the elapsed time
      *
      *  @param  delta_time The elapsed time
      *
      *  @return  State change: delta of the position and velocity relative to the primary body
      *
      *  @throw  runtime_error In case the anomaly cannot be calculated with enough precision
      */
    std::pair<geometry::Vec3<units::LENGTH_T>, geometry::Vec3<units::SPEED_T>> forward(units::TIME_T delta_time);

    /**
      *  \brief  Getters for the Keplerian elements
      */
#define GET(x) auto x() const {return this->_##x;}
#define GET2(x,y) auto x() const {return this->_##y;}
    GET(i)
    GET(a)
    GET(e)
    GET2(ascNode, asc_node)
    GET(periapsis)
    GET(anomaly)
    GET(mu)
    GET2(eccAnomaly, ecc_anomaly)
    GET2(meanAnomaly, mean_anomaly)
    GET2(timePeriapsis, time_periapsis)
    GET(position)
    GET(velocity)
#undef GET
#undef GET2

    /**
      *  \brief  Get orbital period
      */
    units::TIME_T period() const { return units::TIME_T(units::TIME_T::rep(TWO_PI * sqrt(_a * _a * _a / _mu))); }
    
    /**
      *  \brief  Set orbit vertices in the provided ellipse
      */
    //const std::vector<geometry::Point3<units::LENGTH_T>>& orbitVertices(size_t num_vertices);
    geometry::Ellipse<units::LENGTH_T> orbitVertices(size_t num_vertices) const;
    

  protected:

    /**
      *  \brief  Element of the keplerian orbit: SEMIMAJOR AXIS
      */
    units::LENGTH_T        _a{ 0.0 };

    /**
      *  \brief  Element of the keplerian orbit: ECCENTRICITY (is actually dimensionless, but is defined with LENGHT_T)
      */
    units::LENGTH_T        _e{ 0.0 };

    /**
      *  \brief  Element of the keplerian orbit: INCLINATION
      */
    units::ANGLE_T         _i{ 0.0 };

    /**
      *  \brief  Element of the keplerian orbit: ASCENDING NODE LONGITUDE (Angle between the X axis and the asc. node)
      */
    units::ANGLE_T         _asc_node{ 0.0 };

    /**
      *  \brief  Element of the keplerian orbit: PERIAPSIS ARGUMENT (Angle betwwen the asc. node and the periapsis)
      */
    units::ANGLE_T         _periapsis{ 0.0 };

    /**
      *  \brief  Element of the keplerian orbit: TRUE ANOMALY (Angle between the periapsis and the actual position of the body)
      */
    units::ANGLE_T         _anomaly{ 0.0 };

    /**
      *  \brief  Conevenient parameter of the keplerian orbit: ECCENTRIC ANOMALY
      */
    units::ANGLE_T         _ecc_anomaly{ 0.0 };

    /**
      *  \brief  Conevenient parameter of the keplerian orbit: MEAN ANOMALY
      */
    units::ANGLE_T         _mean_anomaly{ 0.0 };

    /**
      *  \brief  Conevenient parameter of the keplerian orbit: Time elapsed since last periapsis
      */
    units::TIME_T          _time_periapsis{ 0 };

    /**
      *  \brief  Reused constant parameter: reduced mass of the primary and secondary bodies (Gm1 + Gm2)
      */
    units::REDUCED_MASS_T  _mu{ 0.0 };

    /**
      *  Current position (for the current true anomaly) in the non-intertial reference CS centered at the primary body
      */
    PBody::PositionType    _position{ 0.0, 0.0, 0.0 };

    /**
      *  Current velocity (for the current true anomaly) in the non-intertial reference CS relative to the primary body
      */
    PBody::VelocityType    _velocity{ 0.0, 0.0, 0.0 };


  private:
    /**
      *  \brief  Recalculates the other anomalies and time after periapsis
      */
    void extParams();

    /**
      *  \brief  Transforms keplerian orbit elements to the cartesian elements relative to the primary CS
      */
    void cartesianParams();

  }; // END class KeplerOrbit
}


#endif // KEPLER_ORBIT_H








      //#include <Geometry/Shapes2D/ellipse.h>
      //#include <physics/mass.h>
      //#include <physics/constants.h>

      //#include <unordered_map>
      //#include <string>


      //class KeplerOrbit : protected geo::Ellipse<T>
      //{
      //public:

      //  /**
      //    *  Returns the 6 Keplerian elements plus the 2 additional anomalies as a Map.
      //    */
      //  inline std::unordered_map<std::string, T> elements() const;


      //    /**
      //      *  Returns a vector of <num_points> positions equally distributed (in eccentric anomaly) along the orbit.
      //      *  Each position is represented as a 3 dimensional geometric point
      //      */
      //    inline const std::vector<geo::Point<T, 3>>& orbitPositions(size_t num_points);

      //  /**
      //    *  Get current position, in cartesian coordinates
      //    */
      //  inline const geo::Point<T, 3>& position() const;

      //  /**
      //    *  Get current velocity, in cartesian coordinates
      //    */
      //  inline const Eigen::Matrix<T, 3, 1>& velocity() const;

      //  /**
      //    *  Returns the orbital period in seconds.
      //    */
      //  inline T period() const;

      //  /**
      //    *  Returns time elapsed since the last periapsis, in seconds.
      //    */
      //  inline T timeAfterPeriapsis() const;

      //  /**
      //    *  For the current orbit, updates the true anomaly. The other anomalies will be recalculated as well
      //    */
      //  inline void anomaly(T new_anomaly);

      //  /**
      //    *  For the current orbit, updates the true anomaly and the other anomalies according to the elapsed time
      //    *  Throws an exception if the increased time is too big to apply the differential calculation (1 part in 1 million)
      //    */
      //  inline void addTime(T delta_time) throw(std::string);

      //protected:
      //  /**
      //    * Elements of the keplerian orbit (a and e are inherited from "geo::Ellipse")
      //    */
      //  T _i;
      //  T _asc_node;
      //  T _periapsis;
      //  T _anomaly;
      //  T _ecc_anomaly;
      //  T _mean_anomaly;

      //  T _mu_P;
      //  T _mu_S;

      //  /**
      //    *  Current position (for the current true anomaly)
      //    */
      //  geo::Point<T, 3> _position;

      //  /**
      //    *  Current velocity (for the current true anomaly)
      //    */
      //  Eigen::Matrix<T, 3, 1> _velocity;

      //  /**
      //    *  Variables for reused calculations
      //    */
      //  T _cos_anomaly;
      //  T _sin_anomaly;
      //  T _e_cos_anomaly; // e * cos(anomaly)
      //  T _p; // a * (1 - e * e)
      //  T _a_cube; // a * a * a
      //  T _mu; // mu_P + mu_S

      //  /**
      //    *  Rotation matrices for reused calculations
      //    */
      //  Eigen::Matrix<T, 3, 3> _rot_z_asc_node; // Rotation matrix around the z axis by asc_node
      //  Eigen::Matrix<T, 3, 3> _rot_u_asc_node_i; // Rotation matrix around the asc_node axis by i
      //  Eigen::Matrix<T, 3, 3> _rot_orb_z_periapsis; // Rotation matrix around the z axis of the orbit plane by periapsis


      //  /**
      //    *  Sets the true anomaly and calculates the other 2 anomalies.
      //    */
      //  void set_anomalies(T new_anomaly);

      //};


      //template <typename T>
      //KeplerOrbit<T>::KeplerOrbit(const Mass<T> &m1, const Mass<T> &m2, T i, T asc_node, T periapsis, T a, T e, T anomaly) throw (std::range_error)
      //  : KeplerOrbit{ _GC_ * m1 , _GC_ * m2, i, asc_node, periapsis, a, e, anomaly } {}

      //// The generated ellipse must be "centered" at the focus
      //template <typename T>
      //KeplerOrbit<T>::KeplerOrbit(T mu1, T mu2, T i, T asc_node, T periapsis, T a, T e, T anomaly) throw (std::range_error)
      //  : geo::Ellipse<T>{ a, typename geo::Ellipse<T>::Eccentricity(e), /*Centered at the focus*/ geo::Point<T, 3>(-e * a ,0) }, _i{ i }, _asc_node{ asc_node }, _periapsis{ periapsis },
      //  _mu_P{ mu1 }, _mu_S{ mu2 }, _position{ 0.0, 0.0, 0.0 }, _velocity{ 0.0, 0.0, 0.0 } {

      //  // VALIDATIONS
      //  if (_mu_P <= 0 || _mu_S <= 0)
      //    throw std::range_error("Invalid mass: value must be positive");
      //  if (_i < 0 || _i >= _PI_)
      //    throw std::range_error("Invalid inclination: value must be in the range [0, PI)");
      //  if (_asc_node < 0 || _asc_node >= TWO_PI)
      //    throw std::range_error("Invalid ascending node: value must be in the range [0, 2*PI)");
      //  if (_periapsis < 0 || _periapsis >= TWO_PI)
      //    throw std::range_error("Invalid periapsis: value must be in the range [0, 2*PI)");
      //  if (_anomaly < 0 || _anomaly >= TWO_PI)
      //    throw std::range_error("Invalid anomaly: value must be in the range [0, 2*PI)");

      //  // Variable for calculations to be reused
      //  _p = this->_a * (1 - this->_e * this->_e);
      //  _a_cube = this->_a * this->_a * this->_a;
      //  _mu = _mu_P + _mu_S;

      //  // ROTATION Matrices, used to determine position and velocity
      //  //      1. Around the z axis by asc_node
      //  //      2. Around the asc_node axis by i
      //  //          (orb_u_z is the result of rotating u_z around the asc_node axis by i)
      //  //      3. Around the orb_u_z axis by periapsis
      //  _rot_z_asc_node = Eigen::AngleAxis<T>(_asc_node, Eigen::Matrix<T, 3, 1>::UnitZ()).toRotationMatrix();
      //  Eigen::Matrix<T, 3, 1> u_asc_node{ std::cos(_asc_node), std::sin(_asc_node), 0.0 };
      //  _rot_u_asc_node_i = Eigen::AngleAxis<T>(_i, u_asc_node).toRotationMatrix();
      //  Eigen::Matrix<T, 3, 1> orb_u_z = _rot_u_asc_node_i * Eigen::Matrix<T, 3, 1>::UnitZ();
      //  _rot_orb_z_periapsis = Eigen::AngleAxis<T>(_periapsis, orb_u_z).toRotationMatrix();

      //  // Apply rotation to the Ellipse
      //  this->rotate3D(_asc_node, Eigen::Matrix<T, 3, 1>::UnitZ());
      //  this->rotate3D(_i, u_asc_node);
      //  this->rotate3D(_periapsis, orb_u_z);

      //  set_anomalies(anomaly);
      //}

      //template <typename T>
      //std::unordered_map<std::string, T> KeplerOrbit<T>::elements() const {
      //#define MAP_PAIR(s) {#s,this->_##s}
      //  return std::unordered_map<std::string, T> { MAP_PAIR(i), MAP_PAIR(asc_node), MAP_PAIR(periapsis), MAP_PAIR(a), MAP_PAIR(e), MAP_PAIR(anomaly),
      //    MAP_PAIR(ecc_anomaly), MAP_PAIR(mean_anomaly) };
      //#undef MAP_PAIR
      //}

      //template <typename T>
      //const std::vector< geo::Point<T, 3> >& KeplerOrbit<T>::orbitPositions(size_t num_points) {
      //  return this->vertices(num_points);
      //}

      //template <typename T>
      //const geo::Point<T, 3>& KeplerOrbit<T>::position() const {
      //  return _position;
      //}

      //template <typename T>
      //const Eigen::Matrix<T, 3, 1>& KeplerOrbit<T>::velocity() const {
      //  return _velocity;
      //}

 



      //template <typename T>
      //void KeplerOrbit<T>::anomaly(T new_anomaly) {
      //  if (new_anomaly < 0 || new_anomaly >= TWO_PI)
      //    new_anomaly = new_anomaly - TWO_PI * int(new_anomaly / TWO_PI);

      //  set_anomalies(new_anomaly);
      //}

      //template <typename T>
      //void KeplerOrbit<T>::addTime(T delta_time) throw(std::string) {
      //  // 1. Calculate anomaly in time+delta_time
      //  //    a- New mean anomaly, M1 = M + 2*PI / T * delta_time
      //  T delta_M = TWO_PI / this->period() * delta_time;
      //  //    b- New ecc_anomaly (APPROXIMATION!!!), E1 = E + 2*PI/T/(1-e*cos(E))*delta_time
      //  T new_ecc_anomaly = _ecc_anomaly + delta_M / (1 - this->_e * cos(_ecc_anomaly));
      //  //    c- Check that the approximation is good enough
      //  T current_time_after_periapsis = this->timeAfterPeriapsis();
      //  T new_time_after_periapsis = sqrt(_a_cube / _mu) * (new_ecc_anomaly - this->_e * sin(new_ecc_anomaly));

      //  if (abs((current_time_after_periapsis + delta_time) - new_time_after_periapsis) > this->period() / static_cast<T>(1e6))
      //    throw std::string("DELTA TIME TOO BIG: APPROXIMATION FAILS!");
      //  //    d- Calculate new true anomaly, tan (anomaly/2) = sqrt( (1+e)/(1-e) )*tan(E/2)
      //  T new_true_anomaly = 2 * atan(sqrt((1 + this->_e) / (1 - this->_e)) * tan(new_ecc_anomaly / 2));
      //  if (new_true_anomaly < 0)
      //    new_true_anomaly = TWO_PI + new_true_anomaly;

      //  // Update anomalies in the orbital parameters
      //  this->anomaly(new_true_anomaly);
      //}

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      //template <typename T>
      //void KeplerOrbit<T>::set_anomalies(T new_anomaly) {
      //  _anomaly = new_anomaly;

      //  _cos_anomaly = std::cos(_anomaly);
      //  _sin_anomaly = std::sin(_anomaly);
      //  _e_cos_anomaly = this->_e * _cos_anomaly;

      //  // Eccentric anomaly
      //  //    cos E = (e+cos(anomaly)) / (1+e*cos(anomaly))
      //  //    sin E = (sqr(1-e*e)*sin(anomaly)) / (1+e*cos(anomaly))
      //  // This provides an angle between 0 and PI
      //  T E = acos((this->_e + _cos_anomaly) / (1 + _e_cos_anomaly));

      //  // In order to determine whether the angle is between PI and 2*PI, the sin is used
      //  if ((sqrt(1 - this->_e * this->_e) * _sin_anomaly / (1 + _e_cos_anomaly)) < 0)
      //    E = TWO_PI - E;

      //  _ecc_anomaly = E;

      //  // Mean anomaly
      //  //  M = E - e*sin E
      //  _mean_anomaly = E - this->_e * sin(E);

      //  /// RE-CALCULATE POSITION AND VELOCITY

      //  // BEGIN: Calculate position in the orbital plane:
      //  //      x axis pointing to the periapsis
      //  //      y in the direction of the movement

      //  //    Distance to the center
      //  T radius = _p / (1 + _e_cos_anomaly);

      //  _position.x() = radius * _cos_anomaly;
      //  _position.y() = radius * _sin_anomaly;

      //  // Rotate the coordinates 3 times:
      //  //      1. Around the z axis by asc_node
      //  //      2. Around the asc_node axis by i
      //  //          (orb_u_z is the result of rotating u_z around the asc_node axis by i)
      //  //      3. Around the orb_u_z axis by periapsis
      //  _position = _rot_orb_z_periapsis * (_rot_u_asc_node_i * (_rot_z_asc_node * _position));


      //  // BEGIN: Calculate velocity in the orbital plane:
      ////      x axis pointing to the periapsis
      ////      y in the direction of the movement

      ////    Radial velocity (in the direction of the radius vector)
      //  T rad_v = sqrt(_mu / _p) * this->_e * _sin_anomaly;
      //  //    Tangential velocity (tangential to the radius vector)
      //  T tan_v = sqrt(_mu / _p) * (1 + _e_cos_anomaly);

      //  _velocity.x() = rad_v * _cos_anomaly - tan_v * _sin_anomaly;
      //  _velocity.y() = rad_v * _sin_anomaly + tan_v * _cos_anomaly;

      //  // END: Calculate velocity in the orbital plane

      //  // Rotate the coordinates 3 times:
      //  //      1. Around the z axis by asc_node
      //  //      2. Around the asc_node axis by i
      //  //          (orb_u_z is the result of rotating u_z around the asc_node axis by i)
      //  //      3. Around the orb_u_z axis by periapsis
      //  _velocity = _rot_orb_z_periapsis * (_rot_u_asc_node_i * (_rot_z_asc_node * _velocity));
      //}



//#endif // KEPLER_ORBIT_H

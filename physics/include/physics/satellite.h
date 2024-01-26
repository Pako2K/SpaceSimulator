#ifndef SATELLITE_H
#define SATELLITE_H

#include <physics/p_body.h>
#include <physics/planet.h>
#include <physics//minor_body.h>


namespace physics
{
  template <class BODY>
  class Satellite : public PBody
  {
    static_assert(BODY.hash_code() == Planet.hash_code() || BODY.hash_code() == MinorBody.hash_code(), "Specified Body type cannot have satellites");

  public:
    const BODY&         parent;

    const units::LENGTH_T eq_radius{ 0 };

    const units::LENGTH_T polar_radius{ 0 };

    Satellite(DECL_BODY_CONSTRUCTOR_PARAMS, const BODY& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius);

    Satellite(DECL_BODY_CONSTRUCTOR_PARAMS, const BODY& parent)
      : Satellite{ name, mass, radius, position, velocity, parent, radius, radius } {}
    
    Satellite(DECL_BODY_CONSTRUCTOR_RM_PARAMS, const BODY& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius)
      : Satellite{ name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, parent, eq_radius, polar_radius } {}

    Satellite(DECL_BODY_CONSTRUCTOR_RM_PARAMS, const BODY& parent)
      : Satellite{ id, name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, parent, radius, radius } {}
    

    ~Satellite() {};
  };

}


#endif //SATELLITE_H

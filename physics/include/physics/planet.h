#ifndef PLANET_H
#define PLANET_H

#include <physics/k_body.h>
#include <physics/star.h>


namespace physics
{
  class Planet : public KBody
  {
  public:
//    const Star&            parent;

    const units::LENGTH_T  eq_radius{ 0 };

    const units::LENGTH_T  polar_radius{ 0 };

    const bool is_dwarf;

    Planet(DECL_BODY_CONSTRUCTOR_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, bool dwarf);

    Planet(DECL_BODY_CONSTRUCTOR_RM_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, bool dwarf)
      : Planet{ id, name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, parent, eq_radius, polar_radius, dwarf } {}
      
    ~Planet() {};
  };

}


#endif //PLANET_H

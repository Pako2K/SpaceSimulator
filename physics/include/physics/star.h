#ifndef STAR_H
#define STAR_H

#include <array>

#include <physics/k_body.h>


namespace physics
{

  class Star : public KBody
  {
  public:
    const units::LENGTH_T         eq_radius{ 0 };

    const units::LENGTH_T         polar_radius{ 0 };

    const std::array<uint8_t, 3>  color{ 0, 0, 0 }; // For instance 240,240,190

    Star(DECL_BODY_CONSTRUCTOR_PARAMS, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color);

    Star(DECL_BODY_CONSTRUCTOR_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color);

    Star(DECL_BODY_CONSTRUCTOR_RM_PARAMS, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color)
      : Star(id, name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, eq_radius, polar_radius, color) {}

    Star(DECL_BODY_CONSTRUCTOR_RM_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color)
      : Star(id, name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, parent, eq_radius, polar_radius, color) {}

    ~Star() {};
  };
  
}


#endif //STAR_H

#include <physics/star.h>

using namespace physics;

Star::Star(DECL_BODY_CONSTRUCTOR_PARAMS, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color)
  : KBody{ id, name, mass, radius, position, velocity }, eq_radius{ eq_radius }, polar_radius{ polar_radius }, color{ color } {
  
  // Check that the radius is positive
  if (eq_radius <= static_cast<units::LENGTH_T>(0) || polar_radius <= static_cast<units::LENGTH_T>(0))
    throw std::invalid_argument("Radius cannot be negative");

}

Star::Star(DECL_BODY_CONSTRUCTOR_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, std::array<uint8_t, 3> color)
  : KBody{ id, name, mass, radius, position, velocity, parent }, eq_radius{ eq_radius }, polar_radius{ polar_radius }, color{ color } {

  // Check that the radius is positive
  if (eq_radius <= static_cast<units::LENGTH_T>(0) || polar_radius <= static_cast<units::LENGTH_T>(0))
    throw std::invalid_argument("Radius cannot be negative");

}

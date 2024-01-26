#include <physics/planet.h>

using namespace physics;

Planet::Planet(DECL_BODY_CONSTRUCTOR_PARAMS, const Star& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius, bool is_dwarf)
  : KBody{ id, name, mass, radius, position, velocity, parent }, eq_radius{ eq_radius }, polar_radius{ polar_radius }, is_dwarf{ is_dwarf }  {
    
  // Check that the radius is positive
  if (eq_radius <= static_cast<units::LENGTH_T>(0) || polar_radius <= static_cast<units::LENGTH_T>(0))
    throw std::invalid_argument("Radius must be positive");
}

#include <physics/satellite.h>

using namespace physics;

template <class BODY>
Satellite<BODY>::Satellite(DECL_BODY_CONSTRUCTOR_PARAMS, const BODY& parent, units::LENGTH_T eq_radius, units::LENGTH_T polar_radius)
  : PBody{ id, name, mass, radius, position, velocity }, parent{ parent }, eq_radius{ eq_radius }, polar_radius{ polar_radius }  {

  // Check that the radius is positive
  if (eq_radius <= static_cast<units::LENGTH_T>(0) || polar_radius <= static_cast<units::LENGTH_T>(0))
    throw std::invalid_argument("Radius must be positive");
}

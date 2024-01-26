#include <physics/p_body.h>


using namespace physics;


PBody::PBody(DECL_BODY_CONSTRUCTOR_PARAMS)
  : utils::UniqueSortable<std::string, types::Mass<>, std::greater<types::Mass<>>>("PBody", name, mass), 
  reduced_mass{ units::GRAV_CONST * mass() }, radius{ radius }, _position{ position }, _velocity{ velocity } {
  
  validate();
}


PBody::PBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS)
  : utils::UniqueSortable<std::string, types::Mass<>, std::greater<types::Mass<>>>("PBody", name, types::Mass<>(reduced_mass / units::GRAV_CONST)),
  reduced_mass{ reduced_mass }, radius{ radius }, _position{ position }, _velocity{ velocity } {

  validate();
}

  
void PBody::validate() {
  if (matchingKey().size() == 0)
    throw std::invalid_argument("Name cannot be empty");
}

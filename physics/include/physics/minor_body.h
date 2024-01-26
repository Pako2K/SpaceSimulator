#ifndef MINOR_BODY_H
#define MINOR_BODY_H

#include <physics/p_body.h>
#include <physics/star.h>


namespace physics
{
  class MinorBody : public PBody
  {
  public:
    const Star&  parent;

    MinorBody(DECL_BODY_CONSTRUCTOR_PARAMS, const Star& parent) 
      : PBody{ id, name, mass, radius, position, velocity }, parent{ parent } {}
    
    MinorBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS, const Star& parent)
      : MinorBody{ id, name, types::Mass<>(reduced_mass / units::GRAV_CONST), radius, position, velocity, parent } {}

    ~MinorBody() {};
  };

}


#endif //MINOR_BODY_H

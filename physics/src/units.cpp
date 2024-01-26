#include <physics/units.h>

using namespace physics::units;


#define UNIT_CLASS_DEFS(__Name__, __Abbr__, __Conv__)   const std::string __Name__::name{ #__Abbr__ }; \
                                                        const double __Name__::to_Kg{ __Conv__ }

UNIT_CLASS_DEFS(Microgram, ug., 1e-9);
UNIT_CLASS_DEFS(Milligram, mg., 1e-6);
UNIT_CLASS_DEFS(Gram, g., 1e-3);
UNIT_CLASS_DEFS(Kilogram, Kg., 1.0);
UNIT_CLASS_DEFS(Ton, ton, 1e3);
UNIT_CLASS_DEFS(EarthMass, EM., E_MASS);
UNIT_CLASS_DEFS(SolarMass, SM., S_MASS);

#undef UNIT_CLASS_DEFS
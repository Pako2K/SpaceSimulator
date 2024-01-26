#ifndef UNITS_H
#define UNITS_H

#include <chrono>
#include <string>

namespace physics
{
  namespace units
  {
    // Types
    using TIME_T = std::chrono::seconds;
    using TIME_NANO_T = std::chrono::nanoseconds;
    using LENGTH_T = double; // In meters
    using ANGLE_T = double; // In radians
    using SPEED_T = double; // In m/s
    using ACCELERATION_T = double; // In m/s/s
    using REDUCED_MASS_T = double; // = G*M, in m^3/s^2
    using ENERGY_T = double; //in Kg*m^2/s^2
    using ANG_MOMEMTUM_MASSLESS_T = double; // in m^2/s 

    /**
      *  \brief  Gravitacional Constant, in m^3/Kg^1/s^2
      */
    constexpr double GRAV_CONST{ 6.674083e-11 };

    /**
      *  \brief  Astronomical Unit
      */
    constexpr LENGTH_T _AU_{ 149597870700.0 };

    /**
      *  \brief  Speed of light
      */
    constexpr SPEED_T _C_{ 299792458.0 };

    /**
      *  \brief  Distance of 1 light-year
      */
    constexpr LENGTH_T _LIGHT_YEAR_{ _C_ * 365 * 86400 };


    /**
      *   \brief  Solar Mass in Kg
      */
    constexpr double S_MASS{ 1.98854e+30 };

    /**
      *   \brief  Earth Mass in Kg
      */
    constexpr double E_MASS{ 5.97219e+24 };


    /**
      *  \brief  BASE CLASS for the units of Mass
      */
    class MassUnit { private: MassUnit() {} };

    /**
      *  \brief  MACRO: for the standard definition of Mass Units
      *  @param  __Name__  Name of the Unit Class
      *  @param  __Abbr__  Abbreviation of the unit
      *  @param  __Conv__  Conversion factor to the SI unit (Kg)
      */
#define UNIT_CLASS(__Name__)    class __Name__ : public MassUnit \
                                  { \
                                  public: \
                                    static const std::string name; \
                                    static const double to_Kg; \
                                  }; \

    UNIT_CLASS(Microgram);
    UNIT_CLASS(Milligram);
    UNIT_CLASS(Gram);
    UNIT_CLASS(Kilogram);
    UNIT_CLASS(Ton);
    UNIT_CLASS(EarthMass);
    UNIT_CLASS(SolarMass);

#undef CLASS_UNIT 
 
  }
}

#endif // UNITS_H
#ifndef PHYSICS_TYPES_H
#define PHYSICS_TYPES_H

#include <type_traits>
#include <ostream>

#include <physics/units.h>

namespace physics
{
  namespace types
  {
    /**
      *  \brief Mass
      *         Template class with 2 parameters:
      *           - Mass Unit
      *           - Mass value type
      *         It ensures consistent values and operations with masses
      */
    template <typename MASS_UNIT = units::Kilogram, typename TYPE = double>
    class Mass
    {
      static_assert(std::is_floating_point<TYPE>::value, "Mass must be floating point type");
      static_assert(std::is_base_of<units::MassUnit, MASS_UNIT>::value, "Mass Unit type invalid");

    public:
      /**
        *  \brief CONSTRUCTOR: mass
        *  @param  mass
        *  @throw  range_error exception if mass is not positive
        */
      Mass(TYPE mass) {
        if (mass >= 0)
          _mass = mass;
        else
          throw std::range_error("Mass must be positive");
      }

      /**
        *  \brief Assignment operator
        */
      Mass<MASS_UNIT, TYPE>& operator= (TYPE new_mass) {
        if (new_mass >= 0)
          _mass = new_mass;
        else
          throw std::range_error("Bad Assignment: Mass must be positive");
        return *this;
      }

      /**
        *  \brief  Returns the value of the mass
        */
      TYPE operator() () const { return _mass; }

      /**
        *  \brief  Returns the units of the mass
        */
      const std::string& unit() const { return MASS_UNIT::name; }


      /**
        *  \brief  Returns the mass in the specified unit
        */
      template <typename NEW_UNIT>
      Mass<NEW_UNIT, TYPE> cast() const { return Mass<NEW_UNIT, TYPE>(_mass * MASS_UNIT::to_Kg / NEW_UNIT::to_Kg); }


      /**
       *  \brief  Friend insertion operator. Prints in the specified output stream the Mass value and the unit
       *  @param   os  a reference to an output stream object
       *  @param   a_mass  a reference to a mass
       *  @return  the reference to the output stream
       */
      friend std::ostream& operator<<(std::ostream& os, const Mass<MASS_UNIT, TYPE>& a_mass) {
        os << a_mass._mass << " " << MASS_UNIT::name;
        return os;
      }


      /**
        *  \brief Friend addition operator
        *  @return The new mass resulting of adding the 2 masses
        */
      friend Mass<MASS_UNIT, TYPE> operator+(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        return Mass<MASS_UNIT, TYPE>(m1._mass + m2._mass);
      }

      /**
        *  \brief Friend addition operator
        *  @return The mass in the left after adding the 2nd mass
        */
      friend Mass<MASS_UNIT, TYPE>& operator+=(Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        m1._mass += m2._mass;
        return m1;
      }

      /**
        *  \brief Friend substraction operator
        *  @return The new mass resulting of substracting the 2 masses
        */
      friend Mass<MASS_UNIT, TYPE> operator-(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        return Mass<MASS_UNIT, TYPE>(m1._mass - m2._mass);
      }

      /**
        *  \brief Friend substraction operator
        *  @return The mass in the left after substracting the 2nd mass
        */
      friend Mass<MASS_UNIT, TYPE>& operator-=(Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        m1._mass -= m2._mass;
        return m1;
      }

      /**
        *  \brief Friend multiplication operator
        *  @return A new mass multiplied by the factor
        */
      friend Mass<MASS_UNIT, TYPE> operator*(const TYPE factor, const Mass<MASS_UNIT, TYPE>& m1) {
        return Mass<MASS_UNIT, TYPE>(factor * m1._mass);
      }

      /**
        *  \brief Friend multiplication operator
        *  @return A new mass multiplied by the factor
        */
      friend Mass<MASS_UNIT, TYPE> operator*(const Mass<MASS_UNIT, TYPE>& m1, const TYPE factor) {
        return factor * m1;
      }

      /**
        *  \brief  Friend multiplication operator
        *  @return The result of multiplying the values of the 2 masses
        */
      friend TYPE operator*(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        return m1._mass * m2._mass;
      }

      /**
        *  \brief  Friend division operator
        *  @return The result of dividing the values of the 2 masses
        */
      friend TYPE operator/(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT, TYPE>& m2) {
        return m1._mass / m2._mass;
      }

      /**
        *  \brief  Friend comparison operator
        *  @return True if the 2 masses are equal
        */
      template <typename MASS_UNIT_2>
      friend bool operator==(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass == m2.cast<MASS_UNIT>()._mass;
      }

      /**
        *  \brief  Friend comparison operator
        *  @return True if the 2 masses are NOT equal
        */
      template <typename MASS_UNIT_2>
      friend bool operator!=(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass != m2.cast<MASS_UNIT>()._mass;
      }

      /**
        *  \brief  Friend greater than comparison operator
        *  @return True if the first mass is bigger
        */
      template <typename MASS_UNIT_2>
      friend bool operator>(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass > m2.cast<MASS_UNIT>()._mass;
      }

      /**
        *  \brief  Friend less than comparison operator
        *  @return True if the first mass is smaller
        */
      template <typename MASS_UNIT_2>
      friend bool operator<(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass < m2.cast<MASS_UNIT>()._mass;
      }

      /**
        *  \brief  Friend greater than or equal comparison operator
        *  @return True if the first mass is equal to or bigger
        */
      template <typename MASS_UNIT_2>
      friend bool operator>=(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass >= m2.cast<MASS_UNIT>()._mass;
      }

      /**
        *  \brief  Friend less than comparison operator
        *  @return True if the first mass is equal to or smaller
        */
      template <typename MASS_UNIT_2>
      friend bool operator<=(const Mass<MASS_UNIT, TYPE>& m1, const Mass<MASS_UNIT_2, TYPE>& m2) {
        return m1._mass <= m2.cast<MASS_UNIT>()._mass;
      }

    protected:
      /**
        *  \brief  Value of the mass in the specified units
        */
      TYPE _mass;
    };

#define LITERAL_OP(Name, Abbr)    inline Mass<units::Name, double> operator"" _##Abbr(const long double value) { \
                                    return Mass<units::Name, double>(value); \
                                  }; \
                                  inline Mass<units::Name, double> operator"" _##Abbr(const unsigned long long value) { \
                                    return Mass<units::Name, double>(double(value)); \
                                  };

    LITERAL_OP(Microgram, ug);
    LITERAL_OP(Milligram, mg);
    LITERAL_OP(Gram, g);
    LITERAL_OP(Kilogram, Kg);
    LITERAL_OP(Ton, ton);
    LITERAL_OP(EarthMass, EM);
    LITERAL_OP(SolarMass, SM);

#undef LITERAL_OP

  }
}

#endif // PHYSICS_TYPES_H
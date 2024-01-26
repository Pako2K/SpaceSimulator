#ifndef P_BODY_H
#define P_BODY_H

#include <string>
#include <functional>

#include <collections/unique_sortable.h>

#include <physics/types.h>
#include <geometry/point.h>


namespace physics
{

  /**
    *  \brief  Abstract spherical based body, derived from UniqueSortable.
    *          It is abstract because the constructors are protected, so they can be called only by a derived class.
    *          A spherical body is defined by a name (matching key), which must be unique. The name of a body is built depending on its type.
    *          Basic properies are the mass (sorting key), radius, position and velocity.
    *          Sorting will be done by decreasing mass.
    */
  class PBody : public utils::UniqueSortable<std::string, types::Mass<>, std::greater<types::Mass<>> >
  {

  public:
    /**
      *  Type defs
      */
    using PositionType = geometry::Point3<units::LENGTH_T>;
    using VelocityType = geometry::Vec3<units::SPEED_T>;

    /**
      * \brief  Body's Reduced Mass
      */
    const units::REDUCED_MASS_T  reduced_mass;

    /**
      * \brief  Body's average radius
      */
    const geometry::Length<units::LENGTH_T>  radius{ 0 };
    

    PBody(const PBody&) = delete;

    PBody(PBody&&) = default;

    ~PBody() {}

    PBody& operator=(const PBody&) = delete;

    PBody& operator=(PBody&&) = delete;


    /**
      *  \brief  Returns the name
      */
    const std::string&     name() const { return matchingKey(); }

    /**
      *  \brief  Returns the mass
      */
    const types::Mass<>&   mass() const { return sortingKey(); }
      
    /**
      *  \brief  Returns the position
      */
    const PositionType&    position() const { return _position; }

    /**
      *  \brief  Returns the velocity
      */
    const VelocityType&    velocity() const { return _velocity; }


    /**
      *  \brief  Output stream insertion operator
      *  @param  os  A reference to an output stream
      *  @param  An instance of a PBody object
      *  @return  The output stream received a parameter
      */
    friend std::ostream& operator<< (std::ostream& os, const PBody& obj) {
      os << "Body = " << obj.name() << " { " << obj.mass()<< " } ";
      return os;
    }


  protected:
#define DECL_BODY_CONSTRUCTOR_PARAMS const std::string&     name, \
                                     const types::Mass<>&   mass, \
                                     const units::LENGTH_T  radius, \
                                     const PositionType&    position, \
                                     const VelocityType&    velocity

#define DECL_BODY_CONSTRUCTOR_RM_PARAMS const std::string&           name, \
                                        const units::REDUCED_MASS_T  reduced_mass, \
                                        const units::LENGTH_T        radius, \
                                        const PositionType&          position, \
                                        const VelocityType&          velocity

    /**
      *  \brief  Default constructor. Uses the mass to calculate the reduced mass
      */
    PBody(DECL_BODY_CONSTRUCTOR_PARAMS);

    /**
      *  \brief  Default constructor. Uses the reduced mass to calculate the mass
      */
    PBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS);        

    /**
      *  \brief  Body's position in Cartesian coordinates, relative to a reference CS. Measured in meters.
      */
    PositionType         _position{ 0.0, 0.0, 0.0 };

    /**
      * \brief  Body's velocity in Cartesian coordinates, relative to a reference CS. Measured in meters/second.
      */
    VelocityType         _velocity{ 0.0, 0.0, 0.0 };


  private:

    void validate();

  };

} // END Namespace Physics


#endif // P_BODY_H

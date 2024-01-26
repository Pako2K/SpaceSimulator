#ifndef K_BODY_H
#define K_BODY_H

#include <physics/p_body.h>
#include <physics/kepler_orbit.h>

#include <collections/mtree.h>


namespace physics
{
  /**
    *  \brief  Keplerian body, derived from abstract class PBody.
    *          Each body can be classified in a specific type: STAR, PLANET, etc (see enum BodyType)
    *          The matching key, which must be unique, is the name of the body built depending on its type:
                  - STARS: Name
                  - PLANETS: Name
                  - DWARF PLANETS AND MINOR BODIES: id + " " + name | provisional name
                  - SATELLITES: name + " [<Parent body name> <id-in-Roman-Numeral>]"
    *          The keplerian body has a Kepler orbit unless it is not gravitationally bound to any other body.
    *          All the bodies can be stored in a hierarchical tree, represented by an MTree.
    *          Keplerian bodies can interact and be perturbated by gravitational interactions. The way these interactions are calculated depends on the configuration
    *             defined in the properties file body.cfg. These properties are read upon initialization of the class: see initialize(). Bodies can't be created before
    *             class initialization.
    *          Once all the bodies are created, using a common CS, the barycenter of the system as well as the different local barycenters should be calculated. Only bodies fulfilling
    *             some conditions will be used to calculate the barycenter and the perturbations on their parent bodies: see barycenters()
    *          The barycenter of the system will be used as new inertial CS for all the bodies. No more bodies can be created after this change is performed. 
    *             The position of all barycenters will be stored in each parent body, using the new inertial CS. 
    *          Only after these steps are executed the gravitational interaction of the system should be called: see gravInteraction(). This method will calculate the changes in positions 
    *             and velocities, as well as perturbations caused by all the bodies in all the other bodies, according to certain rules configured in body.cfg.
    *          Finally the movement of each body will be executed: see keplerMove()
    */
  class KBody : public PBody
  {
  public:
    enum BodyType : uint8_t {STAR, PLANET, DWARF_PLANET, MINOR_BODY, SATELLITE};

    inline static const std::array<std::string, 5> TYPE_NAME{ "STAR", "PLANET", "DWARF_PLANET", "MINOR_BODY", "SATELLITE" };

    const BodyType    TYPE;

    /**
      *  \brief  Body Id. It is a unique Id for a family of bodies (e.g. all satellites of a planet; all the minor bodies; planets)
      */
    const int64_t    ID;

    /**
      *  \brief  Body common name (without numbers or provisional designations). It can be empty
      */
    const std::string COMMON_NAME;

    /**
      *  \brief  Body provisional name (for minor bodies and satellites). It can be empty
      */
    const std::string PROVISIONAL_NAME;


    static void initialize();

    /**
      *  \brief  Determine Barycenter displacement for all the parent bodies with their perturbator children bodies.
      */
    static void barycenters(tree::MTree<KBody>& bodies);


    /**
      *  \brief  Determine Barycenter displacement for a parent body with their perturbator children bodies.
      *          This information is stored in the bodies and will be used during the calulation of the perturbations
      */
    static void barycenter(tree::MTree<KBody>& bodies, KBody& parent_body);


    static void gravInteraction(tree::MTree<KBody>& bodies, const units::TIME_T& delta_t);


    KBody(DECL_BODY_CONSTRUCTOR_PARAMS, KBody& parent, BodyType type, int64_t id, std::string provisional_name = "");

    KBody(DECL_BODY_CONSTRUCTOR_PARAMS); // Constructor for the main star
    
    KBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS, KBody& parent, BodyType type, int64_t id, std::string provisional_name = "");

    KBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS); // Constructor for the main star

    KBody(KBody&&) = default;

    ~KBody() {};

    bool hasParent() const { return _parent; }

    const KBody& parent() const { return *_parent; }

    const KeplerOrbit& orbit() const { return *_orbit; }
       
    const bool parentPerturbator() const { return _parent_perturbator; }

    /**
      *  \brief  Get the barycenter position with the perturbating bodies (children)
      */
    const PositionType& barycenterPos() const { return _barycenter_pos; }

    /**
      *  \brief  Get the barycenter velocity with the perturbating bodies (children)
      */
    const VelocityType& barycenterVel() const { return _barycenter_vel; }


  protected:

    static bool _initialized;
    
    static double _barycenter_ratio_limit;

    static bool _barycenters_set;
    
    KBody*  _parent{ nullptr };

    std::unique_ptr<KeplerOrbit>  _orbit{ new KeplerOrbit()};


    /**
      *  \brief  Flag to indicate whether this body perturbates its parent
      *          Set in the constructor and not changed afterwards
      */
    bool _parent_perturbator{ false };


    /**
      *  \brief  Position of the barycenter with the perturbator bodies (children)
      *          It should be updated before the first move and after every move
      */
    PositionType _barycenter_pos;
    
  
    /**
      *  \brief  Velocity of the barycenter with the perturbator bodies (children)
      *          It should be updated before the first move and after every move
      */
    VelocityType _barycenter_vel;


    /**
      *  \brief  Calculates the new keplerian orbit position after interacting with the parent body for delta_t seconds
      *          DO NOT USE WITH THE ROOT BODY
      */
    void keplerMove(const units::TIME_T& delta_t);


  private:
    static std::string uniqueName(BodyType type, int64_t id, const std::string& name, const std::string& provisional_name, const std::string& parent_name);

    static bool validateTypes(BodyType parent_type, BodyType child_type);

    void commonConstructor();

  };

}


#endif // K_BODY_H
//////////
//////////
//////////  public:
//////////    /**
//////////      *  \brief  Comparison Functor. It uses the distances of the bodies to their parents. Bodies must have the same parent. Can be used for sorting in the Bodies Tree
//////////      *  @param   a_body  Body to compare to
//////////      *  @return  True if this body's distance is less than the distance of the compared Body
//////////      */
//////////    //class SortByDistance
//////////    //{
//////////    //public:
//////////    //  bool operator()(const Body& lbody, const Body& rbody) const {
//////////    //    if (lbody.parent() != rbody.parent())
//////////    //      throw std::invalid_argument("Bodies must have the same parent");
//////////    //    return (lbody.position().vec() - lbody.parent()->position().vec()).norm() < (rbody.position().vec() - rbody.parent()->position().vec()).norm();
//////////    //  }
//////////    //};
//////////     
//////////
//////////
//////////  protected:
//////////  
//////////    /**
//////////      * \brief  Generic body constructor for a body
//////////      *         Protected constructor so bodies cannot be instantiated, except by the derived classes
//////////      *         It validates the mass and the name, which must be unique (not case sensitive validation).
//////////////      * @param  id              The id of the body
//////////      * @param  name            The name of the body
//////////      * @param  mass            The mass of the body (In Kilograms)
//////////      * @param  position        The initial position of the body (in meters)
//////////      * @param  velocity        The initial velocity of the body (in m/s)
//////////      * @param  rotation_period The rotation period 
//////////      * @param  rotation_tilt   The rotation axis tilt
//////////      * @param  rotation_alfa   The rotation axis angle relative to the X axis of the orbit
//////////      * @param  rotation        The initial rotation angle, relative to the X axis of the body's own CS
//////////////      * @param  parent          The body which this body is gravitationally bound to
//////////      * @throw  invalid_argument  If the name of the body has already been used or rotation period is negative
//////////      */
//////////#define DECL_BODY_CONSTRUCTOR_PARAMS size_t id, const std::string& name, const types::Mass<>& mass, units::LENGTH_T radius, const geometry::Point3<units::LENGTH_T>& position, geometry::Vec3<units::SPEED_T>& velocity, \
//////////                                    units::TIME_T rotation_period, units::ANGLE_T rotation_tilt, units::ANGLE_T rotation_alfa, units::ANGLE_T rotation, ConstBodyPtr parent
//////////
//////////    explicit Body(DECL_BODY_CONSTRUCTOR_PARAMS);
//////////
//////////    /**
//////////      * \brief  Generic body constructor for a body
//////////      *         Protected constructor so bodies cannot be instantiated, except by the derived classes
//////////      *         It validates the mass and the name, which must be unique (not case sensitive validation).
//////////////      * @param  id              The id of the body
//////////      * @param  name            The name of the body
//////////      * @param  reduced_mass    The reduced mass of the body (G*M)
//////////      * @param  position        The initial position of the body (in meters)
//////////      * @param  velocity        The initial velocity of the body (in m/s)
//////////      * @param  rotation_period The rotation period
//////////      * @param  rotation_tilt   The rotation axis tilt
//////////      * @param  rotation_alfa   The rotation axis angle relative to the X axis of the orbit
//////////      * @param  rotation        The initial rotation angle, relative to the X axis of the body's own CS
//////////////      * @param  parent          The body which this body is gravitationally bound to
//////////      * @throw  invalid_argument  If the name of the body has already been used or rotation period is negative
//////////      */
//////////#define DECL_BODY_CONSTRUCTOR_RED_MASS_PARAMS size_t id, const std::string& name, units::REDUCED_MASS_T reduced_mass, units::LENGTH_T radius, const geometry::Point3<units::LENGTH_T>& position, geometry::Vec3<units::SPEED_T>& velocity, \
//////////                                            units::TIME_T rotation_period, units::ANGLE_T rotation_tilt, units::ANGLE_T rotation_alfa, units::ANGLE_T rotation, ConstBodyPtr parent
//////////    explicit Body(DECL_BODY_CONSTRUCTOR_RED_MASS_PARAMS);
//////////
//////////    /**
//////////      * \brief  Deleted copy constructor.
//////////      */
//////////    Body(const Body& body) = delete;
//////////
//////////    /**
//////////      * \brief  Deleted assignment operator.
//////////      */
//////////    Body& operator= (const Body &body) = delete;
//////////
//////////

//////////
//////////    /**
//////////      * \brief  Body's acceleration in Cartesian coordinates, relative to the parent body or the reference CS, if the body has no parent. Measured in meters/second/second.
//////////      *         Updated to 0 after each movement
//////////      */
//////////    geometry::Vec3<units::ACCELERATION_T> _acceleration {0.0, 0.0, 0.0};
//////////
//////////    /**
//////////      * \brief  Body's rotation (spin) period in seconds
//////////      */
//////////    units::TIME_T _rotation_period{ 0 };
//////////
//////////    /**
//////////      * \brief  Body's rotation axis tilt in radians, from the Z axis of the body's orbit
//////////      */
//////////    units::ANGLE_T _rotation_tilt{ 0 };
//////////
//////////    /**
//////////      * \brief  Body's rotation axis angle in the ecliptic coordinates system, in radians
//////////      */
//////////    units::ANGLE_T _rotation_alfa{ 0 };
//////////
//////////    /**
//////////      * \brief  Body's rotation angle in radians, from the inertial x axis of the body's internal cartesian CS.
//////////      *         Updated after each movement.
//////////      */
//////////    units::ANGLE_T _rotation{ 0 };
//////////
//////////
//////////  private:
//////////    ///**
//////////    //  * \brief  Tree of existing bodies sorted by distance. Used to avoid body name duplication and for hierarchical grouping
//////////    //  */
//////////    //static utils::Tree<Body, SortingByDistance> _bodies_tree_dist;
//////////
//////////    ///**
//////////    //  * \brief  Tree of existing bodies sorted by mass. Used to avoid body name duplication and for hierarchical grouping
//////////    //  */
//////////    //static utils::Tree<Body, SortingByMassDesc> _bodies_tree_mass;
//////////
//////////  };
//////////
//////////} // END namespace physics
//////////
//////////#endif // BODY_H
//////////
//////////
//////////
//////////  ////////////
///////////////////////**
//////////////////////  *  Abstract Class representing an astronomical body with a mass, which can interact gravitationally with other bodies
//////////////////////  */
//////////////////////class Body{
//////////////////////
//////////////////////protected:
//////////////////////
//////////////////////  /**
//////////////////////    *  Body's former Parent Body
//////////////////////    */
//////////////////////  const Body *_former_parent_body {nullptr};
//////////////////////
//////////////////////  /**
//////////////////////    *  Body's biggest perturbation.
//////////////////////    */
//////////////////////  VAR_TYPE _max_perturbation {0};
//////////////////////
//////////////////////  /**
//////////////////////    *  Body causing the biggest perturbation. This is the 1st candidate to become the new parent body.
//////////////////////    */
//////////////////////  const Body *_max_perturbation_body {nullptr};
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////  /**
//////////////////////    *  Calculates accelerations due to gravitational perturbation by "grandparent" bodies
//////////////////////    *  Examples:
//////////////////////    *       - Any planet satellite and the Sun
//////////////////////    *       - Space ship orbiting a planet and the Sun
//////////////////////    *       - Space ship orbiting a satellite and its planet and the Sun
//////////////////////    *  Throws an exception if the bodies are too close to each other (COLLISION)
//////////////////////    */
//////////////////////  void interactGP() throw (std::string);
//////////////////////
//////////////////////  /**
//////////////////////    *  Calculates acceleration due to gravitational perturbation by another body
//////////////////////    *  Throws an exception if the bodies are too close to each other (COLLISION)
//////////////////////    */
//////////////////////  void interact(const Body &sec_body) throw (std::string);
//////////////////////
//////////////////////  /**
//////////////////////    *  Moves this body according to the elapsed time, delta_time, current position,velocity and acceleration. A new velocity is also calculated
//////////////////////    *  Returns: a flag indicating if the parent body has changed.
//////////////////////    */
//////////////////////  virtual bool move(TimeType delta_time);
//////////////////////
//////////////////////
//////////////////////
//////////////////////  /**
//////////////////////    *  Pure virtual member: returns the previous parent body. To be implemented by children classes which represent bodies with a parent
//////////////////////    */
//////////////////////  const Body& formerParent() const {
//////////////////////    if (_former_parent_body)
//////////////////////      return *_former_parent_body;
//////////////////////    else
//////////////////////      throw std::string("NOT FOUND");
//////////////////////  }
//////////////////////
//////////////////////};
//////////////////////
//////////////////////
//////////////////////#endif // BODY_H_
//////////

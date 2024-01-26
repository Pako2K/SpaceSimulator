#include <physics/k_body.h>

#include <files/properties_file_reader.h>
#include <logger.h>


using namespace physics;
using namespace geometry;


std::string to_roman(size_t numeral);


static const std::string  __PROPS_FILE_NAME__{ "config/body.cfg" }; /**< Location of the config file with the body configuration */


bool KBody::_initialized{ false };
bool KBody::_barycenters_set{ false };
double KBody::_barycenter_ratio_limit{ 0 };


void KBody::initialize() {
  // Read properties file
  utils::PropertiesFileReader properties(__PROPS_FILE_NAME__);

  // Initialize variables
  _barycenter_ratio_limit = properties.property<double>("BARYCENTER_LIMIT");

  _initialized = true;
}


void KBody::barycenters(tree::MTree<KBody>& bodies) {
  // Barycenter of the main star and the perturbator bodies
  barycenter(bodies, bodies.root());

  // Reset positions and velocities of all the bodies, so the new coordinates system is inertial and 
  //    centered at the barycenter
  auto iter = bodies.begin();
  while (iter.hasNext()) {
    auto& body = iter.next();
    body._position -= bodies.root()._barycenter_pos.vec();
    body._velocity -= bodies.root()._barycenter_vel;
    body._barycenter_pos -= bodies.root()._barycenter_pos.vec();
    body._barycenter_vel -= bodies.root()._barycenter_vel;
  }

  // Barycenter of the rest of main children bodies with their own children bodies (satellites or planets, in case of secondary stars)
  auto child_iter = bodies.children(bodies.root().matchingKey());
  while (child_iter.hasNext()) {
    auto& child_body = child_iter.next();
    barycenter(bodies, child_body);

    // Go one level down for secondary stars and calculate the barycenter of its planets
    if (child_body.TYPE == KBody::STAR) {
      auto grand_child_iter = bodies.children(child_body.matchingKey());
      while (grand_child_iter.hasNext()) {
        barycenter(bodies, grand_child_iter.next());
      }
    }
  }

  _barycenters_set = true;
}


void KBody::barycenter(tree::MTree<KBody>& bodies, KBody& parent_body) {
  // Barycenter position and velocity are defined as:
  //      Bpos = sum(m(i) / M * r(i)); 
  //      Bvel = sum(m(i) / M * v(i)); 
  //      where M = sum(m(i))

  auto iter = bodies.children(parent_body.matchingKey());
  //      Skip calculation if there aren't any children
  if (!iter.hasNext()) {
    return;
  }

  //      First calculate the total mass of the system
  types::Mass<> total_mass{ parent_body.reduced_mass };
  while (iter.hasNext()) {
    auto& child_body = iter.next();
    // Only include the mass of the bodies which are defined as perturbators
    if (child_body._parent_perturbator)
      total_mass += child_body.reduced_mass;
  }
  //      Now calculate position and velocity
  iter.rewind();
  PositionType bary_pos{ (parent_body.reduced_mass / total_mass) * parent_body._position.vec() };
  VelocityType bary_vel{ (parent_body.reduced_mass / total_mass) * parent_body._velocity };
  while (iter.hasNext()) {
    auto& child_body = iter.next();
    // Only include bodies which are defined as perturbators
    if (child_body.parentPerturbator()) {
      DebugLog("Added body to the barycenter calculation: " + child_body.name());
      bary_pos += (child_body.reduced_mass / total_mass) * child_body._position.vec();
      bary_vel += (child_body.reduced_mass / total_mass) * child_body._velocity;
    }
  }
  // Update the parent body with the barycenter
  parent_body._barycenter_pos = bary_pos;
  parent_body._barycenter_vel = bary_vel;

  DebugLog("For " << parent_body.name() << "\nBarycenter Position:\n" << bary_pos << "\nBarycenter velocity:\n" << bary_vel);
  DebugLog("    Barycenter Position (rel):\n" << bary_pos.vec() - parent_body.position().vec()  << "\n    Barycenter velocity(rel):\n" << bary_vel - parent_body.velocity());
}


void KBody::gravInteraction(tree::MTree<KBody>& bodies, const units::TIME_T& delta_t) {
  // *********************************************************************************************
  // APPROXIMATION 0 : Interaction only with the parent body, according to its Keplerian orbit 
  // *********************************************************************************************
  // Nothing to do: new position and velocity can be calculated during the execution of the movement
  //      1. First level under the root body
  auto iter = bodies.children(bodies.root().matchingKey());
  while (iter.hasNext()) {
    auto& body = iter.next();
    body.keplerMove(delta_t);

    //      2. Second level under the root body
    auto iter_children = bodies.children(body.matchingKey());
    while (iter_children.hasNext()) {
      auto& child_body = iter_children.next();
      child_body.keplerMove(delta_t);

      //      3. Third level under the root body
      // TBD !!!
    }
    
  }


  // After all bodies have been moved, their keplerian orbits must be recalculated for all the children bodies 
  //    which parents have a barycenter not matching its position
  if (bodies.root().barycenterPos() != bodies.root().position() ||
    bodies.root().barycenterVel() != bodies.root().velocity()) {

    iter.rewind();
    while (iter.hasNext()) {
      auto& body = iter.next();
      body._orbit.reset(new KeplerOrbit(body.parent(), body));
    }
    //      2. Second level under the root body
    // TBD !!!
  }

  // *********************************************************************************************
  // APPROXIMATION 1 : ???
  // *********************************************************************************************
  //for (size_t body_1 = 0; body_1 < bodies.size() - 1; body_1++) {
  //  for (size_t body_2 = body_1 + 1; body_2 < bodies.size(); body_2++) {
  //    geometry::Vec3<units::LENGTH_T> dist = bodies[body_2]->_position.vec() - bodies[body_1]->_position.vec();
  //    bodies[body_1]->_acceleration += dist.normalized() * units::GRAV_CONST * bodies[body_2]->_mass() / dist.squaredNorm();
  //    bodies[body_2]->_acceleration += -dist.normalized() * units::GRAV_CONST * bodies[body_1]->_mass() / dist.squaredNorm();
  //  }
  //  bodies[body_1]->move(delta_t);
  //}
  //bodies[bodies.size() - 1]->move(delta_t);
}


KBody::KBody(DECL_BODY_CONSTRUCTOR_PARAMS, KBody& parent, BodyType type, int64_t id, std::string provisional_name)
  : PBody{ uniqueName(type, id, name, provisional_name, parent.name()), mass, radius, position, velocity }, _parent{ &parent },
  TYPE{ type }, _barycenter_pos{ position }, _barycenter_vel{ velocity }, ID{ id }, COMMON_NAME{ name }, PROVISIONAL_NAME{ provisional_name } {

  commonConstructor();
}

KBody::KBody(DECL_BODY_CONSTRUCTOR_PARAMS) 
  : PBody{ name, mass, radius, position, velocity }, _parent{ nullptr }, TYPE{ STAR }, _barycenter_pos{ position }, _barycenter_vel{ velocity }, ID{ 1 }, COMMON_NAME{ name }, PROVISIONAL_NAME{ "" }{

  commonConstructor();
}

KBody::KBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS, KBody& parent, BodyType type, int64_t id, std::string provisional_name)
  : PBody{ uniqueName(type, id, name, provisional_name, parent.name()), reduced_mass, radius, position, velocity }, _parent{ &parent },
  TYPE{ type }, _barycenter_pos{ position }, _barycenter_vel{ velocity }, ID{ id }, COMMON_NAME{ name }, PROVISIONAL_NAME{ provisional_name } {

  commonConstructor();
}

KBody::KBody(DECL_BODY_CONSTRUCTOR_RM_PARAMS)
  : PBody{ name, reduced_mass, radius, position, velocity }, _parent{ nullptr }, TYPE{ STAR }, _barycenter_pos{ position }, _barycenter_vel{ velocity }, ID{ 1 }, COMMON_NAME{ name }, PROVISIONAL_NAME{ "" } {

  commonConstructor();
}
  

void KBody::commonConstructor() {
  if (!_initialized)
    throw std::runtime_error("General Body configuration not initialized");

  if(_barycenters_set)
    throw std::runtime_error("Barycenters are defined. No more bodies can be created");

  if (!_parent)
    return;

  if (!validateTypes(_parent->TYPE, TYPE))
    throw std::invalid_argument("Parent and child body types are not compatible");

  // Create the keplerian orbit
  _orbit.reset(new KeplerOrbit(*_parent, *this));

  // Determine whether the body contributes to parent perturbation (around their common barycenter)
  Vec3<units::LENGTH_T> v_rel_position = _position.vec() - _parent->_position.vec();
  units::LENGTH_T l_rel_position = v_rel_position.norm();

  units::LENGTH_T contr_barycenter = reduced_mass / _parent->reduced_mass * l_rel_position;
  if (contr_barycenter > _barycenter_ratio_limit * _parent->radius) {
    _parent_perturbator = true;
  }
}


std::string KBody::uniqueName(BodyType type, int64_t id, const std::string& name, const std::string& provisional_name, const std::string& parent_name) {
  switch (type) {
  case STAR: return name;
  case PLANET: return name;
  case DWARF_PLANET: return std::to_string(id) + " " + (name.size() ? name : provisional_name);
  case MINOR_BODY: return std::to_string(id) + " " + (name.size() ? name : provisional_name);
  case SATELLITE: return (name.size() ? name : provisional_name) + " - " + parent_name + " " + to_roman(id);
  default: return name;
  }
}


bool KBody::validateTypes(BodyType parent_type, BodyType child_type) {
  if (parent_type == STAR) {
    if (child_type != SATELLITE)
      return true;
    else
      return false;
  }
  else if (parent_type == PLANET || parent_type == DWARF_PLANET || parent_type == MINOR_BODY) {
    if (child_type == SATELLITE)
      return true;
    else
      return false;
  }
  else // SATELLITE
    return false;
}


void KBody::keplerMove(const units::TIME_T& delta_t) {
  // *********************************************************************************************************
  // APPROXIMATION 0 : Movement due to interaction only with the parent body, according to its Keplerian orbit 
  // *********************************************************************************************************
  // Keplerian orbits have as a result a new anomaly(pos and vel relative to the parent)
  // The change in the parent CS is returned
  auto change = _orbit->forward(delta_t);

  // If this body is a perturbator of the parent
  if (_parent_perturbator) {
    // Apply this relative change to the position and velocity in the inertial Barycenter CS
    // dR = Mparent / (m + MParent) dR(rel2Parent)
    // dV = Mparent / (m + MParent) dV(rel2Parent)
    auto pair_mass = _parent->reduced_mass + reduced_mass;
    _position += _parent->reduced_mass / pair_mass * change.first;
    _velocity += _parent->reduced_mass / pair_mass * change.second;

    // And now apply the perturbation to the parent body and the relative position with their barycenter
    // dR(parent) = - sum(i = 1, N) [m / (m + MParent) dR(rel2Parent)
    // dV(parent) = - sum(i = 1, N) [m / (m + MParent) dR(rel2Parent)
    _parent->_position -= reduced_mass / pair_mass * change.first;
    _parent->_velocity -= reduced_mass / pair_mass * change.second;

    // Unless the parent is the root body, the barycenter is not inertial and must be updated
    if (_parent->_parent) {
      // Apply changes to the barycenter (3 body problem)
      throw std::exception("TO BE DONE");
    }
  }
  else {
    // Relative change equals absolute change
    _position = _parent->position() + _orbit->position().vec();// += change.first;
    _velocity = _parent->velocity() + _orbit->velocity();// += change.second;
  }
}



std::string to_roman(size_t numeral) {
  std::vector<std::string> roman_nums{ "Nulo", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
    "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX", "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL", "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L",
    "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", "LX", "LXI", "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", "LXIX", "LXX", "LXXI", "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", "LXXVIII", "LXXIX", "LXXX",
    "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", "LXXXVII", "LXXXVIII", "LXXXIX", "XC", "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", "XCVII", "XCVIII", "XCIX", "C", "CI", "CII", "CIII", "CIV", "CV", "CVI", "CVII", "CVIII", "CIX", "CX",
    "CXI", "CXII", "CXIII", "CXIV", "CXV", "CXVI", "CXVII", "CXVIII", "CXIX", "CXX", "CXXI", "CXXII", "CXXIII", "CXXIV", "CXXV", "CXXVI", "CXXVII", "CXXVIII", "CXXIX", "CXXX", "CXXXI", "CXXXII", "CXXXIII", "CXXXIV", "CXXXV", "CXXXVI", "CXXXVII", "CXXXVIII", "CXXXIX", "CXL",
    "CXLI", "CXLII", "CXLIII", "CXLIV", "CXLV", "CXLVI", "CXLVII", "CXLVIII", "CXLIX", "CL", "CLI", "CLII", "CLIII", "CLIV", "CLV", "CLVI", "CLVII", "CLVIII", "CLIX", "CLX", "CLXI", "CLXII", "CLXIII", "CLXIV", "CLXV", "CLXVI", "CLXVII", "CLXVIII", "CLXIX", "CLXX",
    "CLXXI", "CLXXII", "CLXXIII", "CLXXIV", "CLXXV", "CLXXVI", "CLXXVII", "CLXXVIII", "CLXXIX", "CLXXX", "CLXXXI", "CLXXXII", "CLXXXIII", "CLXXXIV", "CLXXXV", "CLXXXVI", "CLXXXVII", "CLXXXVIII", "CLXXXIX", "CXC", "CXCI", "CXCII", "CXCIII", "CXCIV", "CXCV", "CXCVI", "CXCVII", "CXCVIII", "CXCIX", "CC" };

  return roman_nums.at(numeral);
}
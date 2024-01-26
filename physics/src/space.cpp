#include "physics/space.h"


#include <iomanip>
#include <ctime>
#include <sstream>

#include <logger.h>
#include <sqlitedb/sqlitedb.h>

#include <physics/k_body.h>


using namespace physics;
using namespace physics::units;
using namespace sqlitedb;

////////using namespace geometry;


static const std::string  __PROPS_FILE_NAME__   { "config/space.cfg" }; /**< Location of the config file with the space configuration */

/// ***********************************************************************************************************************
/// ******************************************** STATIC Data Members ******************************************************

Space *Space::_space_ptr {nullptr};

/// ******************************************** STATIC Data Members (END) ************************************************
/// ***********************************************************************************************************************


/// ***********************************************************************************************************************
/// ************************************************* PUBLIC **************************************************************
Space& Space::create() {
  if ( !_space_ptr ) {
    _space_ptr = new Space();
  }
  return *_space_ptr;
}


void Space::destroy() {
  delete _space_ptr;
}

size_t Space::addObserver() {
  _observers.push_back(new Observer(_observers.size() + 1));
  return (_observers.size() - 1);
}

void Space::removeObserver(size_t obs_id) {
  if (obs_id < _observers.size() && obs_id > 0 && _observers[obs_id] != nullptr) {
    delete _observers[obs_id];
    _observers[obs_id] = nullptr;
  }
}

Observer& Space::observer(size_t obs_id) { 
  //if (obs_id < _observers.size() && _observers[obs_id] != nullptr)
    return *_observers.at(obs_id);
}

Observer& Space::activeObs(size_t obs_id) {
  if (obs_id < _observers.size() && _observers[obs_id] != nullptr)
    _active_obs = obs_id;

  return *_observers[_active_obs];
}


void Space::runTick() {
  _elapsed_time += _tick;

  ////////// Move the observer
  ////////_observers[_active_obs]->move();
  ////////
  ////////// Let the bodies interact
  ////////KBody::gravInteraction(_bodies, _tick);  
}


void Space::tick(const TIME_T &new_tick) {
  _tick = new_tick;
}


/// ************************************************* PUBLIC (END) ********************************************************
/// ***********************************************************************************************************************


/// ***********************************************************************************************************************
/// ************************************************* PROTECTED ***********************************************************

Space::Space() {
  DebugLog( "Space: CONSTRUCTOR Called" );

  // Read properties file
  utils::PropertiesFileReader properties(__PROPS_FILE_NAME__);

	// Initialize variables
  _datetime_format = std::make_unique<std::pair<std::string, std::string>>(properties.property("DATE_FORMAT"), properties.property("TIME_FORMAT"));
  std::istringstream iss_init_date_time { properties.property("INIT_DATE_TIME") };
  std::tm tm_datetime;
  std::string date_time_format = _datetime_format->first + " " + _datetime_format->second;
  // Extract time
  iss_init_date_time >> std::get_time(&tm_datetime, date_time_format.c_str());

  if (iss_init_date_time.fail())
    throw std::string("Invalid Initial Date/Time. Expected format: " + _datetime_format->first + " " + _datetime_format->second);

  _init_date_time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(mktime(&tm_datetime)));
  DebugLog( "Initial Time: " + std::to_string(std::chrono::system_clock::to_time_t(_init_date_time)) );

  _tick = static_cast<TIME_T>(properties.property<int32_t>("TICK"));

  _elapsed_time = static_cast<TIME_T>(0);

  ////////// Create Default Observer
  ////////Point3<LENGTH_T> init_pos;
  ////////init_pos[0] = properties.propertyCast<LENGTH_T>("OBSERVER_X")[0];
  ////////init_pos[1] = properties.propertyCast<LENGTH_T>("OBSERVER_Y")[0];
  ////////init_pos[2] = properties.propertyCast<LENGTH_T>("OBSERVER_Z")[0];
  ////////_observers.push_back(new Observer(_observers.size() + 1));
  ////////_observers[_active_obs]->reposition(init_pos);

 
  // Create bodies from DB
  createBodies(properties);

  ////////// Determine Barycenters for all the parent bodies and reset CS to the system barycenter, which is an inertial CS
  ////////KBody::barycenters(_bodies);
}

Space::~Space() {
  DebugLog( "Space: DESTROYED" );
  while (_observers.size()) {
    delete _observers.back();
    _observers.pop_back();
  }
}

/// ************************************************* PROTECTED (END) *****************************************************
/// ***********************************************************************************************************************


/// ***********************************************************************************************************************
/// ************************************************* PRIVATE *************************************************************
void Space::createBodies(const utils::PropertiesFileReader& properties) {
  // Initialize Body parameters
  KBody::initialize();
    
  // Check the DB type
  if (properties.property("DB.TYPE").size() == 0) {
    throw std::runtime_error("DB.TYPE parameter not found in " + __PROPS_FILE_NAME__);
  }

  if (properties.property("DB.TYPE") == "SQLITE") {
    // Open DB connection
    SQLiteDB db(properties.property("DB." + properties.property("DB.TYPE")));
        
    // Get body types
    auto query_body_types = db.createSQL("SELECT bty_id, bty_name FROM bty_body_types WHERE bty_active = 1");
    std::map<std::string, int> bod_types;
    std::map<int, std::string> bod_types_byID;
    while (query_body_types.execute()) {
      bod_types[query_body_types.fetchValue<std::string>(1)] = query_body_types.fetchValue<int>(0);
      bod_types_byID[query_body_types.fetchValue<int>(0)] = query_body_types.fetchValue<std::string>(1);
    }

    // Read bodies table 
    enum body_col { id, name, prov_name, typ_id, parent_id, mass, reduced_mass, radius, posX, posY, posZ, velX, velY, velZ };
        
    auto query_body = db.createSQL("SELECT bod_number, bod_name, bod_prov_name, bod_typ_id, bod_parent_id, bod_mass, bod_reduced_mass, bod_avg_radius, eph_pos_x, eph_pos_y, eph_pos_z, eph_vel_x, eph_vel_y, eph_vel_z "
                                "FROM bod_bodies, eph_ephemeris "
                                "WHERE bod_active = 1 and bod_id = eph_bod_id and eph_sim_id is NULL and eph_time = ?");
    
    query_body.bind(1, long(std::chrono::duration_cast<TIME_T>(_init_date_time.time_since_epoch()).count()));
    
    // Variables used to read the ephemeris
    geometry::Point3<LENGTH_T> pos;
    geometry::Vec3<SPEED_T> vel;

    while (query_body.execute()) {
      // Check whether the body type is active
      if (bod_types_byID.count(query_body.fetchValue<int>(typ_id)) == 0)
        continue;

      for (int i = 0; i < 3; i++) {
        pos[i] = LENGTH_T(query_body.fetchValue<LENGTH_T>(i + posX));
        vel[i] = SPEED_T(query_body.fetchValue<SPEED_T>(i + velX));
      }
    
      // Read parent body name and find parent body
      KBody* parent{ nullptr };
      auto body_parent_id = query_body.fetchValue<int64_t>(parent_id);
      if (body_parent_id) {
        try {
          auto query_parent = db.createSQL("select bod_name from bod_bodies where bod_id = ?");
          query_parent.bind(1, body_parent_id).execute();
          auto parent_name = query_parent.fetchValue<std::string>(0);
          parent = &_bodies.find(parent_name);
        }
        catch (std::out_of_range& exc) {
          std::stringstream txt;
          txt << exc.what() << " - Exception in " << __FILE__ << " on line " << __LINE__ << "\n" << "ERROR: Parent Body for " << query_body.fetchValue<std::string>(name) << " not found." << "/n";
          ErrorLog(txt.str());
          throw std::exception("Parent Body not found. Make sure the parent body is created before its children");
        }
      }

//#define BODY_PARAMS     query_body.fetchValue<int64_t>(id), query_body.fetchValue<std::string>(name), types::Mass<>(query_body.fetchValue<double>(mass)), query_body.fetchValue<LENGTH_T>(radius), pos, vel
//#define BODY_RM_PARAMS  query_body.fetchValue<int64_t>(id), query_body.fetchValue<std::string>(name), query_body.fetchValue<double>(reduced_mass), query_body.fetchValue<LENGTH_T>(radius), pos, vel

#define BODY_PARAMS     query_body.fetchValue<std::string>(name), types::Mass<>(query_body.fetchValue<double>(mass)), query_body.fetchValue<LENGTH_T>(radius), pos, vel
#define BODY_RM_PARAMS  query_body.fetchValue<std::string>(name), query_body.fetchValue<double>(reduced_mass), query_body.fetchValue<LENGTH_T>(radius), pos, vel

      auto body_type_id = query_body.fetchValue<int>(typ_id);
      auto body_reduced_mass = query_body.fetchValue<double>(reduced_mass);
      KBody::BodyType body_type;
      
      // Create STARS
      if (body_type_id == bod_types["STAR"]) {
        body_type = KBody::BodyType::STAR;
        // If star is the main star (no parent)
        if (!parent) {
          // If reduced mass is not available
          if (body_reduced_mass == 0)
            _bodies.root(new KBody(BODY_PARAMS));
          else
            _bodies.root(new KBody(BODY_RM_PARAMS));

          // Go to the next body
          continue;
        }
      }
      else if (body_type_id == bod_types["PLANET"]) {
        body_type = KBody::BodyType::PLANET;
      }
      else if (body_type_id == bod_types["DWARF_PLANET"]) {
        body_type = KBody::BodyType::DWARF_PLANET;
      }
      else if (body_type_id == bod_types["SATELLITE"]) {
        body_type = KBody::BodyType::SATELLITE;
      }
      else if (body_type_id == bod_types["MINOR_BODY"]) {
        body_type = KBody::BodyType::MINOR_BODY;
      }
      else if (body_type_id == bod_types["SHIP"]) {
//        body_type = KBody::BodyType::SHIP;
        continue;
      }
      else {
        std::stringstream txt;
        txt << " - Exception in " << __FILE__ << " on line " << __LINE__ << "\n" << "ERROR: Body Type  " << body_type_id << " not recognized." << "/n";
        ErrorLog(txt.str());
        throw std::exception("Body Type not recognized");
      }

      // If reduced mass is not available
      if (body_reduced_mass == 0)
        _bodies.addNode(new KBody(BODY_PARAMS, *parent, body_type, query_body.fetchValue<int64_t>(id), query_body.fetchValue<std::string>(prov_name)), parent->name());
      else
        _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, body_type, query_body.fetchValue<int64_t>(id), query_body.fetchValue<std::string>(prov_name)), parent->name());
      
#undef BODY_PARAMS
#undef BODY_RED_MASS_PARAMS       
    }
  }
  else  if (properties.property("DB.TYPE") == "MYSQL") {
    throw std::runtime_error("DB.TYPE not supported: " + properties.property("DB.TYPE"));
    //////// // auto table_eph = connection->getDefaultSchema().getTable("eph_ephemeris");
    ////////
    ////////  // Get body types
    ////////  auto res_body_types = connection->sql("select typ_id, typ_name from typ_types where typ_active = 1").execute();
    ////////  std::map<std::string, int> bod_types;
    ////////  std::map<int, std::string> bod_types_byID;
    ////////  for (auto row : res_body_types.fetchAll()) {
    ////////    bod_types[row[1]] = row[0];
    ////////    bod_types_byID[row[0]] = row[1];
    ////////  }
    ////////  
    ////////  // Read bodies table 
    ////////  enum body_col { id, name, typ_id, parent_id, mass, reduced_mass, radius, posX, posY, posZ, velX, velY, velZ };
    ////////
    ////////  auto time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    ////////  DEBUG_LOG("Loading active bodies from DB. START: " + std::to_string(time_stamp.count()));
    ////////
    ////////  auto res_body = connection->sql("select bod_id, bod_name, bod_typ_id, bod_parent_id, bod_mass, bod_reduced_mass, bod_avg_radius, eph_pos_x, eph_pos_y, eph_pos_z, eph_vel_x, eph_vel_y, eph_vel_z " 
    ////////                                  "from bod_bodies, eph_ephemeris "
    ////////                                  "where bod_active = 1 and bod_id = eph_bod_id and eph_sim_id is NULL and eph_time = " + std::to_string(std::chrono::duration_cast<physics::units::TIME_T>(_init_date_time.time_since_epoch()).count())).execute();
    ////////
    ////////  DEBUG_LOG("Loading active bodies from DB. END: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time_stamp.count()));
    ////////  time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    ////////
    ////////  // Variables used to read the ephemeris
    //////////  auto ephemeris_time = std::chrono::duration_cast<physics::units::TIME_T>(_init_date_time.time_since_epoch()).count();
    ////////  Point3<LENGTH_T> pos;
    ////////  Vec3<SPEED_T> vel;
    //////////  std::string eph_where_str{ "eph_bod_id = :body_id AND eph_time = :time AND eph_sim_id is NULL" };
    ////////
    ////////
    ////////  for (auto row : res_body.fetchAll()) {
    ////////    // Check if the body type is active
    ////////    if (bod_types_byID.count(row[typ_id]) == 0)
    ////////      continue;
    ////////
    ////////    for (int i = 0; i < 3; i++) {
    ////////      pos[i] = LENGTH_T(row[i + posX]);
    ////////      vel[i] = SPEED_T(row[i + velX]);
    ////////    }
    ////////    DEBUG_LOG("   Set ephemeris. END: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time_stamp.count()));
    ////////    time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    ////////
    ////////
    ////////    // Read parent body name and find parent body
    ////////    KBody* parent{ nullptr };
    ////////    if (!row[parent_id].isNull()) {
    ////////      try {
    ////////        auto res_parent = connection->sql("select bod_name from bod_bodies where bod_id = " + std::to_string(size_t(row[parent_id]))).execute();
    ////////        std::string parent_name = res_parent.fetchOne()[0].get<std::string>();
    ////////
    ////////        parent = &_bodies.find(parent_name);
    ////////      }
    ////////      catch (std::out_of_range& exc) {
    ////////        std::stringstream txt;
    ////////        txt << exc.what() << " - Exception in " << __FILE__ << " on line " << __LINE__ << "\n" << "ERROR: Parent Body for " << row[name].get<std::string>() << " not found." << "/n";
    ////////        ERROR_LOG(txt.str());
    ////////        throw std::exception("Parent Body not found");
    ////////      }
    ////////      catch (mysqlx::Error &e) {
    ////////        std::stringstream txt;
    ////////        txt << "SQLException in " << __FILE__ << " on line " << __LINE__ << "\n" << "ERROR: " << e.what() << "/n";
    ////////        ERROR_LOG(txt.str());
    ////////        throw e;
    ////////      }
    ////////    }
    ////////
    ////////    DEBUG_LOG("   Read Parent name from DB. END: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time_stamp.count()));
    ////////    time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    ////////
    ////////
    ////////#define BODY_PARAMS     row[id].get<std::size_t>(), row[name].get<std::string>(), types::Mass<>(row[mass].get<double>()), row[radius].get<LENGTH_T>(), pos, vel
    ////////#define BODY_RM_PARAMS  row[id].get<std::size_t>(), row[name].get<std::string>(), row[reduced_mass].get<double>(), row[radius].get<LENGTH_T>(), pos, vel
    ////////
    ////////    // Create STARS
    ////////    if (int(row[typ_id]) == bod_types["STAR"]) {
    ////////      // Parse color
    ////////      //std::vector<std::string> colorStr;
    ////////      //utils::Tokenizer tknzr;
    ////////      //tknzr(colorStr, row[12], ',');
    ////////      //std::array<uint8_t, 3> color{ uint8_t(atoi(colorStr[0].data())), uint8_t(atoi(colorStr[1].data())), uint8_t(atoi(colorStr[2].data())) };
    ////////      
    ////////      // If star has a parent
    ////////      if (parent) {
    ////////
    ////////        // If reduced mass is not available
    ////////        if (row[reduced_mass].isNull())
    ////////          _bodies.addNode(new KBody(BODY_PARAMS, *parent, KBody::BodyType::STAR), parent->name());
    ////////        else
    ////////          _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, KBody::BodyType::STAR), parent->name());
    ////////      }
    ////////      else {
    ////////        // If reduced mass is not available
    ////////        if (row[reduced_mass].isNull())
    ////////          _bodies.root(new KBody(BODY_PARAMS));
    ////////        else
    ////////          _bodies.root(new KBody(BODY_RM_PARAMS));
    ////////
    ////////      }
    ////////    }
    ////////    // Create PLANETS
    ////////    else if (int(row[typ_id]) == bod_types["PLANET"]) {
    ////////
    ////////      // If reduced mass is not available
    ////////      if (row[reduced_mass].isNull())
    ////////        _bodies.addNode(new KBody(BODY_PARAMS, *parent, KBody::BodyType::PLANET), parent->name());
    ////////      else
    ////////        _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, KBody::BodyType::PLANET), parent->name());
    ////////    }
    ////////    // Create DWARF PLANETS
    ////////    else if (int(row[typ_id]) == bod_types["DWARF_PLANET"]) {
    ////////
    ////////      // If reduced mass is not available
    ////////      if (row[reduced_mass].isNull())
    ////////        _bodies.addNode(new KBody(BODY_PARAMS, *parent, KBody::BodyType::DWARF_PLANET), parent->name());
    ////////      else
    ////////        _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, KBody::BodyType::DWARF_PLANET), parent->name());
    ////////    }
    ////////    // Create SATELLITES
    ////////    else if (int(row[typ_id]) == bod_types["SATELLITE"]) {
    ////////      // If reduced mass is not available 
    ////////      if (row[reduced_mass].isNull())
    ////////        _bodies.addNode(new KBody(BODY_PARAMS, *parent, KBody::BodyType::SATELLITE), parent->name());
    ////////      else
    ////////        _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, KBody::BodyType::SATELLITE), parent->name());
    ////////    }
    ////////    // Create MINOR BODIES
    ////////    else if (int(row[typ_id]) == bod_types["MINOR"]) {
    ////////      // If reduced mass is not available
    ////////      if (row[reduced_mass].isNull())
    ////////        _bodies.addNode(new KBody(BODY_PARAMS, *parent, KBody::BodyType::MINOR_BODY), parent->name());
    ////////      else
    ////////        _bodies.addNode(new KBody(BODY_RM_PARAMS, *parent, KBody::BodyType::MINOR_BODY), parent->name());
    ////////    }
    ////////    // Create SHIPS
    ////////    else if (int(row[typ_id]) == bod_types["SHIP"]) {
    ////////    }
    ////////    else {
    ////////      std::stringstream txt;
    ////////      txt << " - Exception in " << __FILE__ << " on line " << __LINE__ << "\n" << "ERROR: Body Type  " << int(row[typ_id]) << " not recognized." << "/n";
    ////////      ERROR_LOG(txt.str());
    ////////      throw std::exception("Body Type not recognized");
    ////////    }
    ////////    DEBUG_LOG("Body created: " + row[name].get<std::string>() + " - " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time_stamp.count()));
    ////////    time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    ////////
    ////////#undef BODY_PARAMS
    ////////#undef BODY_RED_MASS_PARAMS       
    ////////  }
    ////////
    ////////
    ////////  std::stringstream log;
    ////////  log << "Trees:\nBODIES SORTED BY MASS:\n" << _bodies << std::endl;
    ////////  DEBUG_LOG(log.str());
  }
  else {
    throw std::runtime_error("DB.TYPE not supported: " + properties.property("DB.TYPE"));
  }

  DebugLog("Trees:\nBODIES SORTED BY MASS:\n" << _bodies);

#ifdef DEBUG_LOG_LEVEL
  DebugLog("   BODIES SORTED BY NAME (PLAIN):");
  auto iter = _bodies.begin();
  while (iter.hasNext()) {
    DEBUG_LOG((*iter).name());
    iter.next();
  }

  DebugLog("   BODIES SORTED BY NAME (TREE):");
  DebugLog("   " + _bodies.root().name());
  std::set<std::string> names;
  auto child_iter = _bodies.children(_bodies.root().name());
  while (child_iter.hasNext()) {
    names.insert((*child_iter).name());
    
    auto grand_child_iter = _bodies.children((*child_iter).name());
    std::set<std::string> grand_child_names;
    while (grand_child_iter.hasNext()) {
      grand_child_names.insert((*grand_child_iter).name());
      grand_child_iter.next();
    }
    for (auto& name : grand_child_names)
      DebugLog("         " + name);

    child_iter.next();
  }
  for (auto& name : names)
    DebugLog("      " + name);


  DebugLog("   BODIES SORTED BY DISTANCE (TREE):");
  DebugLog("   " + _bodies.root().name());
  std::map<units::LENGTH_T, std::string> by_distance;
  child_iter.rewind();
  while (child_iter.hasNext()) {
    by_distance[(*child_iter).orbit().a()]=(*child_iter).name();

    auto grand_child_iter = _bodies.children((*child_iter).name());
    std::map<units::LENGTH_T, std::string> grand_child_by_distance;
    while (grand_child_iter.hasNext()) {
      grand_child_by_distance[(*grand_child_iter).orbit().a()] = (*grand_child_iter).name();
      grand_child_iter.next();
    }
    for (auto& pair : grand_child_by_distance)
      DebugLog("         " + pair.second);

    child_iter.next();
  }
  for (auto& pair : by_distance)
    DebugLog("      " + pair.second);
#endif
}

/// ************************************************* PRIVATE (END) *******************************************************
/// ***********************************************************************************************************************

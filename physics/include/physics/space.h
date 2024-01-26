#ifndef SPACE_H
#define SPACE_H

#include <string>
#include <memory>
#include <chrono>

//////#include <mysqlx/xdevapi.h>

#include <files/properties_file_reader.h>
#include <misc/formatDateTime.h>

#include <physics/units.h>
#include <physics/observer.h>
#include <physics/k_body.h>

#include <collections/mtree.h>


namespace physics
{

  /**
    *  \brief  Space simulation class. 
    *          It contains one or more observers and a collection of different types of bodies
    *          The spcae wvolves according to a tick time, which can be increased or decreased (in seconds, min value = 1)
    */
  class Space
  {
    public:
      /* *********************************************** Operations ************************************************************* */
      /**
        *  \brief  Copy constructor: DELETED
        */
      Space(const Space&) = delete;

      /**
        *  \brief  Move constructor: DELETED
        */
      Space(Space&&) = delete;

      /**
        *  \brief  Assignment operation: DELETED
        */
      Space& operator= (const Space&) = delete;

      /**
        *  \brief  Move Assignment operation: DELETED
        */
      Space& operator= (const Space&&) = delete;

      /**
        *  \brief Creates a Space singleton instance
        *  @param
        *  @return a reference to the space singleton object
        */
      static Space& create();

      /**
        *  \brief Destroys the Space singleton instance
        *  @return void
        */
      void destroy();

      /**
        *  \brief  Add an observer
        *  @return The  id of the added observer
        */
      size_t addObserver();

      /**
        *  \brief  Remove an observer, if the id exists
        *  @param  The id of the observer to be removed
        */
      void removeObserver(size_t obs_id);

      /**
        *  \brief  Get an observer from the list of observers or the active one if the observer id does not exists
        *  @param  The id of the observer to be returned
        *  @return A reference to the active observer
        */
      Observer& observer(size_t obs_id);

      /**
        *  \brief Get the active observer
        *  @return A reference to the active observer
        */
      Observer& activeObs() { return *_observers[_active_obs]; }

      /**
        *  \brief  Switches the active observer to the indicated id, if it exists
        *  @param  The id of the observer to be made active
        *  @return A reference to the new active observer
        */
      Observer& activeObs(size_t obs_id);


      /**
        *  \brief  Increases the space time by a tick
        *          Executes all the interactions and updates the state of all the bodies
        *  @return void
        */
      void runTick();

      /**
        *  \brief  Update Tick
        *  @param   new_tick  The new tick time
        *  @return  void
        */
      void tick(const units::TIME_T &new_tick);


      /**
        *  \brief  GET Operations
        */
      auto tick() const { return _tick; }
      auto elapsedTime() const { return _elapsed_time; }
      auto initDateTime() const { return _init_date_time; }
      std::pair<std::string, std::string> dateAndTime() const { return utils::formatAnyDateTime(_init_date_time + _elapsed_time, _datetime_format->first, _datetime_format->second, true); }
      const tree::MTree<KBody>& bodies() const { return _bodies; }

      /* *********************************************** Operations (END) ******************************************************* */


    protected:
      /* ********************************************** Data Members ************************************************************ */
      /**
        * \brief  Pointer to the Space singleton
        */
      static Space *_space_ptr;

      /**
        * \brief  Observers (aka Cameras).
        */
      std::vector<physics::Observer*> _observers;

      /**
        * \brief  Active observer in the observers list
        */
      size_t _active_obs{ 0 };

      /**
        * \brief  Bodies in the space: a hierarchical tree of bodies
        */
      tree::MTree<KBody> _bodies;
       
      /**
        *  \brief Space date/time format, determined by the properties DATE_FORMAT and TIME_FORMAT (see Space())
        */
      std::unique_ptr<const std::pair<std::string, std::string>> _datetime_format {nullptr};

      /**
        *  \brief Time interval for updating the space state, determined by the property TICK (see Space())
        */
      units::TIME_T _tick;

      /**
        *  \brief Space initial date & time, determined by the property INIT_DATE_TIME (see Space())
        */
      std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> _init_date_time;

      /**
        *  \brief Absolute elapsed time in seconds
        */
      units::TIME_T _elapsed_time;


      /* ********************************************** Data Members (END) ****************************************************** */


      /* *********************************************** Operations ************************************************************* */
      /**
        *  \brief Constructor of the singleton
        *         Reads config file and initializes the space
        *                          Configuration file is a properties (key=value) file. Following properties are supported:
        *                            DATE_FORMAT       --> date format
        *                            TIME_FORMAT       --> time format
        *                            INIT_DATE_TIME    --> initial simulation date/time
        *                            TICK              --> initial simulation tick time, in seconds
        *                            OBSERVER_X        --> initial observer's position (in m) in the initial ecliptic CS
        *                            OBSERVER_Y        --> initial observer's position (in m) in the initial ecliptic CS
        *                            OBSERVER_Z        --> initial observer's position (in m) in the initial ecliptic CS
        *                            DB.HOST           
        *                            DB.PORT          
        *                            DB.USER
        *                            DB.PWD
        *                            DB.SCHEMA         --> DB Connection params
        *                            LOG_INTERVAL      --> interval of simulation time used to generate simulation statistical information (in simulation seconds)
        *         Initializes the Observer
        *         Initializes the Bodies
        *         The barycenter of the system is calculated once all the bodies are loaded
        *  @throw  string exception containing the description of the issue
        */
      Space();

      /**
        * \brief  Destructor of the singleton
        */
      ~Space();
      /* *********************************************** Operations (END) ******************************************************* */


    private:

      /* *********************************************** Operations ************************************************************* */
      /**
        *  \brief  Create bodies from the data stored in the DB. 
        *          Only active body typesa nad active bodies are creted. The initial state is taken from the ephemeris which match the space initial date time and are not part of a simulation
        *  @param  Properties of the space, containing the DB connection parameters.
        */
      void createBodies(const utils::PropertiesFileReader& properties);

      /* *********************************************** Operations (END) ******************************************************* */
  };
}

#endif // SPACE_H

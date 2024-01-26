#ifndef SPACE_SIMULATOR_WND_H
#define SPACE_SIMULATOR_WND_H

#include <chrono>
#include <string>

#include <glwnd/core/w_main_window_fs.h>
#include <glwnd/w_label.h>
#include <glwnd/w_state_button.h>
#include <glwnd/w_icon_button.h>
#include <glwnd/w_list.h>
#include <glwnd/w_state_button_group.h>

//////
#include <physics/space.h>
//////#include <physics/units.h>

//////#include <graphics/panel.h>
//////#include <graphics/cartesian_axes.h>
//////#include <graphics/body_renderer.h>
//////
//////#include <space_sim_tracker.h>
//////
//////

/**
  * \brief  Main application window (singleton class)
  *         Creates the main window and runs the space simulator singleton.
  */
class SpaceSimulatorWnd : public glwnd::WMainWindowFS
{
public:

  /**
    *  \brief Constructor of the main window singleton.
    *
    *         # CONFIGURATION
    *         Configuration parameters are read from the file config/space_simulator_wnd.cfg`
    *
    *         ### Configuration parameters
    *         Supported and mandatory parameters are:
    *           - FRAME_BUFFER_SAMPLES: Number of frame buffer samples for antialiasing
    *           - SWAP_INTERVAL: Number of screen refreshes to complete before next framebuffer swap
    *           - REAL_DATE_FORMAT: Format of the real date
    *           - REAL_TIME_FORMAT: Format of the real time
    *           - STATISTICS_INTERVAL: Time Interval for statistics update (in milliseconds)
    *
    *         # Creates the space singleton
    *
    *  @throw std::string   Description of the issue. Typical issues are:
    */
  SpaceSimulatorWnd(const std::pair<std::string, std::string>& real_datetime_format, std::chrono::milliseconds&& info_upd_interval);

  /**
    *  \brief Destructor of the main window.
    */
  virtual ~SpaceSimulatorWnd();


protected:
  /**
    *  \brief  Implementation of the application logic
    */
  virtual void eventLoopIteration() override;
//////
//////  /**
//////    *  \brief Draws the content of the main window
//////    *
//////    *  @return void
//////    */
//////  virtual void draw() override;
//////
//////

  // Implement virtual method t draw the main window
  virtual void drawMainWindow() override;

  /**
    *  \brief Key Pressed events callback
    */
  virtual void keyPressed(int key, int scancode, int modifiers) override;

  virtual void buttonClicked(glwnd::MouseButton button, int modifiers, const glboost::Position2D& pos) override {};

  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};

  virtual void buttonReleased(glwnd::MouseButton button, int modifiers, const glboost::Position2D& pos) override {};

  virtual void buttonDoubleClicked(const glboost::Position2D& pos) override {};

  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};

  virtual void dragged(glwnd::MouseButton button, const glboost::Position2D& pos) override {};

  virtual void mouseMoved(const glboost::Position2D& pos) override {};

//////
//////  /**
//////    *  \brief Mouse events callback
//////    */
//////  virtual void mouseButtonCallback(int button, int action, int mods) override;
//////
//////  /**
//////      *  \brief Mouse motion callback
//////      */
//////  virtual void mousePositionCallback(double xpos, double ypos) override;
//////
//////  /**
//////      *  \brief Mouse scroll callback
//////      */
//////  virtual void mouseScrollCallback(bool up) override;
//////
private:
  /* ******************************************** Data Members ************************************************************ */
//////
//////  /**
//////    *  \brief Pointer to the simulation tracker
//////    */
//////  std::unique_ptr<SpaceSimTracker> tracker{ nullptr };
//////
//////
  std::unique_ptr<glwnd::WLabel>          _lbl_fps{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_tps{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_fps_value{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_tps_value{ nullptr };
  
  std::unique_ptr<glwnd::WLabel>          _lbl_date{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_time{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_elapsed{ nullptr };
  
  std::unique_ptr<glwnd::WLabel>          _lbl_sim_date{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_sim_time{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_sim_elapsed{ nullptr };
  
  std::unique_ptr<glboost::gRectangle>    _rect_lbl_tick{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_tick{ nullptr };
  std::unique_ptr<glwnd::WLabel>          _lbl_tick_value{ nullptr };

  std::unique_ptr<glwnd::WStateButton>    _btn_pause{ nullptr };
  std::unique_ptr<glwnd::WIconButton>     _btn_inc_tick{ nullptr };
  std::unique_ptr<glwnd::WIconButton>     _btn_dec_tick{ nullptr };
  
  std::unique_ptr<glwnd::WList>           _list_bodies{ nullptr };
  using FILTER_MASK = uint8_t;
  FILTER_MASK                             _list_filter_mask{ 0b00001111 };
  static const FILTER_MASK                _SHOW_PLANETS{ 0b00001000 };
  static const FILTER_MASK                _SHOW_DWARF_PLANETS{ 0b00000100 };
  static const FILTER_MASK                _SHOW_MINOR_BODIES{ 0b00000010 };
  static const FILTER_MASK                _SHOW_SATELLITES{ 0b00000001 };
  
  std::unique_ptr<glwnd::WStateButton>     _btn_sort_by_distance{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_sort_by_mass{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_sort_by_name{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_sort_by_id{ nullptr };

  std::unique_ptr<glwnd::WStateButtonGroup> _btn_grp_sort{ nullptr };

  std::unique_ptr<glwnd::WStateButton>     _btn_show_planets{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_show_dwarf_planets{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_show_minor_bodies{ nullptr };
  std::unique_ptr<glwnd::WStateButton>     _btn_show_satellites{ nullptr };

//////  std::unique_ptr<graphics::Label>          txt_obs_x{ nullptr };
//////  std::unique_ptr<graphics::Label>          txt_obs_y{ nullptr };
//////  std::unique_ptr<graphics::Label>          txt_obs_z{ nullptr };
//////  std::unique_ptr<graphics::Label>          txt_obs_center_x{ nullptr };
//////  std::unique_ptr<graphics::Label>          txt_obs_center_y{ nullptr };
//////  std::unique_ptr<graphics::Label>          txt_obs_center_z{ nullptr };
//////  std::unique_ptr<graphics::CartesianAxes>  obs_CS_axes{ nullptr };
//////  std::unique_ptr<graphics::Panel>          list{ nullptr };
//////  graphics::BodyRenderer*                   body_renderer{ nullptr };
//////
//////
  /**
    *  \brief  Reference to the Space singleton
    */
  physics::Space &_space;

  using REAL_TIME_UNIT = std::chrono::nanoseconds;
  const size_t _UNITS_PER_SEC{ size_t(std::chrono::duration_cast<REAL_TIME_UNIT>(std::chrono::seconds(1)).count()) };

  /**
    *  \brief  Real date/time format, determined by the properties REAL_DATE_FORMAT and REAL_TIME_FORMAT 
    */
  const std::pair<std::string, std::string> _REAL_DATETIME_FORMAT;

  /**
    *  \brief  Time interval for updating info on the screen (e.g fps), determined by the property ON_SCREEN_INFO_UPD_INTERVAL
    */
  const REAL_TIME_UNIT _INFO_UPD_INTERVAL;

  /**
    *  \brief  Real Timestamp in nanoseconds
    */
  std::chrono::time_point<std::chrono::high_resolution_clock, REAL_TIME_UNIT> _timestamp;

  /**
    *  \brief  Real date and time formatted strings
    */
  std::pair<std::string, std::string>  _real_date_time;

  /**
    *  \brief  Real Elapsed time since the beginning of the simulation, in nanoseconds
    */
  REAL_TIME_UNIT _total_elapsed_time{ 0 };

  /**
    *  \brief  Initialization of windows objects in the constructor 
    */
  void initializeLayout();

  /**
    *  \brief  Initialization of callback for windows objects in the constructor
    */
  void setListeners();

  /**
    *  \brief Pointer to the function to be called for running the simulation
    *         By default it will point to Space::is_running(). But if the simulation is paused it'll point to the local is_paused()
    */
  void  (SpaceSimulatorWnd::* runTick)(void) { &SpaceSimulatorWnd::isRunning };

  /**
    * \brief  Default allowed tick values
    *         These are the default allowed values for the tick time when using the methods incTick or decTick.
    */
  const std::array<int16_t, 13> _DEFAULT_TICKS{ 1, 2, 5, 10, 20, 30, 60, 120, 300, 600, 1200, 3600, 18000 };

  /**
    * \brief  Selected tick position in _DEFAULT_TICKS
    */
  uint16_t _def_ticks_offset{ 0 };

//////  /**
//////    *  \brief  Tracking interval, in seconds of simulation time. This value is configured in "space_sim_tracker.cfg"
//////    */
//////  physics::units::TIME_T _tracking_interval{ 0 };
//////
//////  /* ******************************************** Data Members (END) ****************************************************** */
//////
//////
//////  /* *********************************************** Operations *********************************************************** */
//////
//////  /**
//////    *  \brief Updates the statistics. Called from the Event Loop
//////    *
//////    *  @return bool  True if the statistics have been updated
//////    */
//////  bool updateStats();
//////
  /**
    *  \brief Switches the state of the simulation to paused or un-paused
    *
    *  @return void
    */
  void pause();

  /**
    *  \brief Function to be called when the simulation is paused
    *         This function has the same signature as Space::is_running() so it can be pointed by
    *         the local runTick function pointer
    *
    *  @return void
    */
  void isPaused() { } // DO NOTHING

  /**
    *  \brief Function to be called when the simulation is running
    *         This function has the same signature as Space::is_paused() so it can be pointed by
    *         the local runTick function pointer
    *
    *  @return void
    */
  void isRunning();

  /**
    *  \brief Increase Tick time to the next predefined value
    *  @return  void
    */
  void incTick();

  /**
    *  \brief Decrease Tick time to the next predefined value
    *  @return  void
    */
  void decTick();

  // Add bodies to the list sorted by distance
  void addBodiesByDistance();

  // Add bodies to the list sorted by distance
  void updateBodiesByDistance();

  // Add bodies to the list sorted by mass
  void updateBodiesByMass();

  // Add bodies to the list sorted by name
  void updateBodiesByName();

  // Add bodies to the list sorted by id
  void updateBodiesById();

  // Applies / removes a filter to the list of bodies
  void filterBodies(FILTER_MASK filter, bool add);

  /* *********************************************** Operations (END) ***************************************************** */
};


#endif // SPACE_SIMULATOR_WND_H


#include <space_simulator_wnd.h>
#include <space_simulator_wnd.defs.h>

#include <chrono>

#include <logger.h>
#include <misc/formatDateTime.h>


using namespace glboost;
using namespace glwnd;
using namespace std::chrono;


/* ************************************************* PUBLIC ************************************************************* */

/// CONSTRUCTOR
SpaceSimulatorWnd::SpaceSimulatorWnd(const std::pair<std::string, std::string>& real_datetime_format, std::chrono::milliseconds&& info_upd_interval)
  : WMainWindowFS{}, _REAL_DATETIME_FORMAT{ real_datetime_format }, _INFO_UPD_INTERVAL{ duration_cast<REAL_TIME_UNIT>(info_upd_interval) }, _timestamp{ high_resolution_clock::now() },
  _real_date_time{ utils::formatDateTime(time_point_cast<seconds>(system_clock::now()), _REAL_DATETIME_FORMAT.first, _REAL_DATETIME_FORMAT.second) }, _space{ physics::Space::create() } {
    
  Font::addFontsDir(FONTS_DIR);
    
  initializeLayout();

  setListeners();
  
  
//////
//////  // Initialize tracking
//////  tracker.reset(new SpaceSimTracker(std::chrono::duration_cast<physics::units::TIME_T>(initRealTime.time_since_epoch()), _space) );
//////  _tracking_interval = physics::units::TIME_T(properties.propertyCast<int>("TRACKING_INTERVAL")[0]);
//////
//////  auto time_stamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
//////  DEBUG_LOG("Before Initial Tracking: " + std::to_string(time_stamp.count()));
//////  tracker->track(0s, _avg_fps);
//////  
//////  DEBUG_LOG("Initial Tracking: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time_stamp.count()));
//////
//////
//////  // Bodies
//////  body_renderer = &graphics::BodyRenderer::create(_space.activeObs(), _space.bodies());
//////  add(*body_renderer);
//////
//////
//////
//////  // Observer position and orientation
//////  const gltext::GT_FontSize    txt_obs_size{ FONT_MEDIUM };
//////  const gltext::GT_PositionPxl txt_obs_X{ static_cast<gltext::GT_PositionPxl>(TO_H_R(__DEF_HOR_RESOLUTION__ / 2 - 250)) };
//////  const gltext::GT_PositionPxl txt_obs_Y{ txt_elapsed_Y };
//////
//////  txt_obs_x.reset(new graphics::Label("X :  1.000 - 0.000 - 0.000", TO_H_R(__DEF_HOR_RESOLUTION__ / 2), txt_obs_Y + 7 * txt_obs_size));
//////  txt_obs_x->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_x->color(gltext::GT_Color(100, 20, 20));
//////  add(*txt_obs_x);
//////  txt_obs_y.reset(new graphics::Label("Y :  0.000 - 1.000 - 0.000 ", TO_H_R(__DEF_HOR_RESOLUTION__ / 2), txt_obs_Y + 4 * txt_obs_size));
//////  txt_obs_y->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_y->color(gltext::GT_Color(20, 100, 20));
//////  add(*txt_obs_y);
//////  txt_obs_z.reset(new graphics::Label("Z :  0.000 - 0.000 - 1.000 ", TO_H_R(__DEF_HOR_RESOLUTION__ / 2), txt_obs_Y + txt_obs_size));
//////  txt_obs_z->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_z->color(gltext::GT_Color(70, 70, 100));
//////  add(*txt_obs_z);
//////  txt_obs_center_x.reset(new graphics::Label("0.000", txt_obs_X, txt_obs_Y + 7 * txt_obs_size));
//////  txt_obs_center_x->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_center_x->color(gltext::GT_Color(100, 20, 20));
//////  add(*txt_obs_center_x);
//////  txt_obs_center_y.reset(new graphics::Label("0.000", txt_obs_X, txt_obs_Y + 4 * txt_obs_size));
//////  txt_obs_center_y->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_center_y->color(gltext::GT_Color(20, 100, 20));
//////  add(*txt_obs_center_y);
//////  txt_obs_center_z.reset(new graphics::Label("0.000", txt_obs_X, txt_obs_Y + txt_obs_size));
//////  txt_obs_center_z->font("Orbitron Medium", txt_obs_size);
//////  txt_obs_center_z->color(gltext::GT_Color(70, 70, 100));
//////  add(*txt_obs_center_z);
//////
//////  // Observer Graphical CS Axes
//////  obs_CS_axes.reset(new graphics::CartesianAxes(_space.activeObs().obsCS(), 
//////    graphics::Position2D(TO_H_R(__DEF_HOR_RESOLUTION__ / 2) - TO_V_R(80), txt_obs_Y + gltext::GT_PositionPxl(4.5 * txt_obs_size)), TO_V_R(60)));
//////  add(*obs_CS_axes);
//////
//////  // Panel
//////  list.reset(new graphics::Panel(3300, 200, 500, 800));
//////  add(*list);
//////  _wnd_regions.addMouseEventObj(*list);
//////

  DebugLog("SpaceSimulatorWnd: CREATED");
}


/// DESTRUCTOR 
SpaceSimulatorWnd::~SpaceSimulatorWnd() {
  DebugLog("SpaceSimulatorWnd: DESTROYED");

////  body_renderer->destroy();

  _space.destroy();
}

/* ************************************************* PUBLIC (END) ******************************************************* */


/* ************************************************* PROTECTED ********************************************************** */

void SpaceSimulatorWnd::eventLoopIteration() {
  // Run internal loop until the info update interval time is reached
  REAL_TIME_UNIT elapsed_time{ 0 };
  REAL_TIME_UNIT info_upd_timer{ 0 };
  size_t total_ticks{ 0 }; 
  do {
    //**********************************************************//
    //**** EXECUTE SPACE TICK, unless simulation is paused *****//
    //**********************************************************//
    (this->*runTick)();

    // Calculate elapsed_time and info_upd_timer in the tick iteration 
    elapsed_time = high_resolution_clock::now() - _timestamp;
    info_upd_timer += elapsed_time;
    _timestamp += elapsed_time;

    ++total_ticks;
  } while (info_upd_timer <= _INFO_UPD_INTERVAL);

  _total_elapsed_time += info_upd_timer;
  
  size_t avg_fps{ size_t(_UNITS_PER_SEC / info_upd_timer.count() + 0.5) };
  size_t avg_tps{ size_t(_UNITS_PER_SEC * total_ticks / info_upd_timer.count()) };

  // Update real datetime
  _real_date_time = utils::formatDateTime(time_point_cast<seconds>(system_clock::now()), _REAL_DATETIME_FORMAT.first, _REAL_DATETIME_FORMAT.second);

  DebugLog("FPS " + std::to_string(avg_fps) + " -- TPS " + std::to_string(avg_tps));
  
  // Update labels on the screen
  _lbl_fps_value->text(std::to_string(avg_fps));
  _lbl_tps_value->text(std::to_string(avg_tps));

  _lbl_date->text(_real_date_time.first);
  _lbl_time->text(_real_date_time.second);
  _lbl_elapsed->text( std::to_string( duration_cast<seconds>(_total_elapsed_time).count() / 3600 ) + " h " +
                      std::to_string( (duration_cast<seconds>(_total_elapsed_time).count() % 3600) / 60 ) + " m " +
                      std::to_string( duration_cast<seconds>(_total_elapsed_time).count() % 60 ) + " s" );

  auto sim_date_time = _space.dateAndTime();
  _lbl_sim_date->text(sim_date_time.first);
  _lbl_sim_time->text(sim_date_time.second.substr(0,5));
  _lbl_sim_elapsed->text(std::to_string(_space.elapsedTime().count() / 31557600) + " y " +
                         std::to_string( (_space.elapsedTime().count() % 31557600) / 86400 ) + " d " +
                         std::to_string( (_space.elapsedTime().count() % 86400) / 3600 ) + " h " +
                         std::to_string( (_space.elapsedTime().count() % 3600) / 60 ) + " m");

   _lbl_tick_value->text(std::to_string(_space.tick().count()) + " s");
}

void SpaceSimulatorWnd::drawMainWindow() {
  _rect_lbl_tick->draw();
}
//////void SpaceSimulatorWnd::draw() {  
//////
//////  // Execute space tick unless simulation is paused
//////  (this->*runTick)();
//////    
//////  if ( updateStats() ) {

//////
//////    std::stringstream txt;
//////    auto obsCS = _space.activeObs().obsCS();
//////    txt << std::setprecision(3) << std::fixed;
//////    txt << "Ux :  " << obsCS[0][0] << " , " << obsCS[0][1] << " , " << obsCS[0][2];
//////    txt_obs_x->text(txt.str());
//////    txt.str(std::string());
//////    txt << "Uy :  " << obsCS[1][0] << " , " << obsCS[1][1] << " , " << obsCS[1][2];
//////    txt_obs_y->text(txt.str());
//////    txt.str(std::string());
//////    txt << "Uz :  " << obsCS[2][0] << " , " << obsCS[2][1] << " , " << obsCS[2][2];
//////    txt_obs_z->text(txt.str());
//////    txt.str(std::string());
//////    txt << obsCS.center()[0] / 1e9;
//////    txt_obs_center_x->text(txt.str());
//////    txt.str(std::string());
//////    txt << obsCS.center()[1] / 1e9;
//////    txt_obs_center_y->text(txt.str());
//////    txt.str(std::string());
//////    txt << obsCS.center()[2] / 1e9;
//////    txt_obs_center_z->text(txt.str());
//////
//////    graphics::MainWindowFS::draw();
//////  }
//////
//////}
/* ************************************************* PROTECTED (END) **************************************************** */

/* ************************************************* PRIVATE ************************************************************ */
void SpaceSimulatorWnd::pause() {
  if ( runTick == &SpaceSimulatorWnd::isPaused )
    runTick = &SpaceSimulatorWnd::isRunning;
  else
    runTick = &SpaceSimulatorWnd::isPaused;
}


void SpaceSimulatorWnd::isRunning() {
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!                                               !!!
  // !!!   MOST IMPORTANT FUNCTION: RUN A SPACE TICK   !!!
  // !!!                                               !!!
                      _space.runTick();
  // !!!                                               !!!
  // !!!                                               !!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


///  ////////// Store tracking information after the defined tracking interval of simulation time
///  ////////static decltype(_tracking_interval) track_timer{ 0 };
///  ////////track_timer += _space.tick();
///  ////////if (track_timer >= _tracking_interval) {
///  ////////  track_timer = 0s;
///  ////////  tracker->track(std::chrono::duration_cast<physics::units::TIME_T>(TIMESTAMP_T(_timestamp)), _avg_fps);
///  ////////}
}


void SpaceSimulatorWnd::incTick() {
  if (_def_ticks_offset < _DEFAULT_TICKS.size() - 1 ) {
    // Increase to the next default value
    _def_ticks_offset++;
    _space.tick(static_cast<physics::units::TIME_T>(_DEFAULT_TICKS.at(_def_ticks_offset)));
  }
}


void SpaceSimulatorWnd::decTick() {
  if (_def_ticks_offset > 0 ) {
    // Decrease to the previous default value
    _def_ticks_offset--;
    _space.tick(static_cast<physics::units::TIME_T>(_DEFAULT_TICKS.at(_def_ticks_offset)));
  }
}


/* ************************************************* PRIVATE (END) ****************************************************** */

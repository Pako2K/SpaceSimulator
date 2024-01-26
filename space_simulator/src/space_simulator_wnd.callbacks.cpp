#include "space_simulator_wnd.h"

#include <logger.h>

/* ************************************************* PRIVATE ************************************************************ */

void SpaceSimulatorWnd::keyPressed(int key, int scancode, int modifiers) {
  // For international keyboard layout compatibility (only printable characters)
  static const char empty_char{ '\0' };
  const char *keyname{ nullptr };
  if (key < 256 && key > 0) keyname = glfwGetKeyName(key, scancode);
  if (!keyname) keyname = &empty_char;

  DebugLog("Key Name: " << *keyname);

  // Close Application
  if (key == GLFW_KEY_ESCAPE) {
    close();
    return;
  }
  // Pause / un-pause Application
  if (*keyname == 'P') {
    _btn_pause->switchState();
    return;
  }
  // Increase / Decrease tick time
  if (*keyname == 'T')  {
    if (modifiers == 0)
      decTick();
    else if (modifiers == GLFW_MOD_SHIFT)
      incTick();
    return;
  }
}

//////
//////#define KEY_PRESSED(key_code) (key == key_code && (action == GLFW_PRESS || action == GLFW_REPEAT))
//////#define KEY_CHR_PRESSED(chr) (*keyname == chr && (action == GLFW_PRESS || action == GLFW_REPEAT))
//////
//////  static const physics::units::LENGTH_T INIT_TRANS = 1e9;  // Percentage of the distance to the referenced point
//////  static const physics::units::LENGTH_T INIT_ACC_FACTOR = 1.04; // Increase percentage per repetition
//////
//////
//////  // Decrease tick time
//////  else if (KEY_CHR_PRESSED('T') && modifiers == 0) {
//////    DEBUG_LOG("Tick before: " + std::to_string(_space.tick().count()));
//////    decTick();
//////    DEBUG_LOG("Tick after: " + std::to_string(_space.tick().count()));
//////  }
//////  // Increase tick time
//////  else if (KEY_CHR_PRESSED('T') && modifiers == GLFW_MOD_SHIFT) {
//////    DEBUG_LOG("Tick before: " + std::to_string(_space.tick().count()));
//////    incTick();
//////    DEBUG_LOG("Tick after: " + std::to_string(_space.tick().count()));
//////  }
//////  // Pause or un-pause simulation
//////  else if (KEY_CHR_PRESSED('P')) {
//////    pause();
//////    DEBUG_LOG("Simulation (un)paused");
//////  }
//////  // Move observer to the X axis
//////  else if (KEY_CHR_PRESSED('X')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::X);
//////    else if (modifiers == GLFW_MOD_SHIFT)
//////      _space.activeObs().reposition(geometry::CoordAxis::X, true);
//////  }
//////  // Move observer to the Y axis
//////  else if (KEY_CHR_PRESSED('Y')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::Y);
//////    else if (modifiers == GLFW_MOD_SHIFT)
//////      _space.activeObs().reposition(geometry::CoordAxis::Y, true);
//////  }
//////  // Move observer to the Z axis
//////  else if (KEY_CHR_PRESSED('Z')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::Z);
//////    else if (modifiers == GLFW_MOD_SHIFT) 
//////      _space.activeObs().reposition(geometry::CoordAxis::Z, true);
//////    // Re-align observer's axes with the WCS
//////    else if (modifiers == GLFW_MOD_CONTROL) 
//////      _space.activeObs().realign();
//////  }
//////  // Move observer along one of its axes
//////  else if (*keyname == '+' || *keyname == '-' ||
//////          key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT || 
//////          key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
//////    
//////    static physics::units::LENGTH_T init_accel { INIT_TRANS };
//////    
//////    
//////    geometry::CoordAxis axis; 
//////    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT)
//////      axis = geometry::CoordAxis::X;
//////    else if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN)
//////      axis = geometry::CoordAxis::Y;
//////    else
//////      axis = geometry::CoordAxis::Z;
//////
//////    if (action == GLFW_PRESS) {
//////      if (key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN || *keyname == '-')
//////        init_accel = -init_accel;
////////      _space.activeObs().addTranslation(axis, init_accel / 60);
//////    }
//////    else if (action == GLFW_REPEAT) {
////////                _space.activeObs().addTranslation(axis, init_accel / 60);
//////                init_accel *= INIT_ACC_FACTOR;
//////                //acc_factor *= 1.1;
//////      //_space.activeObs().addTranslation(axis, ACC_FACTOR * _space.activeObs().acceleration());
//////    }
//////    else if (action == GLFW_RELEASE){// && _space.activeObs().velocity() != 0) {
//////      init_accel = INIT_TRANS;
//////      /*if (init_accel > 0)
//////        init_accel = - max(2 * init_accel * _avg_fps, _space.activeObs().velocity());
//////      else if (init_accel < 0)
//////        init_accel = - min(2 * init_accel * _avg_fps, _space.activeObs().velocity());
//////
//////      DEBUG_LOG("Stop Translation. " + std::to_string(init_accel / _avg_fps));
//////      _space.activeObs().addTranslation(axis, init_accel / _avg_fps);*/
//////    }
//////  }
////// 
//////  // LOCK OBSERVER WITH A BODY
//////  else if (*keyname == '0' && action == GLFW_PRESS) {
//////    _space.activeObs().uncoupleToBody();
//////  }
//////  else if ((*keyname == '1' || *keyname == '2' || *keyname == '3' || *keyname == '4') && action == GLFW_PRESS) {
//////    auto pos = atol(keyname);
//////    auto iter = _space.bodies().begin();
//////    
//////    while (--pos > 0)
//////      iter.next();
//////    auto& body = iter.next();
//////
//////    _space.activeObs().coupleToBody(body);
//////
//////    DEBUG_LOG("Observer coupled to: " + body.name());
//////  }
//////
//////  #undef KEY_PRESSED
//////}
//////
//////
///////* mouse_button_callback */
//////void SpaceSimulatorWnd::mouseButtonCallback(int button, int action, int modifiers) {
//////
//////  DEBUG_LOG("Mouse event : button = " + std::to_string(button) + "  Action = " + std::to_string(action) + "  Modifiers = " + std::to_string(modifiers));
//////  
//////  auto mouse_pos = mousePosition();
//////  DEBUG_LOG("Mouse position : " + std::to_string(mouse_pos.x()) + ", " + std::to_string(mouse_pos.y()));
//////
//////  // Deliver event to controls
//////  auto control = _wnd_regions.getMouseEventObj(mouse_pos);
//////
//////  if (control && action == GLFW_PRESS) {
//////    control_with_focus = control; 
//////    control->onClick(button, mouse_pos);
//////  }
//////  if (control_with_focus && action == GLFW_RELEASE) {
//////    control_with_focus = nullptr;
//////  }
//////}
//////
//////
//////void SpaceSimulatorWnd::mousePositionCallback(double xpos, double ypos) {
//////  if (control_with_focus) {
//////    control_with_focus->onMove(graphics::Position2D(xpos, ypos));
//////    return;
//////  }
//////
//////  // Initial value: means no previous position
//////  static double previousPos_X{ -1.0 };
//////  static double previousPos_Y{ -1.0 };
//////  static double previousPos_X_right{ -1.0 };
//////  static double previousPos_Y_right{ -1.0 };
//////  int stateLeft = mouseButtonState(GLFW_MOUSE_BUTTON_LEFT);
//////  int stateRight = mouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);
//////  int stateMiddle = mouseButtonState(GLFW_MOUSE_BUTTON_MIDDLE);
//////
//////  DEBUG_LOG("Mouse motion : state Left = " + std::to_string(stateLeft) + ", state Right = " + std::to_string(stateRight) +" xpos = " + std::to_string(xpos) + "  ypos = " + std::to_string(ypos));
//////
//////  // ROTATE OBSERVER AROUND WCS CENTER
//////  if (stateLeft == GLFW_PRESS) {
//////    if (previousPos_X == -1.0) {
//////      previousPos_X = xpos;
//////      previousPos_Y = ypos;
//////    }
//////    else {
//////      double diff_X = xpos - previousPos_X;
//////      double diff_Y = ypos - previousPos_Y;
//////
//////      // The full width represents 1/2 full horizontal rotations
//////      if (abs(diff_X) > abs(diff_Y)) {
//////        DEBUG_LOG("Horizontal rotation, in degrees: " + std::to_string(360.0 * diff_X / screenWidth()));
//////        _space.activeObs().rotateWCS(geometry::CoordAxis::Y, -_PI_ * diff_X / screenWidth());
//////      }
//////      else {
//////        DEBUG_LOG("Vertical rotation, in degrees: " + std::to_string(360.0 * diff_Y / screenWidth()));
//////        _space.activeObs().rotateWCS(geometry::CoordAxis::X, -_PI_ * diff_Y / screenWidth());
//////      }
//////
//////      // Store new positions
//////      previousPos_X = xpos;
//////      previousPos_Y = ypos;
//////    }
//////  }
//////  else {
//////    // Set to initial value (no previous motion)
//////    previousPos_X = -1.0;
//////    previousPos_Y = -1.0;
//////  }
//////
//////  // ROTATE OBSERVER AROUND ITSELF
//////  if (stateRight == GLFW_PRESS) {
//////    if (previousPos_X_right == -1.0) {
//////      previousPos_X_right = xpos;
//////      previousPos_Y_right = ypos;
//////    }
//////    else {
//////      int state_control_key = keyState(GLFW_KEY_LEFT_CONTROL);
//////
//////      double diff_X = xpos - previousPos_X_right;
//////      double diff_Y = ypos - previousPos_Y_right;
//////
//////      // The full width represents 1/4 full horizontal rotations
//////      if (state_control_key == GLFW_PRESS){
//////        // Rotate arounnd z axis
//////        _space.activeObs().rotate(geometry::CoordAxis::Z, _HALF_PI_ * ( diff_X *(ypos < screenHeight()/2 ? 1 : (-1)) + diff_Y * (xpos > screenWidth() / 2 ? 1 : (-1)) ) / screenWidth());
//////      }
//////      else {
//////        _space.activeObs().rotate(geometry::CoordAxis::Y, _HALF_PI_ * diff_X / screenWidth());
//////        _space.activeObs().rotate(geometry::CoordAxis::X, _HALF_PI_ * diff_Y / screenWidth());
//////      }
//////      
//////      // Store new positions
//////      previousPos_X_right = xpos;
//////      previousPos_Y_right = ypos;
//////    }
//////  }
//////  else {
//////    previousPos_X_right = -1.0;
//////    previousPos_Y_right = -1.0;
//////  }
//////  
//////}
//////
//////
//////void SpaceSimulatorWnd::mouseScrollCallback(bool up) {
//////  auto mouse_pos = mousePosition();
//////
//////  // Deliver event to controls
//////  auto control = _wnd_regions.getMouseEventObj(mouse_pos);
//////
//////  if (control) {
//////    control->onScroll(up, mouse_pos);
//////    return;
//////  }
//////  
//////
//////  // Zoom in / zoom out
//////  static const float TRANS_PERCENTAGE{ 0.05f };
//////  DEBUG_LOG("Mouse scroll :  up = " + std::to_string(up));
//////  if (up) {
//////    _space.activeObs().translate(geometry::CoordAxis::Z, -TRANS_PERCENTAGE);
//////  }
//////  else {
//////    _space.activeObs().translate(geometry::CoordAxis::Z, TRANS_PERCENTAGE);
//////  }
//////  
//////}
///////* ************************************************* PRIVATE (END) ****************************************************** */
//////




//////#include "space_simulator_wnd.h"
//////
//////#include <logger.h>
//////
///////* ************************************************* PRIVATE ************************************************************ */
//////
///////* glfw_error_callback(int error, const char* description) */
//////void SpaceSimulatorWnd::errorCallback(int error, const char* description){
//////  ERROR_LOG( "GLFW # " + std::to_string(error) + " # " + std::string(description) );
//////}
//////
//////
///////* key_callback(GLFWwindow * window, int key, int scancode, int action, int modifiers) */
//////void SpaceSimulatorWnd::keyCallback(int key, int scancode, int action, int modifiers) {
//////
//////  DEBUG_LOG("Pressed Key, Scancode, Action, Modifiers: " + std::to_string(key) + " " +
//////    std::to_string(scancode) + " " + std::to_string(action) + " " + std::to_string(modifiers));
//////
//////  // Just to improve performance
//////  //if (action == GLFW_RELEASE) return;
//////
//////  // For international keyboard layout compatibility (only printable characters)
//////  static const char empty_char{ '\0' };
//////  const char *keyname{ nullptr };
//////  if (key < 256 && key > 0) keyname = glfwGetKeyName(key, scancode);
//////  if (!keyname) keyname = &empty_char;
//////
//////  DEBUG_LOG("Key Name: " + std::to_string(*keyname));
//////
//////#define KEY_PRESSED(key_code) (key == key_code && (action == GLFW_PRESS || action == GLFW_REPEAT))
//////#define KEY_CHR_PRESSED(chr) (*keyname == chr && (action == GLFW_PRESS || action == GLFW_REPEAT))
//////
//////  static const physics::units::LENGTH_T INIT_TRANS = 1e9;  // Percentage of the distance to the referenced point
//////  static const physics::units::LENGTH_T INIT_ACC_FACTOR = 1.04; // Increase percentage per repetition
//////
//////
//////  // Close Application
//////  if (key == GLFW_KEY_ESCAPE) {
//////    close();
//////  }
//////  // Decrease tick time
//////  else if (KEY_CHR_PRESSED('T') && modifiers == 0) {
//////    DEBUG_LOG("Tick before: " + std::to_string(_space.tick().count()));
//////    decTick();
//////    DEBUG_LOG("Tick after: " + std::to_string(_space.tick().count()));
//////  }
//////  // Increase tick time
//////  else if (KEY_CHR_PRESSED('T') && modifiers == GLFW_MOD_SHIFT) {
//////    DEBUG_LOG("Tick before: " + std::to_string(_space.tick().count()));
//////    incTick();
//////    DEBUG_LOG("Tick after: " + std::to_string(_space.tick().count()));
//////  }
//////  // Pause or un-pause simulation
//////  else if (KEY_CHR_PRESSED('P')) {
//////    pause();
//////    DEBUG_LOG("Simulation (un)paused");
//////  }
//////  // Move observer to the X axis
//////  else if (KEY_CHR_PRESSED('X')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::X);
//////    else if (modifiers == GLFW_MOD_SHIFT)
//////      _space.activeObs().reposition(geometry::CoordAxis::X, true);
//////  }
//////  // Move observer to the Y axis
//////  else if (KEY_CHR_PRESSED('Y')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::Y);
//////    else if (modifiers == GLFW_MOD_SHIFT)
//////      _space.activeObs().reposition(geometry::CoordAxis::Y, true);
//////  }
//////  // Move observer to the Z axis
//////  else if (KEY_CHR_PRESSED('Z')) {
//////    if (modifiers == 0)
//////      _space.activeObs().reposition(geometry::CoordAxis::Z);
//////    else if (modifiers == GLFW_MOD_SHIFT) 
//////      _space.activeObs().reposition(geometry::CoordAxis::Z, true);
//////    // Re-align observer's axes with the WCS
//////    else if (modifiers == GLFW_MOD_CONTROL) 
//////      _space.activeObs().realign();
//////  }
//////  // Move observer along one of its axes
//////  else if (*keyname == '+' || *keyname == '-' ||
//////          key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT || 
//////          key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
//////    
//////    static physics::units::LENGTH_T init_accel { INIT_TRANS };
//////    
//////    
//////    geometry::CoordAxis axis; 
//////    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT)
//////      axis = geometry::CoordAxis::X;
//////    else if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN)
//////      axis = geometry::CoordAxis::Y;
//////    else
//////      axis = geometry::CoordAxis::Z;
//////
//////    if (action == GLFW_PRESS) {
//////      if (key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN || *keyname == '-')
//////        init_accel = -init_accel;
////////      _space.activeObs().addTranslation(axis, init_accel / 60);
//////    }
//////    else if (action == GLFW_REPEAT) {
////////                _space.activeObs().addTranslation(axis, init_accel / 60);
//////                init_accel *= INIT_ACC_FACTOR;
//////                //acc_factor *= 1.1;
//////      //_space.activeObs().addTranslation(axis, ACC_FACTOR * _space.activeObs().acceleration());
//////    }
//////    else if (action == GLFW_RELEASE){// && _space.activeObs().velocity() != 0) {
//////      init_accel = INIT_TRANS;
//////      /*if (init_accel > 0)
//////        init_accel = - max(2 * init_accel * _avg_fps, _space.activeObs().velocity());
//////      else if (init_accel < 0)
//////        init_accel = - min(2 * init_accel * _avg_fps, _space.activeObs().velocity());
//////
//////      DEBUG_LOG("Stop Translation. " + std::to_string(init_accel / _avg_fps));
//////      _space.activeObs().addTranslation(axis, init_accel / _avg_fps);*/
//////    }
//////  }
////// 
//////  // LOCK OBSERVER WITH A BODY
//////  else if (*keyname == '0' && action == GLFW_PRESS) {
//////    _space.activeObs().uncoupleToBody();
//////  }
//////  else if ((*keyname == '1' || *keyname == '2' || *keyname == '3' || *keyname == '4') && action == GLFW_PRESS) {
//////    auto pos = atol(keyname);
//////    auto iter = _space.bodies().begin();
//////    
//////    while (--pos > 0)
//////      iter.next();
//////    auto& body = iter.next();
//////
//////    _space.activeObs().coupleToBody(body);
//////
//////    DEBUG_LOG("Observer coupled to: " + body.name());
//////  }
//////
//////  #undef KEY_PRESSED
//////}
//////
//////
///////* mouse_button_callback */
//////void SpaceSimulatorWnd::mouseButtonCallback(int button, int action, int modifiers) {
//////
//////  DEBUG_LOG("Mouse event : button = " + std::to_string(button) + "  Action = " + std::to_string(action) + "  Modifiers = " + std::to_string(modifiers));
//////  
//////  auto mouse_pos = mousePosition();
//////  DEBUG_LOG("Mouse position : " + std::to_string(mouse_pos.x()) + ", " + std::to_string(mouse_pos.y()));
//////
//////  // Deliver event to controls
//////  auto control = _wnd_regions.getMouseEventObj(mouse_pos);
//////
//////  if (control && action == GLFW_PRESS) {
//////    control_with_focus = control; 
//////    control->onClick(button, mouse_pos);
//////  }
//////  if (control_with_focus && action == GLFW_RELEASE) {
//////    control_with_focus = nullptr;
//////  }
//////}
//////
//////
//////void SpaceSimulatorWnd::mousePositionCallback(double xpos, double ypos) {
//////  if (control_with_focus) {
//////    control_with_focus->onMove(graphics::Position2D(xpos, ypos));
//////    return;
//////  }
//////
//////  // Initial value: means no previous position
//////  static double previousPos_X{ -1.0 };
//////  static double previousPos_Y{ -1.0 };
//////  static double previousPos_X_right{ -1.0 };
//////  static double previousPos_Y_right{ -1.0 };
//////  int stateLeft = mouseButtonState(GLFW_MOUSE_BUTTON_LEFT);
//////  int stateRight = mouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);
//////  int stateMiddle = mouseButtonState(GLFW_MOUSE_BUTTON_MIDDLE);
//////
//////  DEBUG_LOG("Mouse motion : state Left = " + std::to_string(stateLeft) + ", state Right = " + std::to_string(stateRight) +" xpos = " + std::to_string(xpos) + "  ypos = " + std::to_string(ypos));
//////
//////  // ROTATE OBSERVER AROUND WCS CENTER
//////  if (stateLeft == GLFW_PRESS) {
//////    if (previousPos_X == -1.0) {
//////      previousPos_X = xpos;
//////      previousPos_Y = ypos;
//////    }
//////    else {
//////      double diff_X = xpos - previousPos_X;
//////      double diff_Y = ypos - previousPos_Y;
//////
//////      // The full width represents 1/2 full horizontal rotations
//////      if (abs(diff_X) > abs(diff_Y)) {
//////        DEBUG_LOG("Horizontal rotation, in degrees: " + std::to_string(360.0 * diff_X / screenWidth()));
//////        _space.activeObs().rotateWCS(geometry::CoordAxis::Y, -_PI_ * diff_X / screenWidth());
//////      }
//////      else {
//////        DEBUG_LOG("Vertical rotation, in degrees: " + std::to_string(360.0 * diff_Y / screenWidth()));
//////        _space.activeObs().rotateWCS(geometry::CoordAxis::X, -_PI_ * diff_Y / screenWidth());
//////      }
//////
//////      // Store new positions
//////      previousPos_X = xpos;
//////      previousPos_Y = ypos;
//////    }
//////  }
//////  else {
//////    // Set to initial value (no previous motion)
//////    previousPos_X = -1.0;
//////    previousPos_Y = -1.0;
//////  }
//////
//////  // ROTATE OBSERVER AROUND ITSELF
//////  if (stateRight == GLFW_PRESS) {
//////    if (previousPos_X_right == -1.0) {
//////      previousPos_X_right = xpos;
//////      previousPos_Y_right = ypos;
//////    }
//////    else {
//////      int state_control_key = keyState(GLFW_KEY_LEFT_CONTROL);
//////
//////      double diff_X = xpos - previousPos_X_right;
//////      double diff_Y = ypos - previousPos_Y_right;
//////
//////      // The full width represents 1/4 full horizontal rotations
//////      if (state_control_key == GLFW_PRESS){
//////        // Rotate arounnd z axis
//////        _space.activeObs().rotate(geometry::CoordAxis::Z, _HALF_PI_ * ( diff_X *(ypos < screenHeight()/2 ? 1 : (-1)) + diff_Y * (xpos > screenWidth() / 2 ? 1 : (-1)) ) / screenWidth());
//////      }
//////      else {
//////        _space.activeObs().rotate(geometry::CoordAxis::Y, _HALF_PI_ * diff_X / screenWidth());
//////        _space.activeObs().rotate(geometry::CoordAxis::X, _HALF_PI_ * diff_Y / screenWidth());
//////      }
//////      
//////      // Store new positions
//////      previousPos_X_right = xpos;
//////      previousPos_Y_right = ypos;
//////    }
//////  }
//////  else {
//////    previousPos_X_right = -1.0;
//////    previousPos_Y_right = -1.0;
//////  }
//////  
//////}
//////
//////
//////void SpaceSimulatorWnd::mouseScrollCallback(bool up) {
//////  auto mouse_pos = mousePosition();
//////
//////  // Deliver event to controls
//////  auto control = _wnd_regions.getMouseEventObj(mouse_pos);
//////
//////  if (control) {
//////    control->onScroll(up, mouse_pos);
//////    return;
//////  }
//////  
//////
//////  // Zoom in / zoom out
//////  static const float TRANS_PERCENTAGE{ 0.05f };
//////  DEBUG_LOG("Mouse scroll :  up = " + std::to_string(up));
//////  if (up) {
//////    _space.activeObs().translate(geometry::CoordAxis::Z, -TRANS_PERCENTAGE);
//////  }
//////  else {
//////    _space.activeObs().translate(geometry::CoordAxis::Z, TRANS_PERCENTAGE);
//////  }
//////  
//////}
///////* ************************************************* PRIVATE (END) ****************************************************** */
//////

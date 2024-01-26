#include <glwnd/core/w_main_window_fs.h>

#include <string>
#include <sstream>
#include <map>
#include <chrono>

#include <logger.h>


using namespace glwnd;
using namespace glboost;


const uint8_t  __DEF_SWAP_INTERVAL__{ 1 };
const Color4f  __DEF_BACKGROUND_COLOR__{ Color4f(1, 1, 1, 1) };


int             WMainWindowFS::_Open_GL_major{ 3 };
int             WMainWindowFS::_Open_GL_minor{ 2 };
int             WMainWindowFS::_Open_GL_profile{ GLFW_OPENGL_CORE_PROFILE };
int             WMainWindowFS::_samples{ 1 };
bool            WMainWindowFS::_enable_mouse_move{ true };
bool            WMainWindowFS::_enable_mouse_scroll{ true };
bool            WMainWindowFS::_enable_mouse_click{ true };
bool            WMainWindowFS::_enable_keyboard{ true };

WMainWindowFS*  WMainWindowFS::_main_window_fs{ nullptr };

Size2D          WMainWindowFS::_screen_size{ 0,0 };



void WMainWindowFS::initialize() {
  // Initialize GLFW for the whole application (all windows reuse the same GLFW)
  if (!glfwInit()) {
    // Initialization failed
    throw std::string("GLFW CANNOT BE INITIALIZED.");
  }
  DebugLog("Using GLFW Version: " + std::string((char *)glfwGetVersionString()));
  
  // Using OpenGL defined version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _Open_GL_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _Open_GL_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, _Open_GL_profile);

  // Set number of samples for antialiasing
  glfwWindowHint(GLFW_SAMPLES, _samples);


  // Define full screen window (with no border), and initially not visible
  glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  

  // Define GLFW Error callback
  glfwSetErrorCallback(glfw_error_callback);


  // Get primary monitor and its current resolution (video mode)
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (!monitor) {
    glfwTerminate();
    throw std::string("Could not get monitor.");
  }

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if (!mode) {
    glfwTerminate();
    throw std::string("Could not get Video mode.");
  }

  // Store the screen size
  _screen_size.w() = static_cast<SizePxl>(mode->width);
  _screen_size.h() = static_cast<SizePxl>(mode->height);

  DebugLog("Screen resolution: " << _screen_size.w() << " x " << _screen_size.h());
  DebugLog("STANDARD WINDOW INITIALIZATION FINISHED.");
}


void WMainWindowFS::terminate() {
  glfwTerminate();
  DebugLog("GLFW TERMINATED.\n");
}


void WMainWindowFS::contextVersion(uint8_t major, uint8_t minor) {
  // Check that there is no Main Window
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _Open_GL_major = static_cast<int>(major);
    _Open_GL_minor = static_cast<int>(minor);
    // Just in case it is called before initialize()
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _Open_GL_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _Open_GL_minor);
    return;
  }
  throw std::logic_error("Main Window already created.");
}


void WMainWindowFS::samples(uint8_t samples) {
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _samples = static_cast<int>(samples);
    // Just in case it is called before initialize()
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, _samples);
    return;
  }
  throw std::logic_error("Main Window already created.");
}


void WMainWindowFS::disableMouseMove() {
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _enable_mouse_move = false;
    return;
  }
  throw std::logic_error("Main Window already created.");
}


void  WMainWindowFS::disableMouseScroll() {
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _enable_mouse_scroll = false;
    return;
  }
  throw std::logic_error("Main Window already created.");
}


void  WMainWindowFS::disableMouseClick() {
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _enable_mouse_click = false;
    return;
  }
  throw std::logic_error("Main Window already created.");
}


void  WMainWindowFS::disableKeyboard() {
  try {
    WContainer::mainContainer();
  }
  catch (std::logic_error&) {
    _enable_keyboard = false;
    return;
  }
  throw std::logic_error("Main Window already created.");
}


WMainWindowFS::WMainWindowFS() : 
  WContainer{ Position2D(0,0), _screen_size, true }, _swap_interval{ __DEF_SWAP_INTERVAL__ }, _bkg_color{ __DEF_BACKGROUND_COLOR__ } {
  // Create the Full Screen Main Window
  if(_main_window_fs)
    throw std::logic_error("Main Window already created");
  _window = glfwCreateWindow(WObject::size().w(), WObject::size().h(), "", nullptr, nullptr);
  if (!_window) {
    ErrorLog("Could not create window. Main Window not initialized?");
    throw std::runtime_error("Could not create window. Main Window not initialized?");
  }

  // Make this window context the current one (for OpenGL)
  glfwMakeContextCurrent(_window);
  if (!glfwGetCurrentContext()) {
    glfwDestroyWindow(_window);
    throw std::runtime_error("Couldn't make Window context the current one.");
  }
  
  // Set keyboard and mouse callbacks
  if(_enable_keyboard)
    glfwSetKeyCallback(_window, key_callback);
  if(_enable_mouse_click)
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
  if(_enable_mouse_move)
    glfwSetCursorPosCallback(_window, mouse_position_callback);
  if (_enable_mouse_scroll)
    glfwSetScrollCallback(_window, mouse_scroll_callback);

  // Initialize GLEW
  glewExperimental = GL_TRUE;

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    glfwDestroyWindow(_window);
    throw std::runtime_error("Not possible to initialize GL: " + std::string((char *)glewGetErrorString(err)));
  }

  DebugLog("Status: Using GLEW Version: " + std::string((char *)glewGetString(GLEW_VERSION)));
  DebugLog("Status: Using GL Vendor: " + std::string((char *)glGetString(GL_VENDOR)));
  DebugLog("Status: Using GL Renderer: " + std::string((char *)glGetString(GL_RENDERER)));
  DebugLog("Status: Using GL Version: " + std::string((char *)glGetString(GL_VERSION)));
  DebugLog("Status: Using GL ShaderLanguage Version: " + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
  if (glIsEnabled(GL_MULTISAMPLE) == GL_TRUE)
    DebugLog("Status: Multisample Enabled: TRUE");
  else
    DebugLog("Status: Multisample Enabled: FALSE");
  GLint samples;
  glGetIntegerv(GL_SAMPLES, &samples);
  DebugLog("Status: Number of samples: " + std::to_string(samples));

#ifdef W_DEBUG_OPENGL
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glErrorMessageCallback, 0);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  _main_window_fs = this;

  DebugLog("WMainWindowFS CREATED.");
}


WMainWindowFS::~WMainWindowFS() {
  // Terminate GLFW
  glfwDestroyWindow(_window);
  _main_window_fs = nullptr;
  DebugLog("WMainWindowFS DESTROYED.");
};



void WMainWindowFS::run() {
  // Show traces with the resolution, framebuffer size and density
#ifdef W_DEBUG_BUILD
  int m_width{ 0 };
  int m_height{ 0 };
  glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &m_width, &m_height);

  int dpiX{ 0 };
  int dpiY{ 0 };
  if (m_width && m_height) {
    dpiX = _screen_size.w() / m_width;
    dpiY = _screen_size.h() / m_height;
  }

  int fb_width{ 0 };
  int fb_height{ 0 };
  glfwGetFramebufferSize(_window, &fb_width, &fb_height);

  int w_width{ 0 };
  int w_height{ 0 };
  glfwGetWindowSize(_window, &w_width, &w_height);

  DebugLog("Resolution (pixels): " + std::to_string(_screen_size.w()) + " x " + std::to_string(_screen_size.h()));
  DebugLog("Density (pixels per mm): " + std::to_string(dpiX) + " x " + std::to_string(dpiY));
  DebugLog("Density (pixels per inch): " + std::to_string(int(25.4f * dpiX)) + " x " + std::to_string(int(25.4f * dpiY)));
  DebugLog("Framebuffer size (pixels): " + std::to_string(fb_width) + " x " + std::to_string(fb_height));
  DebugLog("Window size (pixels): " + std::to_string(w_width) + " x " + std::to_string(w_height));

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  DebugLog("Viewport: (" + std::to_string(viewport[0]) + ", " + std::to_string(viewport[1]) + ") - (" + std::to_string(viewport[2]) + ", " + std::to_string(viewport[3]) + ")");
#endif
  
  glClearColor(_bkg_color.r(), _bkg_color.g(), _bkg_color.b(), _bkg_color.a());
  
  // Refresh for the current OpenGL context
  glfwSwapInterval(_swap_interval);

  glfwShowWindow(_window);

  DebugLog("Starting the event loop with Swap Interval = " + std::to_string(_swap_interval) + "...\n");

  // Event loop
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    eventLoopIteration();
 
    objDraw();

    glfwSwapBuffers(_window);
    glfwPollEvents();
    checkButtonRepeatClicked();
  } while (!glfwWindowShouldClose(_window));

}


//**  STATIC CALLBACKS  **//

void WMainWindowFS::key_callback(GLFWwindow *window, int key, int scancode, int action, int modifiers) {
  DebugLog("Key: = " + std::to_string(key) + "  Scancode = " + std::to_string(scancode) + "  Action = " + std::to_string(action) + "  Modifiers : " + std::to_string(modifiers));

  if (action == GLFW_PRESS)
    _main_window_fs->keyPressed(key, scancode, modifiers);
  else if (action == GLFW_RELEASE)
    _main_window_fs->keyReleased(key, scancode, modifiers);
  else if (action == GLFW_REPEAT)
    _main_window_fs->keyRepeated(key, scancode, modifiers);
}


void WMainWindowFS::mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers) {
  double posX, posY;
  glfwGetCursorPos(window, &posX, &posY);
  auto mouse_pos = glboost::Position2D(glboost::PositionPxl(posX), glboost::PositionPxl(posY));

  DebugLog("Mouse event : button = " + std::to_string(button) + "  Action = " + std::to_string(action) + "  Modifiers = " + std::to_string(modifiers)
            + " Mouse position : " + std::to_string(mouse_pos.x()) + ", " + std::to_string(mouse_pos.y()));

  static const std::chrono::milliseconds DOUBLE_CLICK_THRESHOLD{ 200 };
  static std::chrono::system_clock::time_point last_click_time;
  auto now_time{ std::chrono::system_clock::now() };
  if (action == GLFW_PRESS) {
    // Check previous Press event time and affected control to trigger a double click event
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now_time - last_click_time) < DOUBLE_CLICK_THRESHOLD) {
      _main_window_fs->event_dispatcher.buttonDoubleClicked(mouse_pos);
      last_click_time = std::chrono::system_clock::time_point();
    }
    else {
      _main_window_fs->event_dispatcher.buttonClicked(MouseButton(button), modifiers, mouse_pos);
      last_click_time = now_time;
    }
  }
  else { // RELEASE
    _main_window_fs->event_dispatcher.buttonReleased(MouseButton(button), modifiers, mouse_pos);
  }
}


void WMainWindowFS::mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    _main_window_fs->event_dispatcher.dragged(LEFT_BUTTON, Position2D(PositionPxl(xpos), PositionPxl(ypos)));
  else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    _main_window_fs->event_dispatcher.dragged(RIGHT_BUTTON, Position2D(PositionPxl(xpos), PositionPxl(ypos)));
  else
    _main_window_fs->event_dispatcher.mouseMoved(Position2D(PositionPxl(xpos), PositionPxl(ypos)));
}


void WMainWindowFS::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  double posX, posY;
  glfwGetCursorPos(window, &posX, &posY);
  _main_window_fs->event_dispatcher.scrolled(PositionPxl(yoffset), glboost::Position2D(glboost::PositionPxl(posX), glboost::PositionPxl(posY)));
}


void WMainWindowFS::checkButtonRepeatClicked() {
  static auto previous_button_state{ GLFW_RELEASE };
  static const std::chrono::milliseconds REPEAT_CLICK_THRESHOLD{ 500 };
  static std::chrono::system_clock::time_point last_click_time;

  if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    auto now_time{ std::chrono::system_clock::now() };
    if (previous_button_state == GLFW_PRESS) {
      
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now_time - last_click_time) > REPEAT_CLICK_THRESHOLD) {
        double posX, posY;
        glfwGetCursorPos(_window, &posX, &posY);
        _main_window_fs->event_dispatcher.buttonRepeatClicked(glboost::Position2D(glboost::PositionPxl(posX), glboost::PositionPxl(posY)));
        DebugLog("Mouse buttonRepeatClicked event. Mouse position : " + std::to_string(posX) + ", " + std::to_string(posY));
      }
    }
    else {
      last_click_time = now_time;
      previous_button_state = GLFW_PRESS;
    }
  }
  else
    previous_button_state = GLFW_RELEASE;
}



void WMainWindowFS::glfw_error_callback(int error, const char* description) {
  ErrorLog("GLFW Error: " + std::to_string(error) + " - " + description);
}


#ifdef W_DEBUG_OPENGL
  void GLAPIENTRY WMainWindowFS::glErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    static const std::map<int, std::string> source_map{ {GL_DEBUG_SOURCE_API, std::string("API - Calls to the OpenGL API")},
                                                  {GL_DEBUG_SOURCE_WINDOW_SYSTEM, std::string("WINDOW_SYSTEM - Calls to a window - system API")},
                                                  {GL_DEBUG_SOURCE_SHADER_COMPILER, std::string("SHADER_COMPILER - A compiler for a shading language")},
                                                  {GL_DEBUG_SOURCE_THIRD_PARTY, std::string("THIRD_PARTY - An application associated with OpenGL")},
                                                  {GL_DEBUG_SOURCE_APPLICATION, std::string("APPLICATION - Generated by the user of this application")},
                                                  {GL_DEBUG_SOURCE_OTHER, std::string("OTHER - Some other source ")} };

    static const std::map<int, std::string> type_map{ {GL_DEBUG_TYPE_ERROR, std::string("ERROR - An error, typically from the API")},
                                                {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, std::string("DEPRECATED_BEHAVIOR - Some behavior marked deprecated has been used")},
                                                {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, std::string("UNDEFINED_BEHAVIOR - Something has invoked undefined behavior")},
                                                {GL_DEBUG_TYPE_PORTABILITY, std::string("PORTABILITY - Some functionality the user relies upon is not portable")},
                                                {GL_DEBUG_TYPE_PERFORMANCE, std::string("PERFORMANCE - Code has triggered possible performance issues")},
                                                {GL_DEBUG_TYPE_MARKER, std::string("MARKER - Command stream annotation")},
                                                {GL_DEBUG_TYPE_PUSH_GROUP, std::string("PUSH_GROUP - Group pushing")},
                                                {GL_DEBUG_TYPE_POP_GROUP, std::string("POP_GROUP")},
                                                {GL_DEBUG_TYPE_OTHER, std::string("OTHER: Some other type")} };

    static const std::map<int, std::string> severity_map{ {GL_DEBUG_SEVERITY_HIGH, std::string("HIGH -  Error or highly dangerous undefined behavior")},
                                                    {GL_DEBUG_SEVERITY_MEDIUM, std::string("MEDIUM - Major performance warnings, shader compilation / linking warnings, or use of deprecated functionality")},
                                                    {GL_DEBUG_SEVERITY_LOW, std::string("LOW - Redundant state change, minor performance warning, or unimportant undefined behavior")},
                                                    {GL_DEBUG_SEVERITY_NOTIFICATION, std::string("NOTIFICATION")} };

    std::stringstream log;
    log << "GL DEBUG CALLBACK. \nSource = " << source_map.at(source) << "\nType = " << type_map.at(type) << "\nId = " << id << "\nSeverity = " << severity_map.at(severity) << "\nMessage = " << message << "\n";

    ERROR_LOG(log.str());
  }
#endif //W_DEBUG_OPENGL
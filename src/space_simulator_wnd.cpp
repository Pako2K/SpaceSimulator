#include "space_simulator_wnd.h"

#include "utils/logger.h"

const int          __OPEN_GL_VERSION_MAJOR__   {4};
const int          __OPEN_GL_VERSION_MINOR__   {0};
const int          __OPEN_GL_PROFILE__         { GLFW_OPENGL_CORE_PROFILE };
const int          __SWAP_INTERVAL__           {0};
const int          __FRAME_BUFFER_SAMPLES__    {1};


/// ***********************************************************************************************************************
/// ******************************************** STATIC Data Members ******************************************************

SpaceSimulatorWnd * SpaceSimulatorWnd::_app_wnd {nullptr};

/// ******************************************** STATIC Data Members (END) ************************************************
/// ***********************************************************************************************************************


/// ***********************************************************************************************************************
/// ************************************************* PUBLIC **************************************************************

/// run()
void SpaceSimulatorWnd::run() {
  if ( !_app_wnd ) {
    try {
      _app_wnd = new SpaceSimulatorWnd();
    }
    catch ( std::string &exc) {
      ERROR_LOG( exc );
      return;
    }

    _app_wnd->show();

     delete _app_wnd;
  }
}

/// ************************************************* PUBLIC (END) ********************************************************
/// ***********************************************************************************************************************

/// ***********************************************************************************************************************
/// ************************************************* PRIVATE *************************************************************

/// CONSTRUCTOR()
SpaceSimulatorWnd::SpaceSimulatorWnd() throw(std::string) {

  DEBUG_LOG( "SpaceSimulatorWnd: CONSTRUCTOR Called" );

  initializeWnd();

  initializeOpenGL();

  // Refresh for the current OpenGL context
	glfwSwapInterval(__SWAP_INTERVAL__);
}

/// initializeWnd()
void SpaceSimulatorWnd::initializeWnd() throw(std::string) {
  //Start GLFW for the whole application (all windows reuse the same GLFW)
	if (!glfwInit()) {
		// Initialization failed
		throw std::string( "GLFW CANNOT BE INITIALIZED." );
	}
  DEBUG_LOG( "Status: Using GLFW Version: " + std::string((char *)glfwGetVersionString()) );

  // Define GLFW Error callback
	glfwSetErrorCallback( glfw_error_callback );

  // Using OpenGL 4.0, core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, __OPEN_GL_VERSION_MAJOR__);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, __OPEN_GL_VERSION_MINOR__);
	glfwWindowHint(GLFW_OPENGL_PROFILE, __OPEN_GL_PROFILE__);

	// Create full screen window (with no border), and initially not visible, using maximum resolution (the window size)
	#ifndef SS_DEBUG
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  #endif
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Set number of samples for antialiasing
  glfwWindowHint(GLFW_SAMPLES, __FRAME_BUFFER_SAMPLES__);

  // Get primary monitor and its current resolution (video mode)
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if ( !monitor ) {
    glfwTerminate();
    throw std::string("Could not get monitor.");
  }

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if ( !mode ) {
    glfwTerminate();
    throw std::string("Could not get Video mode.");
  }

  // Resolution of the screen is in pixels
  _window = glfwCreateWindow(mode->width, mode->height, "SPACE SIMULATOR", nullptr, nullptr);
  if ( !_window ){
    glfwTerminate();
    throw std::string("Could not create window.");
  }

  // Show traces with the resolution, framebuffer size and density
	#ifdef SS_DEBUG
    int width   {0};
    int height  {0};
    glfwGetMonitorPhysicalSize(monitor, &width, &height);

    uint16_t dpiX {0};
    uint16_t dpiY {0};
    if (width && height) {
      dpiX = mode->width / width;
      dpiY = mode->height / height;
    }
    glfwGetFramebufferSize(_window, &width, &height);
    DEBUG_LOG( "Resolution (pixels): " + std::to_string(mode->width) + " x " + std::to_string(mode->height) );
    DEBUG_LOG( "Density (pixels per mm): " + std::to_string(dpiX) + " x " + std::to_string(dpiY) );
    DEBUG_LOG( "Density (pixels per inch): " + std::to_string(int(25.4f * dpiX))  + " x " + std::to_string(int(25.4f * dpiY)) );
    DEBUG_LOG( "Framebuffer size (pixels): " + std::to_string(width) + " x " + std::to_string(height) );
  #endif // BUILD_DEBUG

  // Make this window context the current one (for OpenGL)
	glfwMakeContextCurrent(_window);
	if ( !glfwGetCurrentContext() ) {
		glfwDestroyWindow(_window);
		glfwTerminate();
		throw std::string("Couldn't make Window context the current one.");
	}

	// Set Key and mouse callbacks
	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
}


/// initializeOpenGL()
void SpaceSimulatorWnd::initializeOpenGL() throw(std::string) {
  // Initialize GLEW
	glewExperimental = GL_TRUE;

  GLenum err = glewInit();
  if ( err != GLEW_OK ) {
    glfwDestroyWindow(_window);
    glfwTerminate();
    throw std::string("Not possible to initialize GL: " + std::string((char *)glewGetErrorString(err)));
  }

  DEBUG_LOG( "Status: Using GLEW Version: " + std::string((char *)glewGetString(GLEW_VERSION)) );
  DEBUG_LOG( "Status: Using GL Vendor: " + std::string((char *)glGetString(GL_VENDOR)) );
  DEBUG_LOG( "Status: Using GL Renderer: " + std::string((char *)glGetString(GL_RENDERER)) );
  DEBUG_LOG( "Status: Using GL Version: " + std::string((char *)glGetString(GL_VERSION)) );
  DEBUG_LOG( "Status: Using GL ShaderLanguage Version: " + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION)) );

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  DEBUG_LOG( "Viewport: (" + std::to_string(viewport[0]) + ", " + std::to_string(viewport[1]) + ") - (" + std::to_string(viewport[2]) + ", " + std::to_string(viewport[3]) +")");
}

/// DESTRUCTOR
SpaceSimulatorWnd::~SpaceSimulatorWnd() {
  DEBUG_LOG("SpaceSimulatorWnd: DESTROYED");

  glfwDestroyWindow(_window);
  glfwTerminate();
}


/// show()
void SpaceSimulatorWnd::show() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glfwShowWindow(_window);

	// Event loop
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glfwSwapBuffers(_window);

		glfwPollEvents();
	} while (!glfwWindowShouldClose(_window));
}



/// glfw_error_callback(int error, const char* description)
void SpaceSimulatorWnd::glfw_error_callback(int error, const char* description){
  ERROR_LOG( "GLFW # " + std::to_string(error) + " # " + std::string(description) );
}

/// key_callback(GLFWwindow * window, int key, int scancode, int action, int modifiers)
void SpaceSimulatorWnd::key_callback(GLFWwindow * window, int key, int scancode, int action, int modifiers) {

}

/// mouse_button_callback
void SpaceSimulatorWnd::mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers) {

}

/// ************************************************* PRIVATE (END) *******************************************************
/// ***********************************************************************************************************************

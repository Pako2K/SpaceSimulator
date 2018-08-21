#ifndef SPACE_SIMULATOR_WND_H
#define SPACE_SIMULATOR_WND_H

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/**
  * \brief  SpaceSimulatorWnd singleton class.
  *         Contains the event loop and all the window related setup, event callbacks, etc.
  *         Creates and runs the space simulator singleton.
  */
class SpaceSimulatorWnd
{
public:
  /**
    *  \brief Creates the Space Simulator Window singleton and starts the event loop.
    *  @param
    *  @return void
    */
  static void run();


private:
  /// ***********************************************************************************************************************
  /// ******************************************** Data Members *************************************************************

  /**
    *  Pointer to the SpaceSimulatorWnd singleton
    */
  static
  SpaceSimulatorWnd    *_app_wnd;

  /**
    *  Pointer to GLFW window
    */
  GLFWwindow           *_window;
  /// ******************************************** Data Members (END) *******************************************************
  /// ***********************************************************************************************************************


  /// ***********************************************************************************************************************
  /// *********************************************** Operations ************************************************************

  /**
    *  \brief Constructor of the SpaceSimulatorWnd singleton
    *         Reads the configuration parameters:
                     FRAME_BUFFER_SAMPLES: Number of frame buffer samples for antialiasing
                     SWAP_INTERVAL: Number of screen refreshes to complete before next framebuffer swap
    *         Creates the GLFW window and the context
    *         Initializes OpenGL
    *         Creates the space singleton
    *  @throw  string exception containing the description of the issue
    */
  SpaceSimulatorWnd() throw(std::string);

  /**
    *  \brief Creates the GLFW window and the context
    *  @param
    *  @return void
    *  @throw  string exception containing the description of the issue
    */
  void initializeWnd() throw(std::string);

  /**
    *  \brief Initializes OpenGL
    *  @param
    *  @return void
    *  @throw  string exception containing the description of the issue
    */
  void initializeOpenGL() throw(std::string);

  /**
    *  \brief Destructor of the SpaceSimulatorApp singleton
    */
  ~SpaceSimulatorWnd();

  /**
    *  \brief Executes the Window Event Loop
    *  @param
    *  @return void
    */
  void show();

  /**
    *  GLFW Error events callback
    */
	static void glfw_error_callback(int error, const char* description);

  /**
    *  GLFW Keyboard events callback
    */
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int modifiers);

  /**
    *  GLFW Mouse events callback
    */
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	/// *********************************************** Operations (END) ******************************************************
  /// ***********************************************************************************************************************
};


#endif // SPACE_SIMULATOR_WND_H

/** \mainpage Space Simulator
 *  \section  Introduction
 *  \section  Dependencies
 *            The following libraries are required:
 *            - utils-1.0.0 (https://github.com/Pako2K/utils)
 *            - GLFW-3.2.1 (https://github.com/glfw/glfw/releases/tag/3.2.1)
 *            - GLEW-2.1.0 (https://github.com/nigels-com/glew/releases/tag/glew-2.1.0)
 *  \section  Build
 *      \subsection Macros
 *                  - GLEW_STATIC --> Use always
 *                  - DEBUG_BUILD --> Use when building for debugging and test purposes. Logs will be shown in the standard output
 *                  - RELEASE_BUILD --> Use when building a deployable version
 *                  - Other macros --> See Logger macros for increasing/decreasing log level
 *  \section  Logging
 *            Logging is performed using the Logger macros.
 *
 *            In Debug Build all logging is done to the standard output.
 *
 *            In Release Build, errors and info logs are by default written to the <b>logs/SpaceSimulator.log</b> file,
 *                unless otherwise stated via the Logger macros during the build
 *
 *  \author   Pako2K
 *
 *  \version  0.1
 *
 *  \date     2018-08-24
 */


#include <logger.h>
#include <files/properties_file_reader.h>
#include <glboost/graphic_types.h>

#include <space_simulator_wnd.h>


// Location of the log file
static const std::string       LOG_FILE { "logs/SpaceSimulator.log" };

// Location of the config file with the application configuration
static const std::string       PROPS_FILE_NAME { "config/space_simulator_wnd.cfg" };

// Window backgrouund color
const glboost::Color4f         BACKGROUND_COLOR { 0, 0, 0.01f, 1 };

/**
 *  @brief Application entry function
 *
 *  It initializes the Logger and starts the SpaceSimulatorWnd singleton Class
 */
int main(int argc, char** args) {

  InitializeLogger(LOG_FILE);

  int return_code{ 0 };
  try {
    // Read properties file
    utils::PropertiesFileReader properties(PROPS_FILE_NAME);

    SpaceSimulatorWnd::initialize();

    SpaceSimulatorWnd::samples(properties.property<uint8_t>("FRAME_BUFFER_SAMPLES"));
    SpaceSimulatorWnd::contextVersion(properties.property<uint8_t>("OPEN_GL_VERSION_MAJOR"), properties.property<uint8_t>("OPEN_GL_VERSION_MINOR"));
    SpaceSimulatorWnd::disableMouseMove();
    //SpaceSimulatorWnd::disableMouseScroll();
    //SpaceSimulatorWnd::disableMouseClick();
    //SpaceSimulatorWnd::disableKeyboard();

    // Create Main Window
    SpaceSimulatorWnd main_wnd(std::pair<std::string, std::string>(properties.property("REAL_DATE_FORMAT"), properties.property("REAL_TIME_FORMAT")),
                               std::chrono::milliseconds(properties.property<uint32_t>("ON_SCREEN_INFO_UPD_INTERVAL")));
    
    main_wnd.swapInterval(properties.property<uint8_t>("SWAP_INTERVAL"));
    main_wnd.background(BACKGROUND_COLOR);
    
    // Execute
    main_wnd.run();
  }
  catch (std::exception& exc) {
    ErrorLog( exc.what() );
    return_code = 1;
  }
  catch (...) {
    ErrorLog("Unexpected exception!");
    return_code = 1;
  }

  DestroyLogger();
  SpaceSimulatorWnd::terminate();
  return return_code;
}

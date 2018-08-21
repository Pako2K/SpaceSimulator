#include <string>

#include "space_simulator_wnd.h"

#include "utils/logger.h"

/**
 *  MAIN FUNCTION
 *  It simply starts the SpaceSimulatorWnd singleton Class
 */
int main(int argc, char** args) {

  try {
    SpaceSimulatorWnd::run();
  }
  catch (std::string &exc) {
    ERROR_LOG( exc );
    return 1;
  }

  return 0;
}


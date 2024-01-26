#include <logger.h>

// Test constructor and changes in visibility
bool WOBJECT_TEST_1();


// Test default functionality for main container
bool WCONTAINER_TEST_1();

// Test default functionality for main container and chidren containers
bool WCONTAINER_TEST_2();


// Test default functionality
bool WCONTROL_TEST_1();


// Test default functionality
bool MAIN_WINDOW_TEST_1();


// Test functionality with customization: constructor, draw, mouse events, ...
bool MAIN_WINDOW_TEST_2();

// Test functionality with customization: constructor, draw, mouse events, ... + 1 control + event listeners
bool MAIN_WINDOW_TEST_3();


// Test label and check button
bool LABEL_BUTTON_1();

int main(int argc, char** args) {
  InitializeLogger(std::cout);
 // WOBJECT_TEST_1();
 // WCONTAINER_TEST_1();
 // WCONTAINER_TEST_2();

  //WCONTROL_TEST_1();

  /*MAIN_WINDOW_TEST_1();
  MAIN_WINDOW_TEST_2();
  MAIN_WINDOW_TEST_3();
  */
  LABEL_BUTTON_1();
     
  return 0;
}



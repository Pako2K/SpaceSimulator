#include <glwnd/core/w_abs_button.h>

#include <logger.h>

using namespace glwnd;
using namespace glboost;


void WAbsButton::click() {
  CALL_LISTENERS(ButtonClicked);
}


void WAbsButton::buttonClicked(MouseButton button, int modifiers, const glboost::Position2D & pos) {
  if (button == LEFT_BUTTON)
    _state = BUTTON_PRESSED;
  DebugLog("Button clicked.");

  customButtonPressed();
}

void WAbsButton::buttonRepeatClicked(const glboost::Position2D & pos) {
  if (pos.x() <= size().w() && pos.y() <= size().h()) {
    // If the release event is within the button area, trigger click event
    CALL_LISTENERS(ButtonRepeatClicked);
  }
  DebugLog("Button repeat clicked");
}


void WAbsButton::buttonReleased(MouseButton button, int modifiers, const glboost::Position2D & pos) {
  if (button != LEFT_BUTTON)
    return;

  _state = BUTTON_RELEASED;
  if (pos.x() <= size().w() && pos.y() <= size().h()) {
    // If the release event is within the button area, trigger click event
    CALL_LISTENERS(ButtonClicked);
  }
  DebugLog("Button released");
  customButtonReleased();
}


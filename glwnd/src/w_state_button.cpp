#include <glwnd/w_state_button.h>

#include <logger.h>

using namespace glwnd;
using namespace glboost;


WStateButton::WStateButton(const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon)
  : WControl{ pos, size, true }, _rectangle{ pos, size }, _icons{ new gPicture{pos, size, released_icon}, new gPicture{pos, size, clicked_icon} } {
  initialize(pos, size);
}


WStateButton::WStateButton(glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, const std::string& released_icon, const std::string& clicked_icon)
  : WStateButton{ Position2D{ pos_x, pos_y }, Size2D{ width, height }, released_icon, clicked_icon } {}


WStateButton::WStateButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon)
  : WControl{ parent, pos, size, true }, _rectangle{ pos, size }, _icons{ new gPicture{pos, size, released_icon}, new gPicture{pos, size, clicked_icon} } {
  initialize(pos, size);
}


WStateButton::WStateButton(WContainer& parent, glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, const std::string& released_icon, const std::string& clicked_icon)
  : WStateButton{ parent, Position2D{ pos_x, pos_y }, Size2D{ width, height }, released_icon, clicked_icon } {}


WStateButton::~WStateButton() {
  for (auto icon : _icons)
    delete icon;
}

void WStateButton::initialize(const Position2D& pos, const Size2D& size) {

  // Initiallly the rectangle is not drawn
  _rectangle.border(false);
}


void WStateButton::position(const glboost::Position2D& new_pos) {
  WControl::position(new_pos);
  _rectangle.position(new_pos);
  for (auto icon : _icons)
    icon->position(new_pos);
}


void WStateButton::size(const glboost::Size2D& new_size) {
  WControl::size(new_size);
  _rectangle.size(new_size);
  for (auto icon : _icons)
    icon->size(new_size);
}


void WStateButton::switchState() {
  _state = ButtonState(!_state);
  CALL_LISTENERS(ButtonStateChanged, _state);
  if (_state == BUTTON_SELECTED)
    CALL_LISTENERS(ButtonSelected);
  else
    CALL_LISTENERS(ButtonUnselected);;
}


void WStateButton::buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) {
  // Set transparency to 50%
  if (button == LEFT_BUTTON) {
    _icons[_state]->opaqueness(0.5f);
    _rectangle.fillColor(Color4f(_rectangle.fillColor().r(), _rectangle.fillColor().g(), _rectangle.fillColor().b(), _rectangle.fillColor().a() / 2));
  }
  DebugLog("Check Button clicked.");
}


void WStateButton::buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) {
  if (button != LEFT_BUTTON)
    return;

  _icons[_state]->opaqueness(1.0f);
  _rectangle.fillColor(Color4f(_rectangle.fillColor().r(), _rectangle.fillColor().g(), _rectangle.fillColor().b(), 2 * _rectangle.fillColor().a()));
  if (pos.x() <= size().w() && pos.y() <= size().h()) {
    // If the release event is within the button area, change the state
    switchState();
  }

  DebugLog("Check Button released");
}


void WStateButton::draw() {
  // Draw background
  _rectangle.draw();
  _icons[_state]->draw();
}


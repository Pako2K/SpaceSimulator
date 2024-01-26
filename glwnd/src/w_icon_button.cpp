#include <glwnd/w_icon_button.h>

#include <logger.h>

using namespace glwnd;
using namespace glboost;


WIconButton::WIconButton(const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon)
  : WAbsButton{ pos, size }, _rectangle{ pos, size }, _icons{ new gPicture{pos, size, released_icon}, new gPicture{pos, size, clicked_icon} } {
  initializeButton();
}


WIconButton::WIconButton(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, const std::string& released_icon, const std::string& clicked_icon)
  : WAbsButton{ parent, pos, size }, _rectangle{ pos, size }, _icons{ new gPicture{pos, size, released_icon}, new gPicture{pos, size, clicked_icon} } {
  initializeButton();
}


WIconButton::~WIconButton() {
  for (auto icon : _icons)
    delete icon;
}

void WIconButton::initializeButton() {
  // Initiallly the rectangle is not drawn
  _rectangle.border(false);
}


void WIconButton::position(const glboost::Position2D & new_pos) {
  WControl::position(new_pos);
  _rectangle.position(new_pos);
  for (auto icon : _icons)
    icon->position(new_pos);
}


void WIconButton::size(const glboost::Size2D & new_size) {
  WControl::size(new_size);
  _rectangle.size(new_size);
  for (auto icon : _icons)
    icon->size(new_size);
}


void WIconButton::draw() {
  // Draw background
  _rectangle.draw();
  _icons[_state]->draw();
}


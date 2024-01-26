#include <glwnd/core/w_control.h>

#include <logger.h>


using namespace glboost;
using namespace glwnd;


WControl::WControl(Position2D a_pos, Size2D a_size, bool has_mouse_interface)
  : WControl{ WContainer::mainContainer(), a_pos, a_size, has_mouse_interface } {}


WControl::WControl(WContainer& parent, Position2D a_pos, Size2D a_size, bool has_mouse_interface)
  : WObject{ a_pos, a_size }, _parent{ parent }, _has_mouse_interface{ has_mouse_interface } {
  _parent.addChild(*this, has_mouse_interface);
  DebugLog("WControl CREATED.");
}


WControl::WControl(const WControl& contr) : WControl{contr._parent, contr._position, contr._size, contr._has_mouse_interface} {
  DebugLog("[ Actually COPIED ]");
}


WControl::~WControl() {
  _parent.removeChild(*this);
  DebugLog("WControl DESTROYED.");
}


void WControl::position(const Position2D& new_pos) {
  WObject::position(new_pos);
  _parent.updateChild(*this);
}


void WControl::size(const Size2D& new_size) {
  WObject::size(new_size);
  _parent.updateChild(*this);
}

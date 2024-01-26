#include <glwnd/core/w_container.h>

#include <logger.h>


using namespace glwnd;
using namespace glboost;


WContainer*   WContainer::_main_container{ nullptr };
WObject*      WContainer::_focus_obj{ nullptr };
Position2D    WContainer::_focus_obj_abs_pos{ 0, 0 };


WContainer& WContainer::mainContainer() {
  if (!_main_container)
    throw std::logic_error("Main Container not created");
  return *_main_container;
}


WContainer::WContainer(Position2D a_pos, Size2D a_size, bool has_mouse_interface) : WObject{ a_pos, a_size }, event_dispatcher{ *this }, _surface{ a_size } {
  if (!_main_container) {
    _main_container = this;
    _focus_obj = this;
    _focus_obj_abs_pos = a_pos;
    // _abs_position does not change
    DebugLog("MAIN WContainer CREATED.");
  }
  else {
    _parent = _main_container;
    _abs_position = _parent->_abs_position + a_pos;
    _parent->addChild(*this, has_mouse_interface);
    DebugLog("WContainer CREATED.");
  }
}


WContainer::WContainer(WContainer& parent, Position2D a_pos, Size2D a_size, bool has_mouse_interface)
  : WObject{ a_pos, a_size }, event_dispatcher{ *this }, _surface{ a_size }, _parent{ &parent } {
  _parent->addChild(*this, has_mouse_interface);
  DebugLog("WContainer with parent CREATED.");
}


WContainer::~WContainer() {
  if (this == _main_container) {
    _main_container = nullptr;
    DebugLog("MAIN WContainer DESTROYED.");
  }
  else
    DebugLog("WContainer DESTROYED.");
}


void WContainer::addChild(WObject& child, bool has_mouse_interface) {
  // Add to list of children to be drawn
  _children.push_back(&child);

  // Add to rectangular surface
  if (has_mouse_interface)
    _surface.addObject(child);
}


void WContainer::updateChild(WObject& child) {
  _surface.updateObject(child);
}


void WContainer::removeChild(WObject& child) {
  auto iterator = std::find(_children.begin(), _children.end(), &child);
  if (iterator != _children.end()) {
    _children.erase(iterator);
    _surface.removeObject(child);
  }
}


void WContainer::position(const Position2D& new_pos) {
  WObject::position(new_pos);

  if (_parent) {
    _abs_position = _parent->_abs_position + new_pos;
    _parent->updateChild(*this);
  }
}


void WContainer::size(const Size2D& new_size) {
  WObject::size(new_size);

  if (_parent)
    _parent->updateChild(*this);

  // Recalculate WRectSurface
  _surface.size(new_size);
}


void WContainer::EventDispatcher::buttonClicked(MouseButton button, int modifiers, const Position2D& pos) {
  std::pair<MouseInterface*, Position2D> params = dispatchTo(pos, true);
  params.first->buttonClicked(button, modifiers, params.second);
}


void WContainer::EventDispatcher::buttonRepeatClicked(const Position2D& pos) {
  std::pair<MouseInterface*, Position2D> params = dispatchTo(pos, false);
  params.first->buttonRepeatClicked(params.second);
}


void WContainer::EventDispatcher::buttonReleased(MouseButton button, int modifiers, const Position2D& pos) {
  // Send event to the object with the click focus
  WContainer::_focus_obj->buttonReleased(button, modifiers, pos - WContainer::_focus_obj_abs_pos);
  // Focus returns to the main container
  WContainer::_focus_obj = &_this_container;
  WContainer::_focus_obj_abs_pos = _this_container.WObject::position();
}


void WContainer::EventDispatcher::buttonDoubleClicked(const Position2D& pos) {
  // Double click event comes always after a buttonClicked / buttonReleased
  std::pair<MouseInterface*, Position2D> params = dispatchTo(pos, true);
  params.first->buttonDoubleClicked(params.second);
}


void WContainer::EventDispatcher::scrolled(PositionPxl offset, const Position2D& pos) {
  std::pair<MouseInterface*, Position2D> params = dispatchTo(pos, false);
  params.first->scrolled(offset, params.second);
}


void WContainer::EventDispatcher::dragged(MouseButton button, const Position2D& pos) {
  // Sed event to the object with the click focus
  WContainer::_focus_obj->dragged(button, pos - WContainer::_focus_obj_abs_pos);
}


void WContainer::EventDispatcher::mouseMoved(const Position2D& pos) {
  std::pair<MouseInterface*, Position2D> params = dispatchTo(pos, false);
  params.first->mouseMoved(params.second);
}


std::pair<MouseInterface*, Position2D> WContainer::EventDispatcher::dispatchTo(const Position2D& pos, bool changeFocus) {
  MouseInterface* obj_interface;

  // Deliver event to controls
  auto object = _this_container._surface.getObject(pos);
  if (!object) {
    // Trigger event in the container
    if (changeFocus)
      WContainer::_focus_obj = &_this_container;
    // Absolute position remains as is
    obj_interface = &_this_container;
    object = &_this_container;
  }
  else {
    // Check whether the control is another container
    WContainer* subcontainer = dynamic_cast<WContainer*>(object);
    if (subcontainer) {
      WContainer::_focus_obj_abs_pos += subcontainer->WObject::position();
      obj_interface = &subcontainer->event_dispatcher;
    }
    else {
      if (changeFocus) {
        WContainer::_focus_obj = object;
        WContainer::_focus_obj_abs_pos += object->position();
      }
      obj_interface = object;
    }
  }
  return std::pair<MouseInterface*, Position2D>(obj_interface, pos - object->position());
}

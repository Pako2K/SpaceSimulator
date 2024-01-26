#include <glwnd/core/w_object.h>

#include <logger.h>


using namespace glwnd;


WObject::WObject(const glboost::Position2D& a_pos, const glboost::Size2D& a_size) : _position{ a_pos }, _size{ a_size } {
  DebugLog("WObject CREATED.");
}


WObject::WObject(const WObject& obj) : _position{ obj._position }, _size{ obj._size }, _draw_fn_ptr{ obj._draw_fn_ptr } {
  DebugLog("WObject CREATED AS COPY .");
}


WObject::WObject(WObject&& obj) : _position{ std::move(obj._position) }, _size{ std::move(obj._size) }, _draw_fn_ptr{ obj._draw_fn_ptr }{
  DebugLog("WObject MOVED. Original WObject to be destroyed");
  obj.~WObject();
}


WObject::~WObject() {
  DebugLog("WObject DESTROYED.");
}


void WObject::show() {
  _draw_fn_ptr = &WObject::draw;
}


void WObject::hide() {
  _draw_fn_ptr = &WObject::hiddenDraw;
}


void WObject::position(const glboost::Position2D& new_position) {
  _position = new_position;
}


void WObject::size(const glboost::Size2D& new_size) {
  _size = new_size;
}

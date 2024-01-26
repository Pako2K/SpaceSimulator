#include <space_simulator_wnd.h>

using namespace glwnd;

#define SHOW_BODY(body) ((body).TYPE == physics::KBody::BodyType::PLANET && (_list_filter_mask & _SHOW_PLANETS)) || \
                        ((body).TYPE == physics::KBody::BodyType::DWARF_PLANET && (_list_filter_mask & _SHOW_DWARF_PLANETS)) || \
                        ((body).TYPE == physics::KBody::BodyType::MINOR_BODY && (_list_filter_mask & _SHOW_MINOR_BODIES))


void SpaceSimulatorWnd::setListeners() {
  // **************** SIMULATION TICK BUTTONS ********************* //
// ************************************************************** //
  _btn_pause->addListener_ButtonStateChanged(this, [](void* ctxt, ButtonState new_state) { ((SpaceSimulatorWnd*)ctxt)->pause(); });
  _btn_dec_tick->addListener_ButtonClicked(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->decTick(); });
  _btn_inc_tick->addListener_ButtonClicked(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->incTick(); });


  // **************** BODIES LIST SORT BUTTONS ********************* //
  // *************************************************************** //
  _btn_sort_by_distance->addListener_ButtonSelected(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->updateBodiesByDistance(); });
  _btn_sort_by_mass->addListener_ButtonSelected(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->updateBodiesByMass(); });
  _btn_sort_by_name->addListener_ButtonSelected(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->updateBodiesByName(); });
  _btn_sort_by_id->addListener_ButtonSelected(this, [](void* ctxt) { ((SpaceSimulatorWnd*)ctxt)->updateBodiesById(); });


  // **************** BODIES LIST FILTER BUTTONS ********************* //
  // ***************************************************************** //
  _btn_show_planets->addListener_ButtonStateChanged(this, [](void* ctxt, ButtonState new_state) { ((SpaceSimulatorWnd*)ctxt)->filterBodies(SpaceSimulatorWnd::_SHOW_PLANETS, new_state == ButtonState::BUTTON_SELECTED); });
  _btn_show_dwarf_planets->addListener_ButtonStateChanged(this, [](void* ctxt, ButtonState new_state) { ((SpaceSimulatorWnd*)ctxt)->filterBodies(SpaceSimulatorWnd::_SHOW_DWARF_PLANETS, new_state == ButtonState::BUTTON_SELECTED); });
  _btn_show_minor_bodies->addListener_ButtonStateChanged(this, [](void* ctxt, ButtonState new_state) { ((SpaceSimulatorWnd*)ctxt)->filterBodies(SpaceSimulatorWnd::_SHOW_MINOR_BODIES, new_state == ButtonState::BUTTON_SELECTED); });
  _btn_show_satellites->addListener_ButtonStateChanged(this, [](void* ctxt, ButtonState new_state) { ((SpaceSimulatorWnd*)ctxt)->filterBodies(SpaceSimulatorWnd::_SHOW_SATELLITES, new_state == ButtonState::BUTTON_SELECTED); });

}


void SpaceSimulatorWnd::updateBodiesByDistance() {
  size_t pos{ 1 };

  auto init_list_size{ _list_bodies->listSize() };

  _list_bodies->unselect();

  _list_bodies->update(pos++, _space.bodies().root().name());

  std::map<physics::units::LENGTH_T, std::string> by_distance;
  auto child_iter = _space.bodies().children(_space.bodies().root().name());
  while (child_iter.hasNext()) {
    if (SHOW_BODY(*child_iter)) {
      by_distance[(*child_iter).orbit().a()] = (*child_iter).name();
    }
    child_iter.next();
  }
  for (auto& pair : by_distance) {
    _list_bodies->update(pos++, "   " + pair.second);

    if (_list_filter_mask & _SHOW_SATELLITES) {
      auto grand_child_iter = _space.bodies().children(pair.second);
      std::map<physics::units::LENGTH_T, std::string> grand_child_by_distance;
      while (grand_child_iter.hasNext()) {
        grand_child_by_distance[(*grand_child_iter).orbit().a()] = (*grand_child_iter).name();
        grand_child_iter.next();
      }
      for (auto& gc_pair : grand_child_by_distance)
        _list_bodies->update(pos++, "      " + gc_pair.second);
    }
  }

  // Remove elements from the list which are not used any more
  while (pos++ <= init_list_size)
    _list_bodies->pop();
}


void SpaceSimulatorWnd::updateBodiesByMass() {
  size_t pos{ 1 };

  auto init_list_size{ _list_bodies->listSize() };

  _list_bodies->unselect();

  std::string body_name;
  body_name = _space.bodies().root().name();
  _list_bodies->update(pos++, body_name);

  auto child_iter = _space.bodies().children(body_name);
  while (child_iter.hasNext()) {
    body_name = (*child_iter).name();
    if (SHOW_BODY(*child_iter)) {
      _list_bodies->update(pos++, "   " + body_name);
    }
    if (_list_filter_mask & _SHOW_SATELLITES) {
      auto grand_child_iter = _space.bodies().children(body_name);
      while (grand_child_iter.hasNext()) {
        _list_bodies->update(pos++, "      " + (*grand_child_iter).name());
        grand_child_iter.next();
      }
    }
    child_iter.next();
  }

  // Remove elements from the list which are not used any more
  while (pos++ <= init_list_size)
    _list_bodies->pop();

}


void SpaceSimulatorWnd::updateBodiesByName() {
  size_t pos{ 1 };
  auto init_list_size{ _list_bodies->listSize() };

  _list_bodies->unselect();

  _list_bodies->update(pos++, _space.bodies().root().name());

  std::map<std::string, std::string> child_hashmap;
  std::string common_name;
  auto child_iter = _space.bodies().children(_space.bodies().root().name());
  while (child_iter.hasNext()) {
    if (SHOW_BODY(*child_iter)) {
      common_name = (*child_iter).COMMON_NAME;
      if (common_name == "")
        common_name = (*child_iter).PROVISIONAL_NAME;
      child_hashmap[common_name] = (*child_iter).name();
    }
    child_iter.next();
  }
  for (auto& pair : child_hashmap) {
    _list_bodies->update(pos++, "   " + pair.second);

    if (_list_filter_mask & _SHOW_SATELLITES) {
      auto grand_child_iter = _space.bodies().children(pair.second);
      decltype(child_hashmap) grand_child_hashmap;
      while (grand_child_iter.hasNext()) {
        common_name = (*grand_child_iter).COMMON_NAME;
        if (common_name == "")
          common_name = (*grand_child_iter).PROVISIONAL_NAME;
        grand_child_hashmap[common_name] = (*grand_child_iter).name();
        grand_child_iter.next();
      }
      for (auto& gc_pair : grand_child_hashmap)
        _list_bodies->update(pos++, "      " + gc_pair.second);
    }
  }

  // Remove elements from the list which are not used any more
  while (pos++ <= init_list_size)
    _list_bodies->pop();
}


void SpaceSimulatorWnd::updateBodiesById() {
  size_t pos{ 1 };
  auto init_list_size{ _list_bodies->listSize() };

  _list_bodies->unselect();

  _list_bodies->update(pos++, _space.bodies().root().name());

  std::map<int64_t, std::string> child_hashmap;
  int64_t body_id;
  auto child_iter = _space.bodies().children(_space.bodies().root().name());
  while (child_iter.hasNext()) {
    if (SHOW_BODY(*child_iter)) {
      body_id = (*child_iter).ID;
      child_hashmap[body_id] = (*child_iter).name();
    }
    child_iter.next();
  }
  for (auto& pair : child_hashmap) {
    _list_bodies->update(pos++, "   " + pair.second);

    if (_list_filter_mask & _SHOW_SATELLITES) {
      auto grand_child_iter = _space.bodies().children(pair.second);
      decltype(child_hashmap) grand_child_hashmap;
      while (grand_child_iter.hasNext()) {
        body_id = (*grand_child_iter).ID;
        grand_child_hashmap[body_id] = (*grand_child_iter).name();
        grand_child_iter.next();
      }
      for (auto& gc_pair : grand_child_hashmap)
        _list_bodies->update(pos++, "      " + gc_pair.second);
    }
  }

  // Remove elements from the list which are not used any more
  while (pos++ <= init_list_size)
    _list_bodies->pop();
}



void SpaceSimulatorWnd::filterBodies(FILTER_MASK filter, bool add) {
  // Update list according to sort crieria and filter
  if (add) 
    _list_filter_mask += filter;
  else
    _list_filter_mask -= filter;
  
  if (_btn_sort_by_distance->state() == ButtonState::BUTTON_SELECTED)
    updateBodiesByDistance();
  else if (_btn_sort_by_mass->state() == ButtonState::BUTTON_SELECTED)
    updateBodiesByMass();
  else if (_btn_sort_by_name->state() == ButtonState::BUTTON_SELECTED)
    updateBodiesByName();
  else if (_btn_sort_by_id->state() == ButtonState::BUTTON_SELECTED)
    updateBodiesById();
}
#include <glwnd/w_state_button_group.h>


using namespace glwnd;


WStateButtonGroup::WStateButtonGroup(WStateButton& button_selected, WStateButton& button_deselected) {
  if (button_selected.state() == ButtonState::BUTTON_UNSELECTED)
    button_selected.switchState();
  if (button_deselected.state() == ButtonState::BUTTON_SELECTED)
    button_deselected.switchState();
  
  _selected = &button_selected;

  add(button_selected);
  add(button_deselected);
}


void WStateButtonGroup::add(WStateButton& button) {
  if (_group.insert(&button).second) {
    if (button.state() == ButtonState::BUTTON_SELECTED && _selected != &button)
      button.switchState();

    //Add listeners
    button.addListener_ButtonStateChanged(&button, [&](void *a_ctxt, ButtonState new_state) {
      if (new_state == ButtonState::BUTTON_SELECTED) {
        if (_selected != a_ctxt) {
          auto prev_selected = _selected;
          _selected = (WStateButton*)a_ctxt;
          prev_selected->switchState();
        }
      }
      else {
        // Trying to deselect the already selected button
        if (_selected == a_ctxt)
          _selected->switchState();
      }

    });
  }
}


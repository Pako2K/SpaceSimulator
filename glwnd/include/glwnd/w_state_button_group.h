#ifndef W_STATE_BUTTON_GROUP_H
#define W_STATE_BUTTON_GROUP_H

#include <set>

#include <glwnd/w_state_button.h>


namespace glwnd
{

  class WStateButtonGroup
  {
  public:
    // Constructor adding at least 2 buttons, one selected.
    WStateButtonGroup(WStateButton& button_selected, WStateButton& button_deselected);
    WStateButtonGroup(const WStateButtonGroup&) = delete;
    WStateButtonGroup(WStateButtonGroup&&) = delete;
    WStateButtonGroup& operator=(const WStateButtonGroup&) = delete;
    WStateButtonGroup& operator=(WStateButtonGroup&&) = delete;
    ~WStateButtonGroup() {};

    // Add a button to the group
    void add(WStateButton& button); 

    // Returns the curremtly selected button or nullptr
    WStateButton* getSelected() const { return _selected; }


  private:
    std::set<WStateButton*>  _group;

    WStateButton*            _selected{ nullptr };
  };

}






#endif // W_STATE_BUTTON_GROUP_H

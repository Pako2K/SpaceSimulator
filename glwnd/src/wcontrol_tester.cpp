#include <glwnd/core/w_object.h>
#include <glwnd/core/w_container.h>
#include <glwnd/core/w_control.h>
#include <iostream>

using namespace glboost;
using namespace glwnd;


class WCont_TESTER_1 : public WControl
{
public:
  WCont_TESTER_1(Position2D a_pos, Size2D a_size) : WControl{ a_pos, a_size, true } {}
  WCont_TESTER_1(WContainer& parent, Position2D a_pos, Size2D a_size) : WControl{ parent, a_pos, a_size, true } {}

  using WObject::show;
  using WObject::hide;


protected:
  virtual void draw() { std::cout << "CONTROL DRAWN!\n"; }
  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {
    std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS BUTTON CLICKED!\n";
  };

  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};
};


class WCONT_TESTER : public WContainer
{
public:
  WCONT_TESTER(glboost::Position2D a_pos, glboost::Size2D a_size) : WContainer(a_pos, a_size, true) {}
  WCONT_TESTER(WContainer& parent, glboost::Position2D a_pos, glboost::Size2D a_size) : WContainer(parent, a_pos, a_size, true) {}

  using WContainer::size;
  using WContainer::position;
  using WObject::show;
  using WObject::hide;

private:
  virtual void drawContainer() override { std::cout << "CONTAINER DRAWN!\n"; }
  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {
    std::cout << "CONTAINER AT x = " << WObject::position().x() << " GETS BUTTON CLICKED!\n";
  };

  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};
};

bool WCONTROL_TEST_1() {
  
  // Should fail, since there is no main container
  try {
    WCont_TESTER_1 my_control(Position2D(7, 5), Size2D(200, 80));
    return false;
  }
  catch (...) {

  }
  
  WCONT_TESTER my_cont(Position2D(10, 10), Size2D(1000, 500));

  std::cout << &my_cont.mainContainer() << "\n";
  std::cout << &my_cont.focusObj() << "\n";

  WCont_TESTER_1 myobj(Position2D(500, 400), Size2D(10, 20));
  

  WCONT_TESTER my_cont_2(Position2D(100, 100), Size2D(200, 200));

  WCont_TESTER_1 myobj2(my_cont_2, Position2D(5, 4), Size2D(10, 20));

  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(106, 106));
  myobj2.hide();

  my_cont.objDraw();

  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(501, 401));
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(101, 101));
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(106, 106));

  // Grandchild container
  WCONT_TESTER my_cont_3(my_cont_2, Position2D(60, 60), Size2D(20, 20));

  my_cont.objDraw();
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(166, 166));


  // Test position, size change
  my_cont_2.position(Position2D(80, 80));
  my_cont_2.size(Size2D(180, 180));

  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(81, 81));
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(261, 261));
  myobj2.show();
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(86, 86));

  // Copy and move constructors
  WCont_TESTER_1 myobj3(myobj2);


  // Destructor
  {
    WCont_TESTER_1 myobj4(Position2D(51, 41), Size2D(20, 20));
    my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(1, 1));
    my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(52, 52));
  }
  // Should not "reach" any object...
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(52, 52));

  return true;
}
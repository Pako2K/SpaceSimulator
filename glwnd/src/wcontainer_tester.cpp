#include <glwnd/core/w_object.h>
#include <glwnd/core/w_container.h>
#include <iostream>

using namespace glboost;
using namespace glwnd;


class WCONT_TESTER : public WContainer
{
public:
  WCONT_TESTER(glboost::Position2D a_pos, glboost::Size2D a_size) : WContainer{ a_pos, a_size, true } {}
  WCONT_TESTER(WContainer& parent, glboost::Position2D a_pos, glboost::Size2D a_size) : WContainer{ parent, a_pos, a_size, true } {}

  using WContainer::size;
  using WContainer::position;
  using WObject::show;
  using WObject::hide;

private:
  virtual void drawContainer() override { std::cout << "CONTAINER DRAWN!\n"; }
  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {
    std::cout << "CONTAINER AT x = " << WObject::position().x() << " GETS BUTTON CLICKED!\n";
  };

  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};

};


class WOBJ_TESTER_1 : public WObject
{
public:
  WOBJ_TESTER_1(glboost::Position2D a_pos, glboost::Size2D a_size) : WObject(a_pos, a_size) {}

  using WObject::show;
  using WObject::hide;
  using WObject::position;

protected:
  virtual void draw() { std::cout << "OBJ DRAWN!\n"; }

  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {
    std::cout << "OBJECT AT x = " << position().x() << " GETS BUTTON CLICKED!\n";
  };
  
  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};

};



bool WCONTAINER_TEST_1() {
  WCONT_TESTER my_cont(Position2D(10,10), Size2D(1000,500));
   
  std::cout << &my_cont.mainContainer() << "\n";
  std::cout << &my_cont.focusObj() << "\n";
  
  WOBJ_TESTER_1 myobj(Position2D(500, 400), Size2D(10, 20));
  WOBJ_TESTER_1 myobj2(Position2D(1500, 400), Size2D(10, 20));
  WOBJ_TESTER_1 myobj3(Position2D(600, 400), Size2D(10, 200));
  WOBJ_TESTER_1 myobj4(Position2D(1500, 400), Size2D(10, 20));

  my_cont.addChild(myobj, true);
  my_cont.addChild(myobj2, true);
  my_cont.addChild(myobj3, true);
  my_cont.addChild(myobj4, true);
  
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(601, 410));
  myobj3.position(Position2D(800, 100));
  my_cont.updateChild(myobj3);
  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(601, 410));

  my_cont.objDraw();

  my_cont.size(Size2D(600, 450));

  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(501, 410));

  myobj.hide();

  my_cont.objDraw();

  my_cont.hide();

  my_cont.objDraw();

  my_cont.event_dispatcher.buttonClicked(BUTTON_8, 0, Position2D(501, 410));

  return true;

}


bool WCONTAINER_TEST_2() {
  WCONT_TESTER my_cont(Position2D(10, 10), Size2D(1000, 500));
  
  std::cout << &my_cont.mainContainer() << "\n";
  std::cout << &my_cont.focusObj() << "\n";

  WOBJ_TESTER_1 myobj(Position2D(500, 400), Size2D(10, 20));

  my_cont.addChild(myobj, false);

  WCONT_TESTER my_cont_2(Position2D(100, 100), Size2D(200, 200));
  
  WOBJ_TESTER_1 myobj2(Position2D(5, 4), Size2D(10, 20));

  my_cont_2.addChild(myobj2, true);

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

  return true;

}
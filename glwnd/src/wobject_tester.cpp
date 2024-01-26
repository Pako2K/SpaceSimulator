#include <glwnd/core/w_object.h>
#include <iostream>
#include <vector>


using namespace glboost;
using namespace glwnd;


class WOBJ_TESTER_1 : public WObject
{
public:
  WOBJ_TESTER_1(glboost::Position2D a_pos, glboost::Size2D a_size) : WObject(a_pos, a_size) {}

  using WObject::show;
  using WObject::hide;
  

protected:
  virtual void draw() { std::cout << "DRAWN!\n"; }

  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};
};



bool WOBJECT_TEST_1() {
  WOBJ_TESTER_1 myobj(Position2D(7,5), Size2D(200,80));

  myobj.objDraw();
  
  myobj.hide();
  std::cout << "Object hidden! " << myobj.isVisible() << "\n";
  myobj.objDraw();
  
  WOBJ_TESTER_1 myobj2(myobj);
  myobj2.objDraw();
  
  myobj.show();
  std::cout << "Object shown! " << myobj.isVisible() << "\n";
  myobj.objDraw();

  myobj2.objDraw();
  myobj2.show();
  myobj2.objDraw();

  WOBJ_TESTER_1 myobj3(std::move(myobj2));
  WOBJ_TESTER_1 myobj4(std::move(WOBJ_TESTER_1(Position2D(70, 50), Size2D(20, 3))));
  
//  myobj2.show();

  std::vector<WOBJ_TESTER_1> v;
  v.reserve(3);
  v.push_back(WOBJ_TESTER_1(Position2D(70, 50), Size2D(20, 3)));
  v.push_back(myobj);

  return true;
  
}
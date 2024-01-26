#include <glwnd/core/w_main_window_fs.h>
#include <glwnd/core/w_control.h>
#include <glwnd/core/event_listener_set.h>


#include <iostream>

#include <glboost/g_rectangle.h>


using namespace glboost;
using namespace glwnd;


class MainWindow : public WMainWindowFS
{
public:
  MainWindow() : WMainWindowFS{} {}
  

protected:
  virtual void drawMainWindow() override { std::cout << "MAIN WINDOW DRAWN!\n"; }

  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override {};
  virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override {};
  virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
  virtual void mouseMoved(const glboost::Position2D& pos)  override {};
};


class MainWindow2 : public WMainWindowFS
{
public:
  MainWindow2() : WMainWindowFS{} {

    rec = new gRectangle(Position2D(200, 800), Size2D(100, 100));
    rec->borderColor(Color4f(1, 0, 0.1f, 1));

    std::cout << "MainWindow2 CREATED.\n";
  }


protected:
  float i{ 0 };
  gRectangle* rec;
  virtual void drawMainWindow() override { rec->draw(); }
  virtual void eventLoopIteration() override { 
    i+=0.001f;
    rec->borderColor(Color4f(float(cos(i)), float(sin(i)), 0.1f, 1)); //background(Color4f(cos(i), sin(i), 0.1, 1));
  }

  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS buttonClicked!\n"; };
  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};

  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS buttonReleased!\n"; };

  virtual void buttonDoubleClicked(const glboost::Position2D& pos) override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS buttonDoubleClicked!\n"; };

  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS scrolled!\n"; };

  virtual void dragged(MouseButton button, const glboost::Position2D& pos) override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS dragged!\n"; };

  virtual void mouseMoved(const glboost::Position2D& pos)override { std::cout << "MainWindow2 AT x = " << WObject::position().x() << " GETS mouseMoved!\n"; };

  virtual void keyPressed(int key, int scancode, int modifiers) override { std::cout << "MainWindow2  GETS keyPressed!\n"; }

  virtual void keyRepeated(int key, int scancode, int modifiers) override { std::cout << "MainWindow2  GETS keyRepeated!\n"; }

  virtual void keyReleased(int key, int scancode, int modifiers) override { std::cout << "MainWindow2  GETS keyReleased!\n"; }
  
  
  
};



class WMyCont : public WControl
{
  // Listeners will receive only the button id
  USE_LISTENERS(buttonClicked, int);
public:
  WMyCont(Position2D a_pos, Size2D a_size) : WControl{ a_pos, a_size, true } {
    rec = std::make_unique<gRectangle>(_parent.absPosition() + a_pos, a_size);
    rec->borderColor(Color4f(0.6f, 0.5f, 0.1f, 1));
    rec->fillColor(Color4f(1, 1, 1, 1));
    rec->fill(true);
  }
  //WMyCont(WContainer& parent, Position2D a_pos, Size2D a_size) : WControl{ parent, a_pos, a_size, true } {}

  using WObject::show;
  using WObject::hide;


protected:
  std::unique_ptr<gRectangle> rec;

  virtual void draw() { rec->draw(); }
  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override {
    std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS BUTTON CLICKED!\n";
    CALL_LISTENERS(buttonClicked, button);
  };
  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override { std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS buttonReleased!\n"; };

  virtual void buttonDoubleClicked(const glboost::Position2D& pos) override { std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS buttonDoubleClicked!\n"; };

  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override { std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS scrolled!\n"; };

  virtual void dragged(MouseButton button, const glboost::Position2D& pos) override { std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS dragged!\n"; };

  virtual void mouseMoved(const glboost::Position2D& pos)override { std::cout << "CONTROL AT x = " << WObject::position().x() << " GETS mouseMoved!\n"; };

  virtual void buttonRepeatClicked(const glboost::Position2D& pos) {};
};


class MainWindow3 : public WMainWindowFS
{
public:
  MainWindow3() : WMainWindowFS{} {

    rec = std::make_unique<gRectangle>(Position2D(200, 800), Size2D(100, 100));
    rec->borderColor(Color4f(1, 0, 0.1f, 1));

    cont.addListener_buttonClicked(rec.get(), onContClicked);

    std::cout << "MainWindow3 CREATED.\n";
  }

  static void onContClicked(void* rec_ptr, int button) {
    std::cout << "onContClicked invoked.\n";
    ((gRectangle*)rec_ptr)->lineWidth(((gRectangle*)rec_ptr)->lineWidth() + 1);
  }

protected:
  float i{ 0 };
  std::unique_ptr<gRectangle> rec;
  WMyCont cont{ Position2D(1200, 80), Size2D(100, 50) };
  virtual void drawMainWindow() override { rec->draw(); }
  virtual void eventLoopIteration() override {
    i += 0.001f;
    rec->borderColor(Color4f(float(cos(i)), float(sin(i)), 0.1f, 1)); 
  }

  virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS buttonClicked!\n"; };

  virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};

  virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS buttonReleased!\n"; };

  virtual void buttonDoubleClicked(const glboost::Position2D& pos) override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS buttonDoubleClicked!\n"; };

  virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS scrolled!\n"; };

  virtual void dragged(MouseButton button, const glboost::Position2D& pos) override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS dragged!\n"; };

  virtual void mouseMoved(const glboost::Position2D& pos)override { std::cout << "MainWindow3 AT x = " << WObject::position().x() << " GETS mouseMoved!\n"; };

  virtual void keyPressed(int key, int scancode, int modifiers) override { std::cout << "MainWindow3  GETS keyPressed!\n"; }

  virtual void keyRepeated(int key, int scancode, int modifiers) override { std::cout << "MainWindow3  GETS keyRepeated!\n"; }

  virtual void keyReleased(int key, int scancode, int modifiers) override { std::cout << "MainWindow3  GETS keyReleased!\n"; }
};






bool MAIN_WINDOW_TEST_1() {
  // Should fail since window is not initialized
  try {
    MainWindow wnd;
    return false;
  }
  catch (std::runtime_error& ) {}

  MainWindow::contextVersion(5, 1);
  MainWindow::samples(10);

  MainWindow::initialize();
  MainWindow::contextVersion(4, 0);
  MainWindow::samples(0);

  {
    MainWindow wnd;

    try {
      MainWindow wnd2;
      return false;
    }
    catch (std::logic_error&) {}


    // Should fail since window is already initialized
    try {
      //MainWindow::contextVersion(4, 1);
      MainWindow::samples(0);
      return false;
    }
    catch (std::logic_error&) {}
  }

  MainWindow::terminate();

  // Should fail since window is not initialized
  try {
    MainWindow wnd;
    return false;
  }
  catch (std::runtime_error&) {}

  MainWindow::initialize();
  MainWindow::contextVersion(4, 0);
  MainWindow::samples(0);
  {
    MainWindow wnd;

    wnd.swapInterval(3);
    wnd.background(Color4f(0, 0.2f, 0, 1));

    wnd.run();
  }
  MainWindow::terminate();

  return true;
}




bool MAIN_WINDOW_TEST_2() {
  MainWindow2::contextVersion(4, 1);
  MainWindow2::samples(1);

  MainWindow2::initialize();

  {
    MainWindow2 wnd;
    
    wnd.swapInterval(1);
    wnd.background(Color4f(0, 0.2f, 0, 1));

    wnd.run();
  }

  MainWindow2::terminate();


  return true;
}





bool MAIN_WINDOW_TEST_3() {
 
  MainWindow3::initialize();
  MainWindow3::contextVersion(4, 1);
  MainWindow3::samples(4);
  {
    MainWindow3 wnd;

    wnd.swapInterval(1);
    wnd.background(Color4f(0, 0.2f, 0, 1));

    wnd.run();
  }

  MainWindow3::terminate();


  return true;
}
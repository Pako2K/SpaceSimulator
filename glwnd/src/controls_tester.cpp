#include <glwnd/core/w_main_window_fs.h>
#include <glwnd/core/w_control.h>
#include <glwnd/core/event_listener_set.h>
#include <glwnd/w_label.h>
#include <glwnd/w_state_button.h>
#include <glwnd/w_icon_button.h>
#include <glwnd/w_state_button_group.h>
#include <glwnd/w_list.h>
#include <glwnd/w_button_rwd.h>

#include <iostream>

#include <glboost/g_rectangle.h>


using namespace glboost;
using namespace glwnd;




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

    cont.addListener_buttonClicked(this, onContClicked);

    std::cout << "MainWindow3 CREATED.\n";

    _lbl = std::make_unique<glwnd::WLabel>("HELLO WORLD", 100,20,200,40);
//    WLabel lbl2(WLabel("HELLO WORLD", 100, 20, 200, 40));
    _btn = std::make_unique<glwnd::WStateButton>(100, 200, 50, 50, "img/play_blue.png", "img/pause_blue.png");
    _btn->color(Color4f(0.5, 0.5, 0.5, 1));

    _btn->addListener_ButtonSelected(rec.get(), [](void *rec_ptr) {
      std::cout << "ButtonSelected invoked.\n";
      ((gRectangle*)rec_ptr)->lineWidth(((gRectangle*)rec_ptr)->lineWidth() - 1);
    }
    );
    _btn->addListener_ButtonStateChanged(nullptr, [](void *a_ctxt, ButtonState new_state) {
      std::cout << "Button state is: " << new_state << ".\n";
    }
    );
       
    _normal_btn.addListener_ButtonClicked(rec.get(), [](void *rec_ptr) {
      std::cout << "Button clicked.\n";
      ((gRectangle*)rec_ptr)->lineWidth(((gRectangle*)rec_ptr)->lineWidth() + 1);
    }
    );

    _btn1 = std::make_unique<glwnd::WStateButton>(1000, 200, 50, 50, "img/play_blue.png", "img/pause_blue.png");
    _btn2 = std::make_unique<glwnd::WStateButton>(1000, 300, 50, 50, "img/play_blue.png", "img/pause_blue.png");
    _btn3 = std::make_unique<glwnd::WStateButton>(1000, 400, 50, 50, "img/play_blue.png", "img/pause_blue.png");

    _btn_grp = std::make_unique<glwnd::WStateButtonGroup>(*_btn1, *_btn2);
    _btn_grp->add(*_btn3);


    WList::elementBackgroundColor(Color4f(0.2f, 0, 1, 1));
    WList::backgroundColor(Color4f(0.2f, 0, 0.8f, 1));
    WList::borderColor(Color4f(0, 0, 0, 1));
    WList::elementBorderColor(Color4f(0, 0, 0, 0));
    WList::fontColor(Color4f(1, 1, 0, 1));
    WList::fontPreSelColor(Color4f(1, 0, 0, 1));
    WList::selectedBorderColor(Color4f(0, 0, 0, 1));;
    _list = std::make_unique<glwnd::WList>(2000,300,500,450,false);
    for (int i = 1; i <= 6500; i++)
      _list->add("str position  -  " + std::to_string(i));

    
    _lbl2 = std::make_unique<glwnd::WLabel>("HELLO WORLD 2", 3100, 820, 200, 40);
    _lbl2->fontColor(Color4i(255,255,255, 255));
    _lbl2->borderColor(Color4i(100, 0, 0, 255));

    _lbl3 = std::make_unique<glwnd::WLabel>("HELLO WORLD 3", 3100, 1120, 200, 40);
    _lbl3->fontColor(Color4i(255, 255, 255, 255));
    _lbl3->borderColor(Color4i(100, 0, 0, 255));

    _lbl4 = std::make_unique<glwnd::WLabel>("HELLO WORLD 4", 3100, 1320, 200, 40);
    _lbl4->alignHor(HorAlignmentEnum::ALIGN_RIGHT);
    _lbl4->alignVer(VerAlignmentEnum::ALIGN_VERT_CENTER);
    _lbl4->fontColor(Color4i(255, 255, 255, 255));
    _lbl4->borderColor(Color4i(100, 0, 0, 255));


    _rwd_btn = std::make_unique<glwnd::WRewdButton>(Position2D(1040, 1040), Size2D(200, 200));
    _rwd_btn->borderColor(Color4f(1,1,1,1));
    _rwd_btn->backgroundColor(Color4f(0.2f,0,0,1));
    _rwd_btn->frontColor(Color4f(0.4f,1,1,1));
  }

  static void onContClicked(void* wnd_ptr, int button) {
    std::cout << "onContClicked invoked.\n";
    gRectangle &rec = *((MainWindow3*)wnd_ptr)->rec;
    rec.lineWidth(rec.lineWidth() + 1);
    auto& lbl = *((MainWindow3*)wnd_ptr)->_lbl2;
    lbl.size(Size2D(300, 100));
    auto& lbl2 = *((MainWindow3*)wnd_ptr)->_lbl3;
    lbl2.position(Position2D(3600, 1200));
    auto& lbl3 = *((MainWindow3*)wnd_ptr)->_lbl4;
    lbl3.position(Position2D(3600, 1400));
    lbl.size(Size2D(300, 80));

    auto& list = *((MainWindow3*)wnd_ptr)->_list;
    list.clear();
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

private:
  std::unique_ptr<glwnd::WLabel> _lbl;
  std::unique_ptr<glwnd::WLabel> _lbl2;
  std::unique_ptr<glwnd::WLabel> _lbl3;
  std::unique_ptr<glwnd::WLabel> _lbl4;
  std::unique_ptr<glwnd::WStateButton> _btn;
  std::unique_ptr<glwnd::WStateButton> _btn1;
  std::unique_ptr<glwnd::WStateButton> _btn2;
  std::unique_ptr<glwnd::WStateButton> _btn3;
  std::unique_ptr<glwnd::WStateButtonGroup> _btn_grp;
  std::unique_ptr<glwnd::WList> _list;
  std::unique_ptr<glwnd::WRewdButton> _rwd_btn;


  WIconButton _normal_btn{ Position2D(500, 200), Size2D(50, 50), "img/play_blue.png", "img/pause_blue.png" };
};






bool LABEL_BUTTON_1() {

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
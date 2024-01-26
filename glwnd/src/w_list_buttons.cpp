#include <glwnd/w_list.buttons.h>

using namespace glwnd;
using namespace glboost;

void WList::LabelSEButton::initializeButton(bool to_start) {
  _button_shape = std::make_unique<gRectangle>(position(), size());
  SizePxl triangle_width{ SizePxl(3 * size().w() / 8) };
  SizePxl bar_width{ SizePxl(size().w() / 16) };
  SizePxl bar_height{ SizePxl(size().h() / 2) };
  PositionPxl left_x{ position().x() + size().w() / 4 };
  PositionPxl tri_left_x{ left_x + 2 * bar_width };
  PositionPxl top_y{ position().y() + size().h() / 4 };
  PositionPxl bottom_y{ top_y + bar_height };
  PositionPxl middle_y{ position().y() + size().h() / 2 };
  if (to_start) {
    _vert_bar = std::make_unique<gRectangle>(Position2D(left_x, top_y), Size2D(bar_width, bar_height));
    _arrow = std::make_unique<gTriangle>(Position2D(tri_left_x, middle_y), Position2D(tri_left_x + triangle_width, top_y), Position2D(tri_left_x + triangle_width, bottom_y));
  }
  else {
    _vert_bar = std::make_unique<gRectangle>(Position2D(position().x() + 3 * size().w() / 4 - bar_width, top_y), Size2D(bar_width, bar_height));
    _arrow = std::make_unique<gTriangle>(Position2D(_vert_bar->position().x() - bar_width, middle_y), Position2D(left_x, top_y), Position2D(left_x, bottom_y));
  }

  _button_shape->fill(true);
  _vert_bar->fill(true);
  _vert_bar->border(false);
  _arrow->fill(true);
  _arrow->border(false);
  _vert_bar->fillColor(_front_color);
  _arrow->fillColor(_front_color);
}


void WList::LabelSEButton::borderColor(const glboost::Color4f& new_color) {
  _button_shape->borderColor(new_color);
}


void WList::LabelSEButton::backgroundColor(const glboost::Color4f& new_color) {
  _button_shape->fillColor(new_color);
}


void WList::LabelSEButton::frontColor(const glboost::Color4f& new_color) {
  _front_color = new_color;
  _vert_bar->fillColor(new_color);
  _arrow->fillColor(new_color);
}


void WList::LabelSEButton::customButtonPressed() {
  Color4f front_color{ (_button_shape->fillColor().r() + _front_color.r()) / 3,
                      (_button_shape->fillColor().g() + _front_color.g()) / 3,
                      (_button_shape->fillColor().b() + _front_color.b()) / 3,
                      (_button_shape->fillColor().a() + _front_color.a()) / 3 };
  auto border_color{ _button_shape->borderColor() };
  border_color.a() = 0.3f;
  _button_shape->borderColor(border_color);
  _vert_bar->fillColor(front_color);
  _arrow->fillColor(front_color);
}


void WList::LabelSEButton::customButtonReleased() {
  auto border_color{ _button_shape->borderColor() };
  border_color.a() = 1.0f;
  _button_shape->borderColor(border_color);
  _vert_bar->fillColor(_front_color);
  _arrow->fillColor(_front_color);
}


void WList::LabelSEButton::draw() {
  // Draw background
  _button_shape->draw();
  _vert_bar->draw();
  _arrow->draw();
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void WList::LabelPagingButton::initializeButton(bool go_back) {
  _button_shape = std::make_unique<gRectangle>(position(), size());
  SizePxl triangle_width{ SizePxl(3 * size().w() / 8) };
  SizePxl triangle_height{ SizePxl(size().h() / 2) };
  PositionPxl left_x1{ position().x() + 3 * size().w() / 32 };
  PositionPxl left_x2{ left_x1 + triangle_width + size().w() / 16 };
  PositionPxl top_y{ position().y() + size().h() / 4 };
  PositionPxl bottom_y{ top_y + triangle_height };
  PositionPxl middle_y{ position().y() + size().h() / 2 };
  if (go_back) {
    _arrows[0] = std::make_unique<gTriangle>(Position2D(left_x1, middle_y), Position2D(left_x1 + triangle_width, top_y), Position2D(left_x1 + triangle_width, bottom_y));
    _arrows[1] = std::make_unique<gTriangle>(Position2D(left_x2, middle_y), Position2D(left_x2 + triangle_width, top_y), Position2D(left_x2 + triangle_width, bottom_y));
  }
  else {
    _arrows[0] = std::make_unique<gTriangle>(Position2D(left_x1, top_y), Position2D(left_x1, bottom_y), Position2D(left_x1 + triangle_width, middle_y));
    _arrows[1] = std::make_unique<gTriangle>(Position2D(left_x2, top_y), Position2D(left_x2, bottom_y), Position2D(left_x2 + triangle_width, middle_y));
  }

  _button_shape->fill(true);
  _arrows[0]->fill(true);
  _arrows[1]->fill(true);
  _arrows[0]->border(false);
  _arrows[1]->border(false);
  _arrows[0]->fillColor(_front_color);
  _arrows[1]->fillColor(_front_color);
}


void WList::LabelPagingButton::borderColor(const glboost::Color4f & new_color) {
  _button_shape->borderColor(new_color);
}


void WList::LabelPagingButton::backgroundColor(const glboost::Color4f & new_color) {
  _button_shape->fillColor(new_color);
}


void WList::LabelPagingButton::frontColor(const glboost::Color4f & new_color) {
  _front_color = new_color;
  _arrows[0]->fillColor(new_color);
  _arrows[1]->fillColor(new_color);
}


void WList::LabelPagingButton::customButtonPressed() {
  Color4f front_color{ (_button_shape->fillColor().r() + _front_color.r()) / 3,
                      (_button_shape->fillColor().g() + _front_color.g()) / 3,
                      (_button_shape->fillColor().b() + _front_color.b()) / 3,
                      (_button_shape->fillColor().a() + _front_color.a()) / 3 };
  auto border_color{ _button_shape->borderColor() };
  border_color.a() = 0.3f;
  _button_shape->borderColor(border_color);
  _arrows[0]->fillColor(front_color);
  _arrows[1]->fillColor(front_color);
}


void WList::LabelPagingButton::customButtonReleased() {
  auto border_color{ _button_shape->borderColor() };
  border_color.a() = 1.0f;
  _button_shape->borderColor(border_color);
  _arrows[0]->fillColor(_front_color);
  _arrows[1]->fillColor(_front_color);
}


void WList::LabelPagingButton::draw() {
  // Draw background
  _button_shape->draw();
  _arrows[0]->draw();
  _arrows[1]->draw();
}
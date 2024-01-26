#include <glwnd/w_label.h>

using namespace glwnd;
using namespace glboost;


WLabel::WLabel(const std::string& text, const Position2D& pos, const Size2D& size) : WControl{ pos, size, false } {
  initialize(text, pos, size);
}


WLabel::WLabel(const std::string& text, PositionPxl pos_x, PositionPxl pos_y, SizePxl width, SizePxl height) : WLabel{ text, Position2D{ pos_x, pos_y }, Size2D{ width, height } } {}


WLabel::WLabel(WContainer& parent, const std::string& text, const Position2D& pos, const Size2D& size) : WControl{ parent, pos, size, false } {
  initialize(text, pos, size);
}


WLabel::WLabel(WContainer& parent, const std::string& text, PositionPxl pos_x, PositionPxl pos_y, SizePxl width, SizePxl height) : WLabel{ parent, text, Position2D{ pos_x, pos_y }, Size2D{ width, height } } {}


void WLabel::initialize(const std::string& text, const Position2D& pos, const Size2D& size) {
  /*WControl::position(pos);
  WControl::size(size);*/

  // Default alignment left and bottom
  _text = std::make_unique<Text>(text, pos.x() + _HOR_MARGIN, pos.y() + size.h() - _VER_MARGIN);

  // Initiate background
  _bkg_rect = std::make_unique<gRectangle>(pos, size);
  _bkg_rect->fill(true);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Scissor box coordinates (viewport coordinates)
  _scissor_coord[0] = pos.x() + _HOR_MARGIN;  // x
  _scissor_coord[1] = viewport[3] - pos.y() - size.h() + _VER_MARGIN;   // y
  _scissor_coord[2] = size.w() - 2 * _HOR_MARGIN;  // width
  _scissor_coord[3] = size.h() - 2 * _VER_MARGIN;   // height
}


void WLabel::position(const glboost::Position2D& new_pos) {
  _bkg_rect->position(new_pos);
  // Move the text by the difference, to maintain the selected alignment
  _text->position(_text->position().x() + new_pos.x() - _position.x(), _text->position().y() + new_pos.y() - _position.y());

  WControl::position(new_pos);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  _scissor_coord[0] = new_pos.x() + _HOR_MARGIN;  // x
  _scissor_coord[1] = viewport[3] - new_pos.y() - _size.h() + _VER_MARGIN;   // y
  _scissor_coord[2] = _size.w() - 2 * _HOR_MARGIN;  // width
  _scissor_coord[3] = _size.h() - 2 * _VER_MARGIN;   // height
}


void WLabel::size(const glboost::Size2D& new_size) {
  WControl::size(new_size);
  _bkg_rect->size(new_size);
      
  textPositionHor();
  textPositionVer();
    
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  _scissor_coord[0] = _position.x() + _HOR_MARGIN;  // x
  _scissor_coord[1] = viewport[3] - _position.y() - _size.h() + _VER_MARGIN;   // y
  _scissor_coord[2] = _size.w() - 2 * _HOR_MARGIN;  // width
  _scissor_coord[3] = _size.h() - 2 * _VER_MARGIN;   // height
}


void WLabel::text(const std::string& new_text) {
  _text->text(new_text);
  textPositionHor();
}


void WLabel::font(const std::string& new_name, glboost::SizePxl new_size) {
  _text->font(new_name, new_size);
  textPositionHor();
  textPositionVer();
}


void WLabel::fontName(const std::string& new_name) {
  _text->font(new_name);
  textPositionHor();
}


void WLabel::fontSize(glboost::SizePxl new_size) {
  _text->font(new_size);
  textPositionHor();
  textPositionVer();
}


void WLabel::alignHor(HorAlignmentEnum alignment) {
  _hor_alignment = alignment;
  textPositionHor();
}


void WLabel::alignVer(VerAlignmentEnum alignment) {
  _ver_alignment = alignment;
  textPositionVer();
}


void WLabel::draw() {
  // Draw background
  _bkg_rect->draw();

  // For the rectangle area within the margins set the stencil buffer pixels to 1, discarding all the fragments 
  glEnable(GL_SCISSOR_TEST);
  glScissor(_scissor_coord[0], _scissor_coord[1], _scissor_coord[2], _scissor_coord[3]);

  //// Draw Text
  _text->show();
  glDisable(GL_SCISSOR_TEST);
}


void WLabel::textPositionHor() {
  // Recalculate text position when the horizontal alignement is not left
  PositionPxl pos_x;
  if (_hor_alignment != ALIGN_LEFT) {
    if (_hor_alignment == ALIGN_HOR_CENTER)
      pos_x = position().x() + (size().w() - _text->sizePxl()) / 2;
    else
      pos_x = position().x() + size().w() - _HOR_MARGIN - _text->sizePxl();
  }
  else
    pos_x = position().x() + _HOR_MARGIN;

  _text->position(pos_x, _text->position().y());
}


void WLabel::textPositionVer() {
  // Recalculate text position when the vertical alignement is not bottom
  PositionPxl pos_y;
  if (_ver_alignment != ALIGN_BOTTOM) {
    if (_ver_alignment == ALIGN_VERT_CENTER)
      pos_y = position().y() + (size().h() + _text->fontSize()) / 2;
    else
      pos_y = position().y() + _VER_MARGIN + _text->fontSize();
  }
  else
    pos_y = position().y() + size().h() - _VER_MARGIN;

  _text->position(_text->position().x(), pos_y);
}
#include <glwnd/w_list.h>
#include <glwnd/w_list.buttons.h>


using namespace glwnd;
using namespace glboost;


WList::WList(const Position2D& pos, const Size2D& size, bool multi_selection) 
  : WControl{ pos, Size2D(size.w(), 2 + ((size.h() - ELEMENT_HEIGHT) / ELEMENT_DISTANCE) * ELEMENT_DISTANCE - __def_label_spacing), true },
  _multi_selection{ multi_selection }, _rectangle{ pos, Size2D(this->_size.w(), this->_size.h() + ELEMENT_HEIGHT) }, _max_visible_elems{ size_t((__def_label_spacing + this->_size.h()) / ELEMENT_DISTANCE) } {
  initialize();
}


WList::WList(PositionPxl pos_x, PositionPxl pos_y, SizePxl width, SizePxl height, bool multi_selection) : WList{ Position2D(pos_x, pos_y), Size2D(width,height), multi_selection } {}


WList::WList(WContainer& parent, const Position2D& pos, const Size2D& size, bool multi_selection) 
  : WControl{ parent, pos, Size2D(size.w(), 2 + ((size.h() - ELEMENT_HEIGHT) / ELEMENT_DISTANCE) * ELEMENT_DISTANCE - __def_label_spacing), true },
  _multi_selection{ multi_selection }, _rectangle{ pos, Size2D(this->_size.w(), this->_size.h() + ELEMENT_HEIGHT) }, _max_visible_elems{ size_t((__def_label_spacing + this->_size.h()) / ELEMENT_DISTANCE) } {
  initialize();
}


WList::WList(WContainer& parent, PositionPxl pos_x, PositionPxl pos_y, SizePxl width, SizePxl height, bool multi_selection) : WList{ parent, Position2D(pos_x, pos_y), Size2D(width,height), multi_selection } {};


WList::~WList() {}


void WList::initialize() {
  _rectangle.borderColor(_BORDER_COLOR);
  _rectangle.fillColor(_BACKGROUND_COLOR);
  _rectangle.fill(true);

  SizePxl btn_height{ static_cast<SizePxl>(_ELEMENT_HEIGHT - 1)};
  _btn_scroll_start = std::make_unique<LabelSEButton>(this->_parent, Position2D(this->_position.x() + 2, this->_position.y() + this->_size.h()), Size2D(btn_height, btn_height), true);
  _btn_scroll_end = std::make_unique<LabelSEButton>(this->_parent, Position2D(this->_position.x() + this->_size.w() - btn_height - 2, _btn_scroll_start->position().y()), Size2D(btn_height, btn_height), false);
  _btn_scroll_up = std::make_unique<LabelPagingButton>(this->_parent, Position2D(this->_position.x() + 2 + 5 * btn_height / 4, _btn_scroll_start->position().y()), Size2D(btn_height, btn_height), true);
  _btn_scroll_down = std::make_unique<LabelPagingButton>(this->_parent, Position2D(this->_position.x() + this->_size.w() - 9 * btn_height / 4 - 2, _btn_scroll_start->position().y()), Size2D(btn_height, btn_height), false);
  
  Color4f btn_front_color{ 1, 1, 1, 1 };
  if ((_BACKGROUND_COLOR.r() + _BACKGROUND_COLOR.g() + _BACKGROUND_COLOR.b()) / 3 > 0.5)
    btn_front_color = Color4f(0, 0, 0, 1);

  _btn_scroll_start->frontColor(btn_front_color);
  _btn_scroll_end->frontColor(btn_front_color);
  _btn_scroll_up->frontColor(btn_front_color);
  _btn_scroll_down->frontColor(btn_front_color);

  _btn_scroll_start->borderColor(_BACKGROUND_COLOR);
  _btn_scroll_end->borderColor(_BACKGROUND_COLOR);
  _btn_scroll_up->borderColor(_BACKGROUND_COLOR);
  _btn_scroll_down->borderColor(_BACKGROUND_COLOR);

  _btn_scroll_start->backgroundColor(_BACKGROUND_COLOR);
  _btn_scroll_end->backgroundColor(_BACKGROUND_COLOR);
  _btn_scroll_up->backgroundColor(_BACKGROUND_COLOR);
  _btn_scroll_down->backgroundColor(_BACKGROUND_COLOR);

  _btn_scroll_start->hide();
  _btn_scroll_up->hide();
  _btn_scroll_down->hide();
  _btn_scroll_end->hide();

  _btn_scroll_start->addListener_ButtonClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) }; 
    list.showElementsFrom(1);
  });
  _btn_scroll_up->addListener_ButtonClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) };
    if (list._top_element > list._scroll_pages * list._max_visible_elems)
      list.showElementsFrom(list._top_element - list._scroll_pages * list._max_visible_elems);
    else
      list.showElementsFrom(1);
  });
  _btn_scroll_up->addListener_ButtonRepeatClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) };
    if (list._top_element > list._fast_scroll_pages * list._max_visible_elems)
      list.showElementsFrom(list._top_element - list._fast_scroll_pages * list._max_visible_elems);
    else
      list.showElementsFrom(1);
  });
  _btn_scroll_down->addListener_ButtonClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) };
    list.showElementsFrom(list._top_element + list._scroll_pages * list._max_visible_elems);
  });
  _btn_scroll_down->addListener_ButtonRepeatClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) };
    list.showElementsFrom(list._top_element + list._fast_scroll_pages * list._max_visible_elems);
  });
  _btn_scroll_end->addListener_ButtonClicked(this, [](void* ctxt) {
    WList& list{ *((WList*)ctxt) };
    list.showElementsFrom(list.listSize() - list._max_visible_elems + 1);
  });
}


void WList::showElementsFrom(LabelId top_pos) {
  if (_labels.size() <= _max_visible_elems)
    return;

  // Adjust top_pos if necessary to show at least the max number of elements
  top_pos = std::min(top_pos, (_labels.size() - _max_visible_elems + 1));

  // Hide current visible elements
  for (auto i = _top_element; i < (_top_element + _max_visible_elems); i++) {
    _labels[i]->hide();
  }

  // Reposition and show last elements
  auto start_pos = _labels[_top_element]->position();
  _top_element = top_pos;
  decltype(_top_element) rel_pos{ 0 };
  for (auto i = _top_element; i < (_top_element + _max_visible_elems); i++) {
    _labels[i]->position(Position2D(start_pos.x(), PositionPxl(start_pos.y() + (rel_pos++) * _ELEMENT_DISTANCE)));
    _labels[i]->show();
  }
}


void WList::setScrollFactors() {
  auto num_pages = _lastId / _max_visible_elems;
  _fast_scroll_pages = 1 + num_pages / 300;
  _scroll_pages = 1 + size_t(log10(num_pages));
  _scroll_step = std::min(_scroll_pages, _max_visible_elems);
}


void WList::add(const std::string& text) {
  _labels[++_lastId] = std::make_unique<WLabel>(this->_parent, text, this->_position.x() + 1, this->_position.y() + 1 + glboost::PositionPxl(std::min(_labels.size(), size_t(_max_visible_elems - 1))) * _ELEMENT_DISTANCE, this->_size.w() - 3, _ELEMENT_HEIGHT);
  _labels[_lastId]->backColor(_LABEL_BKG_COLOR);
  _labels[_lastId]->borderColor(_LABEL_BORDER_COLOR);
  _labels[_lastId]->fontColor(_FONT_COLOR);
  _labels[_lastId]->font(_FONT_NAME, _FONT_SIZE);

  if (_lastId > _max_visible_elems) {
    _labels[_lastId]->hide();
    _btn_scroll_start->show();
    _btn_scroll_up->show();
    _btn_scroll_down->show();
    _btn_scroll_end->show();
  }

  setScrollFactors();
}


void WList::update(size_t pos, const std::string& text) {
  if (pos <= _labels.size())
    _labels[pos]->text(text);
  else
    add(text);
}


void WList::pop() {
  _labels.erase(_lastId);
  _selected.erase(_lastId--);
  if (_top_element > (_labels.size() + 1 - _max_visible_elems) && _top_element > 1) {
    --_top_element;
    // Move elements down
    auto i = _top_element + _max_visible_elems - 1;
    for (; i > _top_element; i--) {
      _labels[i]->position(Position2D(_labels[i]->position().x(), _labels[i]->position().y() + _ELEMENT_DISTANCE));
    }
    // The last one simply needs to be visible
    _labels[i]->show();
  }
  if (_lastId <= _max_visible_elems) {
    _btn_scroll_start->hide();
    _btn_scroll_up->hide();
    _btn_scroll_down->hide();
    _btn_scroll_end->hide();
  }

  setScrollFactors();
}


void WList::clear() {
  _labels.clear();
  _selected.clear();
  _lastId = 0;
  _btn_scroll_start->hide();
  _btn_scroll_up->hide();
  _btn_scroll_down->hide();
  _btn_scroll_end->hide();
}


void WList::unselect() {
  for (auto label : _selected) {
    _labels[label]->fontColor(_FONT_COLOR);
    _labels[label]->backColor(_LABEL_BKG_COLOR);
    _labels[label]->borderColor(_LABEL_BORDER_COLOR);
  }

  _selected.clear();
}


std::vector<std::string> WList::selected() const {
  std::vector<std::string> result;
  for(auto id : _selected)
    result.push_back(_labels.at(id)->text());
  
  return result;
}


size_t WList::listSize() const {
  return _labels.size();
}


void WList::buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) {
   if (button != LEFT_BUTTON)
    return;

  // Store position and highlight element
  _candidate = _top_element + pos.y() / _ELEMENT_DISTANCE;
  if (_candidate <= _labels.size() && _candidate >= _top_element && _candidate < _top_element + _max_visible_elems)
    _labels[_candidate]->fontColor(_FONT_PRESEL_COLOR);
  else
    _candidate = 0;
}


void WList::buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) {
  if (button != LEFT_BUTTON || !_candidate)
    return;

  // Select / Unselect element if the position is still within the same element
  if (_candidate == (_top_element + pos.y() / _ELEMENT_DISTANCE)) {
    if (!_multi_selection) {
      if (_selected.size()) {
        auto curr_sel = *_selected.begin();
        _labels[curr_sel]->fontColor(_FONT_COLOR);
        _labels[curr_sel]->backColor(_LABEL_BKG_COLOR);
        _labels[curr_sel]->borderColor(_LABEL_BORDER_COLOR);
        _selected.clear();
        if (curr_sel != _candidate) {
          _labels[_candidate]->fontColor(_LABEL_BKG_COLOR);
          _labels[_candidate]->backColor(_FONT_COLOR);
          _labels[_candidate]->borderColor(_SELECTED_BORDER_COLOR);

          _selected.insert(_candidate);
        }
      }
      else {
        _labels[_candidate]->fontColor(_LABEL_BKG_COLOR);
        _labels[_candidate]->backColor(_FONT_COLOR);
        _labels[_candidate]->borderColor(_SELECTED_BORDER_COLOR);
        _selected.insert(_candidate);
      }
    }
    // Multi-selection
    else {
      if (_selected.find(_candidate) == _selected.end()) {
        _selected.insert(_candidate);
        _labels[_candidate]->fontColor(_LABEL_BKG_COLOR);
        _labels[_candidate]->backColor(_FONT_COLOR);
        _labels[_candidate]->borderColor(_SELECTED_BORDER_COLOR);
      }
      else {
        _selected.erase(_candidate);
        _labels[_candidate]->fontColor(_FONT_COLOR);
        _labels[_candidate]->backColor(_LABEL_BKG_COLOR);
        _labels[_candidate]->borderColor(_LABEL_BORDER_COLOR);
      }
    }
  }
  else { // restore original "state"
    if (_selected.find(_candidate) == _selected.end()) {
      _labels[_candidate]->fontColor(_FONT_COLOR);
      _labels[_candidate]->backColor(_LABEL_BKG_COLOR);
      _labels[_candidate]->borderColor(_LABEL_BORDER_COLOR);
    }
    else {
      _labels[_candidate]->fontColor(_LABEL_BKG_COLOR);
      _labels[_candidate]->backColor(_FONT_COLOR);
      _labels[_candidate]->borderColor(_SELECTED_BORDER_COLOR);
    }
  }
  _candidate = 0;
}


void WList::scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) {
  // Check if scrolling of elements is required
  if (_labels.size() <= _max_visible_elems)
    return;

  if (offset > 0 and _top_element > 1) {
    if (_top_element <= _scroll_step)
      _scroll_step = _top_element - 1;
    _top_element -= _scroll_step;
    // The ones "above" need to be visible and have the first positions
    auto i{ _top_element };
    for ( ; i < _top_element +_scroll_step; i++) {
      _labels[i]->position(Position2D(_labels[i]->position().x(), _labels[i + _scroll_step]->position().y()));
      _labels[i]->show();
    }

    // Move the rest of the elements still visible down
    auto limit{ _top_element + _max_visible_elems };
    for ( ; i < limit; i++) 
      _labels[i]->position(Position2D(_labels[i]->position().x(), _labels[i]->position().y() + PositionPxl(_scroll_step * _ELEMENT_DISTANCE)));
    
    // Hide last elements and decrease 
    for ( ; i < limit + _scroll_step; i++)
      _labels[i]->hide();
  }
  else if (offset < 0 and _top_element <= (_labels.size() - _max_visible_elems)) {
    auto max_top_element{ _labels.size() - _max_visible_elems + 1 };
    if ((_top_element + _scroll_step) > max_top_element)
      _scroll_step = max_top_element - _top_element;
    _top_element += _scroll_step;

    // Hide first elements
    auto i{ _top_element - _scroll_step };
    for ( ; i < _top_element; i++)
      _labels[i]->hide();
    
    // Move elements up 
    for (; i < _top_element + _max_visible_elems - _scroll_step; i++) {
      _labels[i]->position(Position2D(_labels[i]->position().x(), _labels[i]->position().y() - PositionPxl(_scroll_step * _ELEMENT_DISTANCE)));
    }

    // The ones "below" need to be visible and have the last positions
    for (; i < _top_element + _max_visible_elems; i++) {
      _labels[i]->position(Position2D(_labels[i]->position().x(), _labels[i - 1]->position().y() + _ELEMENT_DISTANCE));
      _labels[i]->show();
    }
  }
}


void WList::draw() {
  // Draw background and border
  _rectangle.draw();
}


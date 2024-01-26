#include <glwnd/core/w_rect_surface.h>


using namespace glwnd;
using namespace glboost;



WRectSurface::WRectSurface(Size2D surface_size, SizePxl min_region_size) : _min_region_size{ min_region_size } {
  setRegions(surface_size);
}

void WRectSurface::size(const glboost::Size2D& new_size) {
  auto old_regions{ _regions };

  setRegions(new_size);
   
  // "Move" all the objects to the new regions
  for (auto obj : _all_objs) 
    addToRegions(*obj);
}


void WRectSurface::addObject(WObject& w_obj) {
  // No checking for duplicates
  addToRegions(w_obj);
  _all_objs.push_back(&w_obj);
}


void WRectSurface::removeObject(WObject& w_obj) {
  bool found{ false };
  // Find and remove the Object from every region
  removeFromRegions(w_obj);

  auto iterator_all_objs = std::find(_all_objs.begin(), _all_objs.end(), &w_obj);
  if (iterator_all_objs != _all_objs.end()) {
    _all_objs.erase(iterator_all_objs);
  }
}


void WRectSurface::updateObject(WObject& w_obj) {
  // Find and remove the Object from every region
  removeFromRegions(w_obj);

  auto iterator_all_objs = std::find(_all_objs.begin(), _all_objs.end(), &w_obj);
  if (iterator_all_objs != _all_objs.end()) {
    //Re-add object to the surface
    addToRegions(w_obj);
  }
  else {
    // The object is actually a new one!
    addObject(w_obj);
  }
}


WObject* WRectSurface::getObject(const Position2D& pos) {
  // row and column
  size_t col = pos.x() / _region_size.w();
  size_t row = pos.y() / _region_size.h();

  // Search the obj in which area the event happened
  for (auto w_obj : _regions.at(row).at(col).objs) {
    if (pos.x() >= w_obj->position().x() && pos.x() <= (w_obj->position().x() + w_obj->size().w()) &&
      pos.y() >= w_obj->position().y() && pos.y() <= (w_obj->position().y() + w_obj->size().h())) {
      if (w_obj->isVisible())
        return w_obj;
      else
        return nullptr;
    }
  }

  // No object found
  return nullptr;
}


void WRectSurface::setRegions(Size2D surface_size) {
  auto num_cols{ DEF_NUM_SEGMENTS };
  auto num_rows{ DEF_NUM_SEGMENTS };
  if (surface_size.w() / num_cols < _min_region_size) {
    num_cols = std::max(1, (surface_size.w()) / _min_region_size);
  }
  if (surface_size.h() / num_rows < _min_region_size) {
    num_rows = std::max(1, (surface_size.h()) / _min_region_size);
  }

  _region_size = Size2D(surface_size.w() / (SizePxl)num_cols, surface_size.h() / (SizePxl)num_rows);

  // Create 2-dimensional matrix of regions
  _regions.clear();
  _regions.resize(num_rows);
  for (auto& reg : _regions)
    reg.resize(num_cols);
}


void WRectSurface::addToRegions(WObject& w_obj) {
  // First and last occupied columns
  size_t first_col = w_obj.position().x() / _region_size.w();
  size_t last_col = (w_obj.position().x() + w_obj.size().w()) / _region_size.w();
  // First and last occupied rows
  size_t first_row = w_obj.position().y() / _region_size.h();
  size_t last_row = (w_obj.position().y() + w_obj.size().h()) / _region_size.h();

  size_t num_rows = _regions.size();
  size_t num_cols = _regions[0].size();

  for (size_t row = first_row; row <= std::min(num_rows - 1, last_row); row++) {
    for (size_t col = first_col; col <= std::min(num_cols - 1, last_col); col++) {
      _regions[row][col].objs.push_back(&w_obj);
    }
  }
}


void WRectSurface::removeFromRegions(const WObject& w_obj) {
  for (auto& cols_vec : _regions) {
    for (auto& objs_vec : cols_vec) {
      auto iterator = std::find(objs_vec.objs.begin(), objs_vec.objs.end(), &w_obj);
      if (iterator != objs_vec.objs.end()) 
        objs_vec.objs.erase(iterator);
    }
  }
}
#include <space_simulator_wnd.h>
#include <space_simulator_wnd.defs.h>

#include <glwnd/core/layout.h>


using namespace glboost;
using namespace glwnd;


void SpaceSimulatorWnd::initializeLayout() {

  // **************** FRAMES PER SECOND AND TICKS PER SECOND  **************** //
  // ************************************************************************* //
  const SizePxl      txt_ftps_size{ FONT_MEDIUM };
  const SizePxl      txt_ftps_H{ SizePxl(3 * txt_ftps_size - 2) };
  const SizePxl      txt_ftps_W{ SizePxl(4 * txt_ftps_size) };
  const SizePxl      txt_ftps_value_W{ SizePxl(7 * txt_ftps_size) };
  const PositionPxl  txt_ftps_X{ RIGHT_MRG - txt_ftps_W - txt_ftps_value_W };
  const PositionPxl  txt_ftps_Y{ BOTTOM_MRG - 2 * txt_ftps_H };

  _lbl_fps = std::make_unique<WLabel>("FPS ", txt_ftps_X, txt_ftps_Y, txt_ftps_W, txt_ftps_H);
  _lbl_fps->font(LBL_FONT_NAME, txt_ftps_size);
  _lbl_fps->fontColor(LBL_DESC_COLOR);
  _lbl_fps->backColor(Color3i(28, 34, 43));
  _lbl_fps->borderColor(_lbl_fps->backColor());
  _lbl_fps->alignVer(ALIGN_BOTTOM);
  _lbl_fps->alignHor(ALIGN_RIGHT);

  _lbl_tps = std::make_unique<WLabel>("TPS ", txt_ftps_X, txt_ftps_Y + txt_ftps_H - 1, _lbl_fps->size().w(), txt_ftps_H);
  _lbl_tps->font(LBL_FONT_NAME, txt_ftps_size);
  _lbl_tps->fontColor(_lbl_fps->fontColor());
  _lbl_tps->backColor(_lbl_fps->backColor());
  _lbl_tps->borderColor(_lbl_fps->borderColor());
  _lbl_tps->alignVer(ALIGN_VERT_CENTER);
  _lbl_tps->alignHor(ALIGN_RIGHT);

  _lbl_fps_value = std::make_unique<WLabel>("", txt_ftps_X + _lbl_fps->size().w() - 1, _lbl_fps->position().y(), txt_ftps_value_W, txt_ftps_H);
  _lbl_fps_value->font(LBL_FONT_NAME, txt_ftps_size);
  _lbl_fps_value->fontColor(LBL_VALUE_COLOR);
  _lbl_fps_value->backColor(_lbl_fps->backColor());
  _lbl_fps_value->borderColor(_lbl_fps->borderColor());
  _lbl_fps_value->alignVer(ALIGN_BOTTOM);

  _lbl_tps_value = std::make_unique<WLabel>("", _lbl_fps_value->position().x(), _lbl_tps->position().y(), _lbl_fps_value->size().w(), txt_ftps_H);
  _lbl_tps_value->font(LBL_FONT_NAME, txt_ftps_size);
  _lbl_tps_value->fontColor(_lbl_fps_value->fontColor());
  _lbl_tps_value->backColor(_lbl_fps_value->backColor());
  _lbl_tps_value->borderColor(_lbl_fps_value->borderColor());
  _lbl_tps_value->alignVer(ALIGN_VERT_CENTER);


  // **************** REAL DATE TIME ********************** //
  // ****************************************************** //
  const SizePxl      txt_time_size{ FONT_VERY_BIG };
  const SizePxl      txt_date_size{ FONT_BIG };
  const SizePxl      txt_elapsed_size{ FONT_BIG };
  const SizePxl      txt_time_H{ SizePxl(2 * txt_date_size) };
  const PositionPxl  txt_time_X{ LEFT_MRG };
  const PositionPxl  txt_time_Y{ BOTTOM_MRG - 3 * txt_time_H };

  _lbl_time = std::make_unique<WLabel>("", txt_time_X, txt_time_Y, 9 * txt_time_size, txt_time_H);
  _lbl_time->font(LBL_FONT_NAME, txt_time_size);
  _lbl_time->fontColor(LBL_VALUE_COLOR);

  _lbl_date = std::make_unique<WLabel>("", txt_time_X, _lbl_time->position().y() + txt_time_H, 15 * txt_date_size, txt_time_H);
  _lbl_date->font(LBL_FONT_NAME, txt_date_size);
  _lbl_date->fontColor(LBL_VALUE_COLOR);

  _lbl_elapsed = std::make_unique<WLabel>("", txt_time_X, _lbl_date->position().y() + txt_time_H, 12 * txt_elapsed_size, txt_time_H);
  _lbl_elapsed->font(LBL_FONT_NAME, txt_elapsed_size);
  _lbl_elapsed->fontColor(LBL_VALUE_COLOR);


  // **************** SIMULATION DATE TIME ********************** //
  // ************************************************************ //
  const SizePxl      txt_sim_date_size{ FONT_VERY_BIG };
  const SizePxl      txt_sim_time_size{ FONT_BIG };
  const SizePxl      txt_sim_elapsed_size{ FONT_BIG };
  const SizePxl      txt_sim_date_H{ SizePxl(16 + txt_sim_date_size) };
  const PositionPxl  txt_sim_date_X{ LEFT_MRG };
  const PositionPxl  txt_sim_date_Y{ TOP_MRG };

  _lbl_sim_date = std::make_unique<WLabel>("", txt_sim_date_X, txt_sim_date_Y, 11 * txt_sim_date_size, txt_sim_date_H);
  _lbl_sim_date->font(LBL_FONT_NAME, txt_sim_date_size);
  _lbl_sim_date->fontColor(LBL_VALUE_COLOR);

  _lbl_sim_time = std::make_unique<WLabel>("", txt_sim_date_X, txt_sim_date_Y + txt_sim_date_H, 5 * txt_sim_time_size, txt_sim_date_H);
  _lbl_sim_time->font(LBL_FONT_NAME, txt_sim_time_size);
  _lbl_sim_time->fontColor(LBL_VALUE_COLOR);

  _lbl_sim_elapsed = std::make_unique<WLabel>("", txt_sim_date_X, txt_sim_date_Y + 2 * txt_sim_date_H, 15 * txt_sim_elapsed_size, txt_sim_date_H);
  _lbl_sim_elapsed->font(LBL_FONT_NAME, txt_sim_elapsed_size);
  _lbl_sim_elapsed->fontColor(LBL_VALUE_COLOR);


  // **************** SIMULATION TICK, LABEL AND BUTTONS ********************* //
  // ************************************************************************* //
  const SizePxl      txt_tick_size{ FONT_BIG };
  const PositionPxl  txt_tick_Y{ txt_sim_date_Y + 4 * txt_sim_date_H };
  const SizePxl      txt_tick_W{ SizePxl(3 * txt_tick_size) };
  const SizePxl      txt_tick_H{ SizePxl(2 * txt_tick_size) };
  const SizePxl      txt_tick_value_W{ SizePxl(6 * txt_tick_size) };
  const SizePxl      btn_incdec_H{ SizePxl(4 * txt_tick_size / 3) };
  const SizePxl      btn_pause_H{ SizePxl(5 * txt_tick_size / 3) };

  _rect_lbl_tick = std::make_unique<gRectangle>(Position2D(LEFT_MRG - txt_tick_size, txt_tick_Y - 3), Size2D(txt_tick_size + txt_tick_W + txt_tick_value_W + 1, 2 * txt_tick_size + 4));
  _rect_lbl_tick->borderColor(Color4i(60, 70, 90, 255).normalized());

  _lbl_tick = std::make_unique<WLabel>("Tick ", LEFT_MRG, txt_tick_Y, txt_tick_W, txt_tick_H);
  _lbl_tick->font(LBL_FONT_NAME, txt_tick_size);
  _lbl_tick->fontColor(LBL_VALUE_COLOR);
  _lbl_tick->alignVer(ALIGN_VERT_CENTER);

  _lbl_tick_value = std::make_unique<WLabel>("", LEFT_MRG + txt_tick_W, txt_tick_Y, txt_tick_value_W, txt_tick_H);
  _lbl_tick_value->font(LBL_FONT_NAME, txt_tick_size);
  _lbl_tick_value->fontColor(LBL_VALUE_COLOR);
  _lbl_tick_value->alignVer(ALIGN_VERT_CENTER);

  _btn_dec_tick = std::make_unique<WIconButton>(Position2D(_rect_lbl_tick->position().x() + _rect_lbl_tick->size().w() + 8, alignVerCenter(_rect_lbl_tick->position().y(), _rect_lbl_tick->size().h(), btn_incdec_H)), Size2D(btn_incdec_H, btn_incdec_H),
    "img/left-arrow-blue.png", "img/left-arrow-white_blue.png");
  _btn_pause = std::make_unique<WStateButton>(Position2D(_btn_dec_tick->position().x() + _btn_dec_tick->size().w() + 8, alignVerCenter(_rect_lbl_tick->position().y(), _rect_lbl_tick->size().h(), btn_pause_H)), Size2D(btn_pause_H, btn_pause_H),
    "img/pause_blue.png", "img/play_blue.png");
  _btn_inc_tick = std::make_unique<WIconButton>(Position2D(_btn_pause->position().x() + _btn_pause->size().w() + 8, _btn_dec_tick->position().y()), Size2D(btn_incdec_H, btn_incdec_H),
    "img/right-arrow-blue.png", "img/right-arrow-white_blue.png");


  // *********************** BODIES LIST ************************************** //
  // ************************************************************************** //
  const SizePxl      list_W{ SizePxl(20 * FONT_MEDIUM) };
  const SizePxl      list_H{ SizePxl(60 * FONT_MEDIUM) };
  const PositionPxl  list_X{ RIGHT_MRG - list_W };
  const PositionPxl  list_Y{ PositionPxl(list_W) };
  WList::fontSize(FONT_MEDIUM);
  WList::fontName(LBL_FONT_NAME);
  WList::fontColor(LBL_VALUE_COLOR.normalized().toRGBA(1));
  WList::elementBorderColor(Color4i(51, 63, 80, 255).normalized());
  WList::selectedBorderColor(Color4i(255, 255, 255, 255).normalized());
  WList::elementSpacing(3);

  _list_bodies = std::make_unique<WList>(Position2D(list_X, list_Y), Size2D(list_W, list_H));

  // Add bodies sorted by distance
  addBodiesByDistance();


  // **************** BODIES LIST SORT BUTTONS ******************************** //
  // ************************************************************************** //
  const SizePxl      sort_buttons_H{ SizePxl(4 * FONT_MEDIUM) };
  const PositionPxl  sort_buttons_Y{ list_Y - 3 * sort_buttons_H / 2 };
  const SizePxl      sort_buttons_sep{ SizePxl(sort_buttons_H + (list_W - 4 * sort_buttons_H) / 3) };
  _btn_sort_by_distance = std::make_unique<glwnd::WStateButton>(list_X, sort_buttons_Y, sort_buttons_H, sort_buttons_H, "img/distance_unsel.png", "img/distance_sel.png");
  _btn_sort_by_mass = std::make_unique<glwnd::WStateButton>(list_X + sort_buttons_sep, sort_buttons_Y, sort_buttons_H, sort_buttons_H, "img/weight_unsel.png", "img/weight_sel.png");
  _btn_sort_by_name = std::make_unique<glwnd::WStateButton>(list_X + 2 * sort_buttons_sep, sort_buttons_Y, sort_buttons_H, sort_buttons_H, "img/alphabetical_unsel.png", "img/alphabetical_sel.png");
  _btn_sort_by_id = std::make_unique<glwnd::WStateButton>(list_X + 3 * sort_buttons_sep, sort_buttons_Y, sort_buttons_H, sort_buttons_H, "img/numerical_unsel.png", "img/numerical_sel.png");

  _btn_grp_sort = std::make_unique<glwnd::WStateButtonGroup>(*_btn_sort_by_distance, *_btn_sort_by_mass);
  _btn_grp_sort->add(*_btn_sort_by_name);
  _btn_grp_sort->add(*_btn_sort_by_id);


  // **************** BODIES LIST SHOW BUTTONS ******************************** //
  // ************************************************************************** //
  const PositionPxl  show_buttons_Y{ list_Y - 3 * sort_buttons_H };
  _btn_show_planets = std::make_unique<glwnd::WStateButton>(list_X, show_buttons_Y, sort_buttons_H, sort_buttons_H, "img/planet_unsel.png", "img/planet_sel.png");
  _btn_show_dwarf_planets = std::make_unique<glwnd::WStateButton>(list_X + sort_buttons_sep, show_buttons_Y, sort_buttons_H, sort_buttons_H, "img/dwarf_planet_unsel.png", "img/dwarf_planet_sel.png");
  _btn_show_minor_bodies = std::make_unique<glwnd::WStateButton>(list_X + 2 * sort_buttons_sep, show_buttons_Y, sort_buttons_H, sort_buttons_H, "img/asteroid_unsel.png", "img/asteroid_sel.png");
  _btn_show_satellites = std::make_unique<glwnd::WStateButton>(list_X + 3 * sort_buttons_sep, show_buttons_Y, sort_buttons_H, sort_buttons_H, "img/satellite_unsel.png", "img/satellite_sel.png");
  _btn_show_planets->switchState();
  _btn_show_dwarf_planets->switchState();
  _btn_show_minor_bodies->switchState();
  _btn_show_satellites->switchState();

}



void SpaceSimulatorWnd::addBodiesByDistance() {
  _list_bodies->add(_space.bodies().root().name());

  std::map<physics::units::LENGTH_T, std::string> by_distance;
  auto child_iter = _space.bodies().children(_space.bodies().root().name());
  while (child_iter.hasNext()) {
    by_distance[(*child_iter).orbit().a()] = (*child_iter).name();
    child_iter.next();
  }
  for (auto& pair : by_distance) {
    _list_bodies->add("   " + pair.second);

    auto grand_child_iter = _space.bodies().children(pair.second);
    std::map<physics::units::LENGTH_T, std::string> grand_child_by_distance;
    while (grand_child_iter.hasNext()) {
      grand_child_by_distance[(*grand_child_iter).orbit().a()] = (*grand_child_iter).name();
      grand_child_iter.next();
    }
    for (auto& gc_pair : grand_child_by_distance)
      _list_bodies->add("      " + gc_pair.second);
  }
}

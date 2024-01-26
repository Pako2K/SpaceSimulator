#ifndef GLWND_LAYOUT_H
#define GLWND_LAYOUT_H

#include <glwnd/core/w_object.h>
#include <glboost/graphic_types.h>

namespace glwnd
{
  // Align horizontally relative to another object
  glboost::PositionPxl alignHorRight(glboost::PositionPxl ref_pos, glboost::SizePxl ref_width, glboost::SizePxl width) { return ref_pos + ref_width - width; }
  glboost::PositionPxl alignHorCenter(glboost::PositionPxl ref_pos, glboost::SizePxl ref_width, glboost::SizePxl width) { return ref_pos + (ref_width - width) / 2; }

  // Align horizontally relative to another object
  glboost::PositionPxl alignVerBottom(glboost::PositionPxl ref_pos, glboost::SizePxl ref_height, glboost::SizePxl height) { return ref_pos + ref_height - height; }
  glboost::PositionPxl alignVerCenter(glboost::PositionPxl ref_pos, glboost::SizePxl ref_height, glboost::SizePxl height) { return ref_pos + (ref_height - height) / 2; }

}



#endif //GLWND_LAYOUT_H
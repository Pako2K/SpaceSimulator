#ifndef SPACE_SIMULATOR_WND_DEFS_H
#define SPACE_SIMULATOR_WND_DEFS_H


#define DEF_HOR_RESOLUTION    SizePxl(3840)     /** Default (expected) horizontal resolution */
#define DEF_VERT_RESOLUTION   SizePxl(2160)     /** Default (expected) vertical resolution */

/** Macro functions to calculate the actual x/y position or height */
#define TO_X_FN(x)  (x * screenSize().w() / DEF_HOR_RESOLUTION)
#define TO_Y_FN(y)  (y * screenSize().h() / DEF_VERT_RESOLUTION) 

/** Location of the fonts */
#define FONTS_DIR   "fonts/" 

#define LBL_FONT_NAME  "Orbitron Medium"

#define FONT_VERY_BIG  SizePxl(TO_Y_FN(26))
#define FONT_BIG       SizePxl(TO_Y_FN(20))
#define FONT_MEDIUM    SizePxl(TO_Y_FN(16))
#define FONT_SMALL     SizePxl(TO_Y_FN(12))

// SET MARGINS
#define LEFT_MRG       PositionPxl(TO_X_FN(50))
#define RIGHT_MRG      PositionPxl(TO_X_FN(DEF_HOR_RESOLUTION) - LEFT_MRG)
#define TOP_MRG        PositionPxl(TO_Y_FN(40))
#define BOTTOM_MRG     PositionPxl(TO_Y_FN(DEF_VERT_RESOLUTION) - TOP_MRG)

// STANDARD COLORS
#define LBL_DESC_COLOR   Color3i(68, 114, 240)
#define LBL_VALUE_COLOR  Color3i(143, 170, 240)


#endif


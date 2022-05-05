#include "InverterLCD.h"

 U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 25, /* data= /R/w */ 33, /* CS=*/ 32, /* reset= */ U8X8_PIN_NONE );  // ESP32

  // settings for u8g lib and LCD
  #define _INVLCD_DISP_w                 128            // LCD width
  #define _INVLCD_DISP_h                 64             // LCD height
  // font settings
  #define _INVLCD_DISP_font              u8g2_font_squeezed_r7_tr   // u8g_font_6x13  // u8glib font (more fonts under u8g.h line 1520 ...)
  #define _INVLCD_DISP_font_w            7              // font width
  #define _INVLCD_DISP_font_h            8             // font height
  // cursor settings
  #define _INVLCD_DISP_cursor_char       "_"            // cursor char
  #define _INVLCD_DISP_cur_space_before  2              // cursor space between
  #define _INVLCD_DISP_cur_space_behind  4              // cursor space between
  // menu position and size
  #define _INVLCD_DISP_box_x0            0              // start point (x0, y0)
  #define _INVLCD_DISP_box_y0            0              // start point (x0, y0)
  #define _INVLCD_DISP_box_x1            128            // width x  (x0 + width)
  #define _INVLCD_DISP_box_y1            64             // hight y  (y0 + height)
  #define _INVLCD_DISP_draw_frame        1              // draw a box around the menu
   // scrollbar width
  #define _INVLCD_DISP_scrollbar_w       6  // scrollbar width (if this value is < 3, the scrollbar is disabled)

  // nothing change here
  #define _INVLCD_DISP_cols_max          ((_INVLCD_DISP_box_x1-_INVLCD_DISP_box_x0)/_INVLCD_DISP_font_w)
  #define _INVLCD_DISP_rows_max          ((_INVLCD_DISP_box_y1-_INVLCD_DISP_box_y0-((_INVLCD_DISP_box_y1-_INVLCD_DISP_box_y0)/_INVLCD_DISP_font_h))/_INVLCD_DISP_font_h)

  // rows and cols
  // when you use more rows or cols as allowed change in LCDMenuLib.h the define "_INVLCD_DISP_cfg_max_rows" and "_INVLCD_DISP_cfg_max_string_length"
  // the program needs more ram with this changes
  #define _INVLCD_DISP_rows              _INVLCD_DISP_rows_max  // max rows
  #define _INVLCD_DISP_cols              20                   // max cols

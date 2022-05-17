#include "PVinverterLCD.h"
#include <string.h>
// #include "../../PVinverter/src/PVinverter.h">


U8G2_ST7920_128X64_F_SW_SPI u82gLCD(U8G2_R0, /* clock=*/ 25, /* data= /R/w */ 33, /* CS=*/ 32, /* reset= */ U8X8_PIN_NONE );  // ESP32

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


PVinverterLCD::PVinverterLCD (int _clock, int _data, int _CS, int _reset ) { 
                    // U8g2 Constructor List (Frame Buffer)
                    // The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
                    // Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
                    //U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 53, /* reset=*/ U8X8_PIN_NONE);     // (MEGA, ...
                    //U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 12, /* reset=*/ U8X8_PIN_NONE);   // (Uno and co
                    //U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 25, /* data= /R/w */ 33, /* CS=*/ 32, /* reset= */ U8X8_PIN_NONE );  // ESP32
                    U8G2_ST7920_128X64_F_SW_SPI u82gLCD(U8G2_R0, /* clock=*/ _clock, /* data= /R/w */ _data, /* CS=*/ _CS, /* reset= */ _reset );  // ESP32
          }

void PVinverterLCD::begin()
{
u82gLCD.begin();
u82gLCD.clearBuffer();					// clear the internal memory
u82gLCD.clear();
u82gLCD.setFont(_INVLCD_DISP_font);	// choose a suitable font

}

void PVinverterLCD::bootscreen(const char *_msg1, const char *_msg2 )
{
  u82gLCD.clearBuffer();					// clear the internal memory
   if(_INVLCD_DISP_draw_frame == 1) {
       u82gLCD.drawFrame(_INVLCD_DISP_box_x0, _INVLCD_DISP_box_y0, (_INVLCD_DISP_box_x1-_INVLCD_DISP_box_x0), (_INVLCD_DISP_box_y1-_INVLCD_DISP_box_y0));
    }
  
  u82gLCD.setFont(_INVLCD_DISP_font);
   u82gLCD.setCursor(_INVLCD_DISP_box_x1/2-u82gLCD.getStrWidth(_msg1)/2, (_INVLCD_DISP_box_y1 / 2) -10 );
   u82gLCD.print(_msg1);
  //u82gLCD.drawStr(0,10, _msg1 );
  if (strlen(_msg2)>0)
    {
    u82gLCD.setCursor(_INVLCD_DISP_box_x1/2-u82gLCD.getStrWidth(_msg2)/2, (_INVLCD_DISP_box_y1 / 2)+10);
    u82gLCD.print(_msg2);
    //u82gLCD.drawStr(0,20, _msg2 );
    }
  u82gLCD.sendBuffer();					// transfer internal memory to the display
}

void PVinverterLCD::QPIGS()
{
  if(_INVLCD_DISP_draw_frame == 1) {
       u82gLCD.drawFrame(_INVLCD_DISP_box_x0, _INVLCD_DISP_box_y0, (_INVLCD_DISP_box_x1-_INVLCD_DISP_box_x0), (_INVLCD_DISP_box_y1-_INVLCD_DISP_box_y0));
    }
  //PV_inverter->devsatus.ACcharge
  u82gLCD.setFont(_INVLCD_DISP_font);
  //bool a = PV_INVERTER::DevStatus.SCCcharge;
  //u82gLCD.drawStr(0,10,(const char*)a);

  //u82gLCD.drawString(0,0,"Line 0");
  //u82gLCD.drawString(0,8,"Line 8");
  //u82gLCD.drawString(0,9,"Line 9");
  //u82gLCD.refreshDisplay();		// only required for SSD1606/7  
  //delay(2000);
  u82gLCD.sendBuffer();					// transfer internal memory to the display
}
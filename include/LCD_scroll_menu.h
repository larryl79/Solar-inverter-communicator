#ifndef LCD_menu_scroll
    #define LCD_menu_scroll


/*
 * Concept of scrolling LCD menu.
 * 
 * you have to uncomment at least either BUTTON_ENABKED, or either ROTARY_ENABLED (, or both).
 * 
 */
#define BUTTONS_ENABLED         // uncomment this line if you want use buttons
#define ROTARY_ENABLED         // uncomment this line if you want use rotary encoder switch

// control menu by buttons
#ifdef BUTTONS_ENABLED
  #define pin_up 5
  #define pin_down 18
  #define pin_button 26
#endif

// control menu by rotary switch
#ifdef ROTARY_ENABLED
  #define ROTARY_A 4      // somtimes reffering as CLK on rotary switch
  #define ROTARY_B  15    // somtimes reffering as DT on rotary switch
  #define ROTARY_SW  27   // rotary button pin

  #define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 
  #include <ESPRotary.h>
  ESPRotary r;
  #include <Button2.h>          //  https://github.com/LennartHennigs/Button2
  Button2 b;
#endif

#ifndef LiquidCrystal_I2C_DEFINED
    #define LiquidCrystal_I2C_DEFINED
    #include <LiquidCrystal_I2C.h>
    #define LCDADDR 0x27                           // address of LCD on I2C bus
    #define LCDLINES 4                             // how many lines of the LCD
    #define LCDCOLUMNS 20                          // how many columns of the LCD  
    LiquidCrystal_I2C lcd(LCDADDR, LCDCOLUMNS, LCDLINES);
#endif

int menu_cursor       = 0;
int cursorpos_prev    = 1;   //dont change it
bool show_menucrursor = true;
bool LCDupdate        = false;
bool LCDclear         = false;

#ifdef ROTARY_ENABLED 
// single click
void click(Button2& btn);

// long click
void resetPosition(Button2& btn);
#endif

void showMenu(int cursorpos);
void menuSelected(int menuPoint);
void menuCursorPos(int cursorpos);
void buttonUp();
void buttonDown();
void buttonSelect();
void rotate(ESPRotary& r);
void showDirection(ESPRotary& r);
void menu_setup();
void menu_loop();

#endif
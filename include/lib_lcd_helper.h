#ifndef LIB_LCD_HELPER
#define LIB_LCD_HELPER
  #ifndef byte
    #define uint8_t byte
  #endif 

/* ********************************************************************** LCD Functions ********************************************** */
#include <Wire.h>
LiquidCrystal_I2C lcd(LCDADDR, LCDROWS, LCDLINES);

void  lcdinit()
{

   #ifdef ESP32
  Wire.begin(I2C_SDA, I2C_SCL);
  #else
  Wire.begin();
  #endif
  Wire.beginTransmission(LCDADDR);
  delay(10);
  byte lcderror = Wire.endTransmission();
  if ( lcderror == 0 ) { lcdok = true; }
  if ( lcdok ) 
     { 
      Serial.println("LDC Connected, initalizing."); 
     } 
     else 
     { 
       Serial.println("LDC doesn't connected. Skip use LCD."); 
       return; 
     }

  lcd.begin();
  lcd.clear();  
  lcd.noBacklight(); // turn off backlight
  lcd.backlight(); // turn on backlight.
  // characters of progressbar
  byte zero[]  = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte one[]   = { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000 };
  byte two[]   = { 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000 };
  byte three[] = { 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100 };
  byte four[]  = { 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110 };
  byte five[]  = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);
}

void lcd_clearLine(int line)
{               
        lcd.setCursor(0,line);
        for(int n = 0; n < 16; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
        {
                lcd.print(" ");
        }
        lcd.setCursor(0,line);
}

void lcdwrite(int i) 
{
  if ( lcdok == true )
    {
      lcd.write(i);
    }
}

void lcdprint (String msg)
{
  if ( lcdok == true )
    {
      lcd.print(msg);
    }
}

void lcdprint (float msg, int decimal = 0)
{
  if ( lcdok == true )
    {
      lcd.print(msg, decimal);
    }
}


void lcdclear()
{
if ( lcdok == true )
    {
      lcd.clear();
    }
}

void lcdsetCursor (int x, int y)
{
  if ( lcdok == true )
    {
      lcd.setCursor(x,y);
    }
}

void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)
{
  double f = totalCount/80.0;          
  int percent = (count+1)/f;
  int number = percent/5;
  int remainder = percent%5;
  if(number > 0)
    {
     lcdsetCursor(number-1,lineToPrintOn);
     lcdwrite(5);
    }
  lcdsetCursor(number,lineToPrintOn);
  lcdwrite(remainder);   
}

#endif
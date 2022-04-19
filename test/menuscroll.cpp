#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define LCDADDR 0x27                           // address of LCD on I2C bus
#define LCDLINES 4
#define LCDROWS 20

//Defining rotary pins 
// 
#include <ESPRotary.h>        //  https://github.com/LennartHennigs/ESPRotary/
#include <Button2.h>          //  https://github.com/LennartHennigs/Button2
#define ROTARY_PIN1  4        // CLK
#define ROTARY_PIN2  15       // DT
#define BUTTON_PIN   27       // Button / switch

#define CLICKS_PER_STEP   2   // this number depends on your rotary encoder 

/////////////////////////////////////////////////////////////////

ESPRotary r;
Button2 b;

/////////////////////////////////////////////////////////////////

LiquidCrystal_I2C lcd(LCDADDR, LCDROWS, LCDLINES);

int page_counter = 1;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

// custom up arrow char
byte UP[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00000,
    0b00000,
    0b00000
};

// custom char down arrow
byte DOWN[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b00100,
    0b11111,
    0b01110,
    0b00100
};

byte REPEAT[8] = {
    0B00000,
    0B00010,
    0B11111,
    0B10010,
    0B00000,
    0B01001,
    0B11111,
    0B01000
};

// rotary on change
void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
   Serial.println(page_counter);
}

// rotary on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
  if (r.directionToString(r.getDirection()) == "LEFT" && page_counter < 6)
    {
      lcd.clear();
      if (page_counter >= 6)
      { page_counter = 6; }
      else
      { page_counter++; }
    }

  if (r.directionToString(r.getDirection()) == "RIGHT" && page_counter > 0)
    { 
      
      lcd.clear();
      if (page_counter <= 1)
      { page_counter = 1; }
      else
      { page_counter--;  }
    }
}      
 
// single click
void click(Button2& btn) {
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}


void setup()
{
    Serial.begin(115200);
    delay(50);
    Serial.println("\n\nSimple Counter");

    r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
    r.setChangedHandler(rotate);
    r.setLeftRotationHandler(showDirection);
    r.setRightRotationHandler(showDirection);

    b.begin(BUTTON_PIN);
    b.setTapHandler(click);
    b.setLongClickHandler(resetPosition);

    lcd.begin();
    lcd.clear();
    lcd.createChar(1, UP);
    lcd.createChar(2, DOWN);
    lcd.createChar(6, REPEAT);
}


/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////


void loop()
{
    r.loop();
    b.loop();


// ------- WRITE ALL PAGE COUNTER ------ //
switch (page_counter)
    {
        case 1:     // page 1
            {
                lcd.setCursor(1,0);
                lcd.print("Page 1");
                lcd.setCursor(1,1);
                lcd.print("1st page ");

                // show arrow
                lcd.setCursor(0,LCDLINES - 1);
                lcd.write(byte(2));
            }
            break;

        case 2:     // page 2
            {
                lcd.setCursor(1,0);
                lcd.print("Page 2");

                // show arrows
                lcd.setCursor(0,0);
                lcd.write(byte(1));
                lcd.setCursor(0,LCDLINES - 1);
                lcd.write(byte(2));
            }
            break;

        case 3:     // page 3
            {
                lcd.setCursor(1,0);
                lcd.print("Page 3");

                // show arrows
                lcd.setCursor(0,0);
                lcd.write(byte(1));
                lcd.setCursor(0,LCDLINES - 1);
                lcd.write(byte(2));
            }
            break;

        case 4:     // page 4
            {
                lcd.setCursor(1,0);
                lcd.print("Page 4");

                // show arrows
                lcd.setCursor(0,0);
                lcd.write(byte(1));
                lcd.setCursor(0,LCDLINES - 1);
                lcd.write(byte(2));
            }
            break;

        case 5:     // page 5
            {
                lcd.setCursor(1,0);
                lcd.print("Page 5");

                // show arrows
                lcd.setCursor(0,0);
                lcd.write(byte(1));
                lcd.setCursor(0,LCDLINES - 1);
                lcd.write(byte(2));
            }
            break;

        case 6:     // page 6
            {
                lcd.setCursor(2,0);
                lcd.print("Page 6");
                lcd.setCursor(2,1);
                lcd.print("Last Page");

                // show arrows
                lcd.setCursor(0,0);
                lcd.write(byte(1));
            }
            break;

    }   // switch end


}  // loop end}
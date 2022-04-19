
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 0;
const int colorG = 0;
const int colorB = 30;

#define ROTARY_ANGLE_SENSOR A0
#define BUTTON 2 //the Grove - LED is connected to D2 of LinkIT ONE
#define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the seeeduino
         //board switches to 3V3, the ADC_REF should be 3.3
#define GROVE_VCC 5//VCC of the grove interface is normally 5v
#define FULL_ANGLE 300//full value of the rotary angle is 300 degrees
#define LIST_SIZE = 10;

char* myStrings[]={"Apple", "Pear", "Bread",
                    "Juice", "Chips","Soup","Oranges",
                    "Meat","Cheese","Cookies","Crackers"};

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    delay(1000);

    pinMode(ROTARY_ANGLE_SENSOR, INPUT);
    pinMode(BUTTON,INPUT);
}

void loop() 
{
    int degrees;
    degrees = getDegree();
    Serial.println("The angle between the mark and the starting position:");
    Serial.println(degrees);
    int index = degrees/LIST_SIZE;

    //Print index to screen
    lcd.setCursor(0,0);
    lcd.print(myStrings[index]);
    lcd.setCursor(0, 1);
    lcd.print(myStrings[index+1]);

    delay(100);
}
int getDegree()
{
  int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);
  float voltage;
  voltage = (float)sensor_value*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;
  return degrees;
}

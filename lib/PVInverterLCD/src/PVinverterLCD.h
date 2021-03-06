#ifndef INVERTERLCD_H
#define INVERTERLCD_H

  #ifndef ARDUINO_H
    #include <Arduino.h>
  #endif 

  #include <PVinverter.h>
  #include <U8g2lib.h>
  
  #ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>
  #endif
  #ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>
  #endif


//extern struct PV_INVERTER::DevStatus DevStatus;
//extern PV_INVERTER::DevStatus;


class PVinverterLCD
      {
      public:
        const char *_msg1;
        const char *_msg2;

        PVinverterLCD (int _clock, int _data, int _CS, int _reset=U8X8_PIN_NONE );
        void begin();
        void bootscreen(const char *_msg1, const char *_msg2 = "" );
        void QPIGS();

      private:
        

      };


#endif
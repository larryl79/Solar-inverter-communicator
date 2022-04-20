#ifndef byte
  #define uint8_t byte
#endif 

#include <HardwareSerial.h>

// #define USE_SOFTWARESERIAL     //uncomment this line if you want use softwareserial instead of hw
#ifdef USE_SOFTWARESERIAL
  #include <SoftwareSerial.h>
  // 16 RX, 17 TX for ESP-32 doit v1 using physical Serial2 with software.
  #define RX_pin 16
  #define TX_pin 17
  SoftwareSerial Serial3(RX_pin, TX_pin);
#endif

#ifndef LiquidCrystal_I2C_DEFINED
  #define LiquidCrystal_I2C_DEFINED
  #define heartbeat_led 2     // hearbeat led pin for ESP32-CAM  4 flashlight / internal led 33   esp32 led 2
  #define I2C_SDA 21          // ESP32 I2c pin        ESP32-CAM 2
  #define I2C_SCL 22          // ESP32 I2c pin        ESP32-CAM 14
  #define LCDADDR 0x27                           // address of LCD on I2C bus
  #define LCDLINES 4
  #define LCDROWS 20
  bool lcdok = false;                            // LCD not present by default (don't change it)
  #include <LiquidCrystal_I2C.h>
  #include <lib_lcd_helper.h>
#endif
#include <PVinverter.h>
//#include <LCD_scroll_menu.cpp>


String swversion="0.1b";
int error_code = 0;

PV_INVERTER inverter(Serial2);


// ******************************************  Setup  ******************************************
void setup()
{
  Serial.begin(115200); //Serial Monitor Console Baud Setting
  while (!Serial)
    {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  Serial.println("");
  #ifdef USE_SOFTWARESERIAL
  Serial.println("Setup serial3 for sodtwareserial");
  Serial3.begin(2400); //MPP-Solar inverter Baud Setting(http://www.offgrid.casa/wp-content/uploads/2017/10/HS_MS_MSX_RS232_Protocol_20140822_after_current_upgrade.pdf)
  Serial3.setTimeout(100);
    while (!Serial3)
    {
    ; // wait for SoftwareSerial port to connect.
    }
  Serial3.flush();
  #else
  
  Serial.println("Setup physical Serial2");
  /*
  Serial2.begin(2400, SERIAL_8N1); //MPP-Solar inverter Baud Setting(http://www.offgrid.casa/wp-content/uploads/2017/10/HS_MS_MSX_RS232_Protocol_20140822_after_current_upgrade.pdf)
  Serial2.setTimeout(100);
    while (!Serial2)
    {
    ; // wait for Serial2 port to connect.
    }
    */
  #endif

  inverter.begin(2400, 'A' , 1 /*VERBOSE_MODE */ );

  lcdinit();
  if ( lcdok == true )
    {
    lcdclear();
    lcdsetCursor(3,0);
    lcdprint("Solar Inverter");
    lcdsetCursor(4,1);
    lcdprint("Communicator");
    lcdsetCursor(7,2);
    lcdprint("V"+String(swversion));
    }
  delay(2000);
  //lcdclear();
  //setup_rotary();
  //printLCDmenu();
  //QPIGS_lcd_base();
  //menu_setup();
  pinMode(heartbeat_led, OUTPUT); //set up internal hearbeat led
  digitalWrite(heartbeat_led, LOW);
}

// ******************************************  Loop  ******************************************
void loop() {
  error_code = 0;
  yield();
  
   error_code = inverter.ask_data(millis());
  if (error_code != 0)                 
  {
    Serial.println("-- ERROR: INVERTER: Error executing 'ask_inverter_data' function! Error code:" + String(error_code));        
  }

  inverter.console_data();
delay(1000);






  //invereter_receive();
  //menu_loop();
  //delay(2000);
  /*
  if (DevStatus.SCCcharge)
    {
      digitalWrite(heartbeat_led, HIGH);
    }
    else
    {
      digitalWrite(heartbeat_led, LOW);
    }
    */

}


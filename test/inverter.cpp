#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #defina USE_SOFTWARESERIAL     //uncomment this line if you wan use softwareserial
#ifdef USE_SOFTWARESERIAL
#include <SoftwareSerial.h>
// 16 RX, 17 TX for ESP-32 doit v1 using physical Serial2 with software.
#define RX_pin 16
#define TX_pin 17
SoftwareSerial Serial3(RX_pin, TX_pin);
#endif

#ifdef ESP32 
#define heartbeat_led 2     // hearbeat led pin for ESP32-CAM  4 flashlight / internal led 33   esp32 led 2
#define I2C_SDA 21          // ESP32 I2c pin        ESP32-CAM 2
#define I2C_SCL 22          // ESP32 I2c pin        ESP32-CAM 14

#endif

#define LCDADDR 0x27                           // address of LCD
bool lcdok = false;                            // LCD not present by default (don't change it)
String stringOne;
String swversion="0.1b";

LiquidCrystal_I2C lcd(LCDADDR, 20, 4);
#include <lib_lcd_helper.h>

// commands in hex without crc and CR (don't add CRC and CR)
// String QPI = "\x51\x50\x49";
String QPIGS = "\x51\x50\x49\x47\x53"; // crc "\xB7\xA9" // CR "\x0D"
/*
String QPIWS = "\x51\x50\x49\x57\x53";
String QDI = "\x51\x44\x49";
String QMOD = "\x51\x4D\x4F\x44";
String QVFW =  "\x51\x56\x46\x57";
String QVFW2 = "\x51\x56\x46\x57\x32";
String QFLAG = "\x51\x46\x4C\x41\x47";
String QPIWS = "\x51\x50\x49\x57\x53";
String QMCHGCR = "\x51\x4D\x43\x48\x47\x43\x52";
String QMUCHGCR = "\x51\x4D\x55\x43\x48\x47\x43\x52";
String QBOOT = "\x51\x42\x4F\x4F\x54";
String QOPM = "\x51\x4F\x50\x4D";
String QPIRI = "\x51\x50\x49\x52\x49";
String QPGS0 = "\x51\x50\x47\x53\x30";
String QBV = "\x51\x42\x56";
String PF = "\x50\x46";
String POP02 = "\x50\x4F\x50\x30\x32";
String POP01 = "\x50\x4F\x50\x30\x31";
String POP00 = "\x50\x4F\x50\x30\x30";
String PCP00 = "\x50\x43\x50\x30\x30";
String PCP01 = "\x50\x43\x50\x30\x31";
String PCP02 = "\x50\x43\x50\x30\x32";
String MUCHGC002 = "\x4D\x55\x43\x48\x47\x43\x30\x30\x32";
String MUCHGC010 = "\x4D\x55\x43\x48\x47\x43\x30\x31\x30";
String MUCHGC020 = "\x4D\x55\x43\x48\x47\x43\x30\x32\x30";
String MUCHGC030 = "\x4D\x55\x43\x48\x47\x43\x30\x33\x30";
String PPCP000 = "\x50\x50\x43\x50\x30\x30\x30";
String PPCP001 = "\x50\x50\x43\x50\x30\x30\x31";
String PPCP002 = "\x50\x50\x43\x50\x30\x30\x32";
String QPIGS2 = "\x51\x50\x49\x47\x53\x32";
String POP03 = "\x50\x4F\x50\x30\x33";
*/

// Structure to store the data for QPIGS
struct pipVals_t {
  uint32_t gridVoltage;             // xxx V
  uint32_t gridFrequency;           // xx.xx Hz  *100
  uint32_t acOutput;                // xxx V
  uint32_t acFrequency;             // xx.xx Hz  *100
  uint32_t acApparentPower;         // xxxx VA
  uint32_t acActivePower;           // xxxx W
  uint32_t loadPercent;             // xxx %
  uint64_t busVoltage;              // xxxx V    *100
  uint32_t batteryVoltage;          // xx.xx V   *100
  uint32_t batteryChargeCurrent;    // xxx A
  uint32_t batteryCharge;           // %
  uint32_t inverterTemperature;     // xxxx      *100
  uint32_t PVCurrent;               // xx A
  uint32_t PVVoltage;               // xx V
  uint32_t PVPower;                 // xxxx W
  uint32_t batterySCC;              // xx.xx V   *100
  uint32_t batteryDischargeCurrent; // xxxx A
  char deviceStatus[8];             // 8 bit binary
} pipVals;

void QPIGS_val()
{
  char pipInputBuf[500];
  char *val;
  
  strcpy(pipInputBuf, stringOne.c_str());
     // Now split the packet into the values
        val = strtok((char *) pipInputBuf, " "); // get the first value
        pipVals.gridVoltage = atoi(val + 1);  // Skip the initial '('

        val = strtok(0, " "); // Get the next value
        pipVals.gridFrequency = atoi(val) * 10 ;

        val = strtok(0, " "); // Get the next value
        pipVals.acOutput = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.acFrequency = atoi(val) * 10;

        val = strtok(0, " "); // Get the next value
        pipVals.acApparentPower = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.acActivePower = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.loadPercent = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.busVoltage = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.batteryVoltage = atoi(val)*100;

        val = strtok(0, " "); // Get the next value
        pipVals.batteryChargeCurrent = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.batteryCharge = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.inverterTemperature = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.PVCurrent = atoi(val);

        val = strtok(0, " "); // Get the next value
        pipVals.PVVoltage = atoi(val);

        // Calculate PV Power
        pipVals.PVPower= pipVals.PVVoltage * pipVals.PVCurrent;

        val = strtok(0, " "); // Get the next value
        pipVals.batterySCC = atoi(val)*100;

        val = strtok(0, " "); // Get the next value
        pipVals.batteryDischargeCurrent = atoi(val);

        val = strtok(0, " "); // Get the next value
        strcpy(pipVals.deviceStatus, val);
}

void QPIGS_print()
{
        // Print out readings
        Serial.print("grid Voltage:......... ");
        Serial.print(pipVals.gridVoltage); Serial.println(" V");
        Serial.print("grid Frequency:....... ");
        Serial.print(pipVals.gridFrequency/10.0,1); Serial.println(" Hz");
        Serial.print("AC Output:............ ");
        Serial.print(pipVals.acOutput); Serial.println(" V");
        Serial.print("AC Frequency:......... ");
        Serial.print(pipVals.acFrequency/10.0,1); Serial.println(" Hz");
        Serial.print("AC ApparentPower:..... ");
        Serial.print(pipVals.acApparentPower); Serial.println(" VA");
        Serial.print("AC ActivePower:....... ");
        Serial.print(pipVals.acActivePower); Serial.println(" W");
        Serial.print("load Percent:......... ");
        Serial.print(pipVals.loadPercent); Serial.println(" %");
        Serial.print("bus Voltage:.......... ");
        Serial.print(pipVals.busVoltage/100.00,2); Serial.println(" V");   // shoiuld divide by 100
        Serial.print("battery Voltage:...... ");
        Serial.print(pipVals.batteryVoltage/100.00,2); Serial.println(" V");
        Serial.print("battery ChargeCurrent: ");
        Serial.print(pipVals.batteryChargeCurrent); Serial.println(" A");
        Serial.print("battery Charge:....... ");
        Serial.print(pipVals.batteryCharge); Serial.println(" %");
        Serial.print("inverter Temperature:. ");
        Serial.print(pipVals.inverterTemperature/10.0); Serial.println(" C");
        Serial.print("PV Current:........... ");
        Serial.print(pipVals.PVCurrent); Serial.println(" A");
        Serial.print("PV Voltage:........... ");
        Serial.print(pipVals.PVVoltage); Serial.println(" V");
        Serial.print("PV Power:............. ");
        Serial.print(pipVals.PVPower);  Serial.println(" W");
        Serial.print("battery SCC:.......... ");
        Serial.print(pipVals.batterySCC/100.00,2); Serial.println(" V");
        Serial.print("battery DischargeDischargeCurrent: ");
        Serial.print(pipVals.batteryDischargeCurrent); Serial.println(" A");
        Serial.print("DeviceStatus:......... ");
        Serial.println(String(pipVals.deviceStatus));
}

void QPIGS_lcd()
{
        // Print out QPIGS values on LCD
        lcdsetCursor(3,0); lcdprint("   "); lcdsetCursor(3,0);
        lcdprint(pipVals.gridVoltage);
        lcdsetCursor(8,0); lcdprint("   "); lcdsetCursor(8,0);
        lcdprint(pipVals.gridFrequency/10.0,1);
        lcdsetCursor(15,0); lcdprint("    "); lcdsetCursor(15,0);
        lcdprint(pipVals.acApparentPower);
        lcdsetCursor(3,1); lcdprint("   "); lcdsetCursor(3,1);
        lcdprint(pipVals.acOutput);
        lcdsetCursor(8,1); lcdprint("   "); lcdsetCursor(8,1);
        lcdprint(pipVals.acFrequency/10,1);
        lcdsetCursor(15,1); lcdprint("    "); lcdsetCursor(15,1);
        lcdprint(pipVals.acActivePower);
        /*
        lcdprint("bus Voltage:.......... ");
        lcdprint(pipVals.busVoltage/100); lcdprint(" V");
        */
       
        lcdsetCursor(3,2); lcdprint("    "); lcdsetCursor(3,2);
        lcdprint(pipVals.batteryVoltage/100,2);
        lcdsetCursor(10,2); lcdprint("   "); lcdsetCursor(10,2);
        lcdprint(pipVals.batteryChargeCurrent);
        
        lcdsetCursor(16,2); lcdprint("   "); lcdsetCursor(16,2);
        lcdprint(pipVals.batteryCharge);
        /*
        lcdprint("inverter Temperature:. ");
        lcdprint(pipVals.inverterTemperature); lcdprint(" C");
        */

        lcdsetCursor(3,3); lcdprint("  "); lcdsetCursor(3,3);
        lcdprint(pipVals.PVVoltage);
        lcdsetCursor(8,3); lcdprint("  "); lcdsetCursor(8,3);
        lcdprint(pipVals.PVCurrent);
        lcdsetCursor(12,3); lcdprint("   "); lcdsetCursor(12,3);
        lcdprint(pipVals.PVPower);
        lcdsetCursor(16,3); lcdprint("   "); lcdsetCursor(16,3);
        lcdprint(pipVals.loadPercent);
        
}


// ******************************************  CRC Functions  ******************************************
uint16_t crc_xmodem_update (uint16_t crc, uint8_t data)
{
  int i;
  crc = crc ^ ((uint16_t)data << 8);
  for (i=0; i<8; i++) {
  if (crc & 0x8000)
    crc = (crc << 1) ^ 0x1021; //(polynomial = 0x1021)
  else
    crc <<= 1;
   }
return crc;
}

uint16_t calc_crc(char *msg, int n)
{
// See bottom of this page: http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
// Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
// Initial value. xmodem uses 0xFFFF but this example
// requires an initial value of zero.
  uint16_t x = 0;
    while( n-- ) {
    x = crc_xmodem_update(x, (uint16_t)*msg++);
    yield();
    }
      return(x);
}

void lcdhome()
{
  lcdclear();
  lcd.setCursor(0,0);
  lcd.print("Gr");
  lcd.setCursor(0,1);
  lcd.print("Ou");
  lcd.setCursor(0,2);
  lcd.print("Ba");
  lcd.setCursor(0,3);
  lcd.print("PV");
  //print metrics
  lcdsetCursor(6,0);
  lcdprint("V");
  lcdsetCursor(12,0);
  lcdprint("Hz");
  lcdsetCursor(18,0);
  lcdprint("VA");

  lcdsetCursor(6,1);
  lcdprint("V");
  lcdsetCursor(12,1);
  lcdprint("Hz");
  lcdsetCursor(19,1);
  lcdprint("W");
  
  lcdsetCursor(8,2);
  lcdprint("V");
  lcdsetCursor(13,2);
  lcdprint("A");
  lcdsetCursor(19,2);
  lcdprint("%");

  lcdsetCursor(5,3);
  lcdprint("V");
  lcdsetCursor(10,3);
  lcdprint("A");
  lcdsetCursor(15,3);
  lcdprint("W");
  lcdsetCursor(19,3);  // inverter load
  lcdprint("%");
  
}

void inverter_send(String inv_command)
{
  uint16_t vgCrcCheck;
  int vRequestLen = 0;
  char s[6];
  int xx; 
  int yy;

  vRequestLen = inv_command.length();
  char vRequestArray[vRequestLen]; //Arrary define
  inv_command.toCharArray(vRequestArray, vRequestLen + 1);
  
  //Calculating CRC
  vgCrcCheck = calc_crc(vRequestArray,vRequestLen);

  // CRC returns two characters - these need to be separated and send as HEX to Inverter
  String vgCrcCheckString = String(vgCrcCheck, HEX);
  String vCrcCorrect = vgCrcCheckString.substring(0,2) + " " + vgCrcCheckString.substring(2,4);
        
  //CRC are returned as B7A9 - need to separate them and change from ASCII to HEX
  vCrcCorrect.toCharArray(s, 6);
  sscanf(s, "%x %x", &xx, &yy);  
  
  inv_command += char(xx);
  inv_command += char(yy);
  inv_command += "\x0D";

  //Sending Request to inverter
  #ifdef USE_SOFTWARESERIAL
  Serial3.print(inv_command);
  #else
  Serial2.print(inv_command);
  #endif
}

// ******************************************  Setup  ******************************************
void setup()
{
  Serial.begin(115200); //Serial Monitor Console Baud Setting
  while (!Serial)
    {
    ; // wait for serial port to connect. Needed for native USB port only
    }

  #ifdef USE_SOFTWARESERIAL
  Serial.println("Setup serial3 for sodtwareserial");
  Serial3.begin(2400); //MPP-Solar inverter Baud Setting(http://www.offgrid.casa/wp-content/uploads/2017/10/HS_MS_MSX_RS232_Protocol_20140822_after_current_upgrade.pdf)
  Serial3.setTimeout(10000);
    while (!Serial3)
    {
    ; // wait for serial port to connect.
    }
  Serial3.flush();
  #else
  Serial.println("Setup physical Serial2");
  Serial2.begin(2400, SERIAL_8N1); //MPP-Solar inverter Baud Setting(http://www.offgrid.casa/wp-content/uploads/2017/10/HS_MS_MSX_RS232_Protocol_20140822_after_current_upgrade.pdf)
  Serial2.setTimeout(10000);
    while (!Serial2)
    {
    ; // wait for Serial2 port to connect.
    }
  #endif
  
  lcdinit();
  if ( lcdok == true ) {
      lcdclear();
      lcdsetCursor(3,0);
      lcdprint("Solar Inverter");
      lcdsetCursor(4,1);
      lcdprint("Communicator");
      lcdsetCursor(7,2);
      lcdprint("V"+String(swversion));
  }
  delay(3000);
 lcdhome();
}

// ******************************************  Loop  ******************************************
void loop() {
  yield();
  
  inverter_send("QPIGS");

  //#endif

    #ifdef USE_SOFTWARESERIAL
    //if (Serial3.available())
    //  {
    stringOne = Serial3.readStringUntil('\r');
    #else
    //if (Serial2.available())
    //  {
    stringOne = Serial2.readStringUntil('\r');
    #endif

  // (NAK handle ???    // not accepted command message from inverter
  if (stringOne.substring(0,3) != "(NAK" )
    {
    Serial.println (stringOne.substring(0,stringOne.length()-2));
    QPIGS_val();
    QPIGS_print();
    QPIGS_lcd();
    delay(1000);
    }
  #ifdef USE_SOFTWARESERIAL
  #else
  //}
  #endif
  stringOne = "";
}


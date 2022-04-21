#ifndef PV_INVERTER_H
#define PV_INVERTER_H

#include <Arduino.h>
#include <string.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

class PV_INVERTER
{
 	public:
        
    String _POP_status;
		String NAK = "\x28\x4E\x41\x4B\x73\x73";   // "(NAKss"  this message receiving on not accepted command from inverter.
		String ACK = "\x41\x43\x4B";               // this message receiving on acknovledge of command

		// enquiry commands
		// commands in hex without CRC and CR (don't add CRC and CR, it will be calculated and added before send) // crc "\xB7\xA9" // CR "\x0D"
		String QPI      = "\x51\x50\x49";                      // Device Protocol ID Inquiry                page 1
		String QID      = "\x51\x49\x44";                      // The device serial number inquiry          page 1
		String QVFW     =  "\x51\x56\x46\x57";                 // Main CPU Firmware version inquiry         page 1
		String QVFW2    = "\x51\x56\x46\x57\x32";              // Another CPU Firmware version inquiry      page 2
		String QPIRI    = "\x51\x50\x49\x52\x49\xF8\x54";      // CRC included         // Device Rating Information inquiry         page 2
		String QFLAG    = "\x51\x46\x4C\x41\x47";              // Device flag status inquiry                page 4
		String QPIGS    = "\x51\x50\x49\x47\x53\xB7\xA9";      // CRC included       // Device general status parameters inquiry  page 4
		String QMOD     = "\x51\x4D\x4F\x44";                  // Device Mode inquiry                                           page 6
		String QPIWS    = "\x51\x50\x49\x57\x53";              // Device Warning Status inquiry                                 page 6
		String QDI      = "\x51\x44\x49";                      // The default setting value information                         page 7
		String QMCHGCR  = "\x51\x4D\x43\x48\x47\x43\x52";      // Enquiry selectable value about max charging current           page 10
		String QMUCHGCR = "\x51\x4D\x55\x43\x48\x47\x43\x52";  // Enquiry selectable value about max utility charging current   page 10
		String QBOOT    = "\x51\x42\x4F\x4F\x54";              // Enquiry DSP has bootstrap or not                              page 10
		String QOPM     = "\x51\x4F\x50\x4D";                  // Enquiry output mode (For 4K/5K)                               page 10
		String QPGS     = "\x51\x50\x47\x53";                  // QPGSn<cr>: Parallel Information inquiry（For 4K/5K)           page 10
		String QBV      = "\x51\x42\x56";                      // ?? no information in the docs. got from net example
		String QPIGS2   = "\x51\x50\x49\x47\x53\x32";          // got from internet example

		// inverter settings commands
		String PD     = "\x50\x44";                 // setting some status disable                           pg 13
		String PE     = "\x50\x45";                 // setting some status enable                            pg 13
		String PF     = "\x50\x46";                 // Setting control parameter to default value            pg 14
		String F      = "\x46";                     //  F<nn><cr>: Setting device output rating frequency    pg 15
    String POP01  = "\x50\x4F\x50\x30\x31\xD2\x69"; // CRC included    // POP01 Setting device output source priority: SolarUtilityBat
    String POP02  = "\x50\x4F\x50\x30\x32\xE2\x0B"; // CRC included    // POP02 Setting device output source priority: SolarBatUtility  
		String PBCV   = "\x50\x42\x43\x56";         // PBCV<nn.n><cr>: Set battery re-charge voltage         pg 16
		String PBDV   = "\x50\x42\x44\x56";         // PBDV<nn.n><cr>: Set battery re-discharge voltage      pg 16
		String PCP    = "\x50\x43\x50";             // PCP<NN><cr>: Setting device charger priority          pg 16
		String PGR    = "\x50\x47\x52";             // PGR<NN><cr>: Setting device grid working range        pg 16
		String PBT    = "\x50\x42\x54";             // PBT<NN><cr>: Setting battery type                     pg 16
		String PSDV   = "\x50\x53\x44\x56";         // PSDV<nn.n><cr>: Setting battery cut-off voltage (Battery under voltage)   pg 17
		String PCVV   = "\x50\x43\x56\x56";         // PCVV<nn.n><cr>: Setting battery C.V. (constant voltage) charging voltage  pg 17
		String PBFT   = "\x50\x42\x46\x54";         // PBFT<nn.n><cr>: Setting battery float charging voltage           pg 17
		String PPVOKC = "\x50\x50\x56\x4F\x4B\x43"; // PPVOKC<n><cr>: Setting PV OK condition                           pg 17
		String PSPB   = "\x50\x53\x50\x42";         // PSPB<n><cr>: Setting Solar power balance                         pg 17
		String MCHGC  = "\x4D\x43\x48\x47\x43";     // MCHGC<mnn><cr>: Setting max charging current                     pg 17
		String MUCHGC = "\x4D\x55\x43\x48\x47\x43"; // MUCHGC<mnn><cr>: Setting utility max charging current            pg 18
		String POPM   = "\x50\x4F\x50\x4D";         // POPM<mn ><cr>: Set output mode (For 4K/5K)                       pg 18
		String PPCP   = "\x50\x50\x43\x50";         // <MNN><cr>: Setting parallel device charger priority (For 4K/5K)  pg 18

    // Structure to store the data for QPIGS
    struct pipVals_t {
      uint32_t _unixtime;
      uint32_t gridVoltage;             // xxx V   
      uint32_t gridFrequency;           // xx.xx Hz  * 10
      uint32_t acOutput;                // xxx V   
      uint32_t acFrequency;             // xx.xx Hz  * 10
      uint32_t acApparentPower;         // xxxx VA
      uint32_t acActivePower;           // xxxx W
      uint32_t loadPercent;             // xxx %
      uint32_t busVoltage;              // xxxx V
      uint32_t batteryVoltage;          // xx.xx V   * 100
      uint32_t batteryChargeCurrent;    // xxx A
      uint32_t batteryCharge;           // %
      uint32_t inverterTemperature;     // xxxx
      uint32_t PVCurrent;               // xx.x A    * 10
      uint32_t PVVoltage;               // xx V
      uint32_t PVPower;                 // xxxx W    * 10
      uint32_t batterySCC;              // xx.xx V   * 100
      uint32_t batteryDischargeCurrent; // xxxx A
      char deviceStatus[8];             // 8 bit binary
      uint32_t batOffsetFan;            // Battery voltage offset for fans on  (2 numbers)
      uint32_t eepromVers;              // EEPROM version (2 numbers)
      uint32_t PV1_chargPower;          // PV1 Charging power (5 numbers)
      char deviceStatus2[4];            // Devide status 2
      uint32_t bat_backToUtilityVolts;  // Setup voltage to stop using battery and back to GRID power
      uint32_t bat_bulkChargeVolts;     // Setup voltage to 1st (elevation) and 2nd (Absorption) battery charge stages
      uint32_t bat_FloatChargeVolts;    // Setup voltage to 3rd (Floating) battery charge stage
      uint32_t bat_CutOffVolts;         // Minimum Limit voltage to discharge the Battery (used for % available battery calculation)
      uint8_t OutPutPriority;           // 0: Utility first / 1: Solar first / 2: SBU first
      uint8_t ChargerSourcePriority;    // 0: Utility first / 1: Solar first / 2: Solar + Utility / 3: Only solar charging permitted
      
    } pipVals;

    struct DevStatus_t 
    {
      bool SBUpriority = 0 ;     // add SBU priority version  b7
      bool ConfigStatus = 0 ;    // configuration status: 1: Change 0: unchanged b6
      bool FwUpdate = 0 ;        // b5: SCC firmware version 1: Updated 0: unchanged
      bool LoadStatus = 0 ;      // b4: Load status: 0: Load off 1:Load on
      bool BattVoltSteady = 0 ;  // b3: battery voltage to steady while charging
      bool Chargingstatus = 0 ;  // b2: Charging status( Charging on/off)
      bool SCCcharge = 0 ;       // b1: Charging status( SCC charging on/off)
      bool ACcharge = 0 ;        // b0: Charging status(AC charging on/off)
        // b2b1b0: 000: Do nothing 
        // 110: Charging on with SCC charge on
        // 101: Charging on with AC charge on
        // 111: Charging on with SCC and AC charge on
    } DevStatus;

    struct DevStatus2_t 
    {
      bool changingFloatMode = 0 ;    // 10: flag for charging to floating mode
      bool SwitchOn = 0 ;     // b9: Switch On
      bool dustProof = 0 ;        // b8: flag for dustproof installed(1-dustproof installed,0-no dustproof, only available for Axpert V series)
    } DevStatus2;

    struct QpiMessage
    {
      byte protocolId;
    };

    struct QmodMessage
    {
      char mode;
    };

    struct QpiwsMessage
    {
      bool reserved0;
      bool inverterFault;
      bool busOver;
      bool busUnder;
      bool busSoftFail;
      bool lineFail;
      bool opvShort;
      bool overTemperature;
      bool fanLocked;
      bool batteryVoltageHigh;
      bool batteryLowAlarm;
      bool reserved13;
      bool batteryUnderShutdown;
      bool reserved15;
      bool overload;
      bool eepromFault;
      bool inverterOverCurrent;
      bool inverterSoftFail;
      bool selfTestFail;
      bool opDcVoltageOver;
      bool batOpen;
      bool currentSensorFail;
      bool batteryShort;
      bool powerLimit;
      bool pvVoltageHigh;
      bool mpptOverloadFault;
      bool mpptOverloadWarning;
      bool batteryTooLowToCharge;
      bool reserved30;
      bool reserved31;
    };

    struct QflagMessage
    {
      bool disableBuzzer;
      bool enableOverloadBypass;
      bool enablePowerSaving;
      bool enableLcdEscape;
      bool enableOverloadRestart;
      bool enableOvertempRestart;
      bool enableBacklight;
      bool enablePrimarySourceInterruptedAlarm;
      bool enableFaultCodeRecording;
    };

    struct QidMessage
    {
      char id[16];
    };


	  PV_INVERTER( HardwareSerial& device) {hwStream = &device;}
    PV_INVERTER( SoftwareSerial& device) {swStream = &device;}

    void begin(uint32_t _baudRate, int _inverter_protocol = 1, uint8_t _verbose_begin = 0); // _protocol: 1 = 18 fields from QPIGS / 2 = 22 fields from QPIGS 
                                                                            // _verbose_begin: 0 = none  / 1 = Debug 
    void console_data();
    int handle_automation(int _hour, int _min);
    int ask_data(uint32_t _now);
    int get_protocol();
    void set_protocol(int _protocol_no);

 private:

    HardwareSerial* hwStream;
    SoftwareSerial* swStream;
    Stream* _streamRef;
    uint8_t _VERBOSE_MODE;

    int _inverter_protocol;    // "1" = 18 fields from QPIGS / "2" = 22 fields from QPIGS 
		void store_QPIRI(String value);
		void store_QPIGS(String value);
    void store_avg_QPIGS(String value);
    void store_status();
    void store_status2();
    int inverter_receive( String cmd, String& str_return ); // 0 = successfull
                                                            // 1 = No serial communication
                                                            // 2 = Not recognized command  // error codes should be positive integers
                                                            
    int inverter_send ( String inv_command );               // 0 = serial communication up and running
                                                            // 1 = No serial communication  // should be change to true and false
                                                           
    void ask_QPIRI( String& _result);    

		int _average_count = 0;
    uint32_t _average_oldtime;

    pipVals_t _pip_average;
    
 		
		// ******************************************  CRC Functions  ******************************************
		uint16_t crc_xmodem_update (uint16_t crc, uint8_t data);
		uint16_t calc_crc(char *msg, int n);
		// ******************************************  inverter communication  *********************************
		
      
	
};

#endif 

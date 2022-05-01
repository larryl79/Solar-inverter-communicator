#ifndef PVINVERTER_H
#define PVINVERTER_H

#ifndef ARDUINO_H
  #include <Arduino.h>
#endif 

#ifndef STRING_H
  #include <string.h>
#endif

#ifndef HARDWARESERIAL_H
  #include <HardwareSerial.h>
#endif

#ifndef SOFTWARESERIAL_H
  #include <SoftwareSerial.h>
#endif

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
    String QPIRI    = "\x51\x50\x49\x52\x49";              // Device Rating Information inquiry         page 2
    String QPIRICRC = "\x51\x50\x49\x52\x49\xF8\x54";      // CRC included         // Device Rating Information inquiry         page 2
    String QFLAG    = "\x51\x46\x4C\x41\x47";              // Device flag status inquiry                page 4
    String QPIGS    = "\x51\x50\x49\x47\x53";              // Device general status parameters inquiry  page 4
    String QPIGSCRC = "\x51\x50\x49\x47\x53\xB7\xA9";      // CRC included       // Device general status parameters inquiry  page 4
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
    String POP01  = "\x50\x4F\x50\x30\x31";     // CRC included    // POP01 Setting device output source priority: SolarUtilityBat
    String POP02  = "\x50\x4F\x50\x30\x32";     // CRC included    // POP02 Setting device output source priority: SolarBatUtility  
    String POP01CRC  = "\x50\x4F\x50\x30\x31\xD2\x69"; // CRC included    // POP01 Setting device output source priority: SolarUtilityBat
    String POP02CRC  = "\x50\x4F\x50\x30\x32\xE2\x0B"; // CRC included    // POP02 Setting device output source priority: SolarBatUtility  
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
      uint32_t inverterTemperature;     // xxxx     // have to / 100.00 to get right value
      uint32_t PVCurrent;               // xx.x A    * 10
      uint32_t PVVoltage;               // xxxx V
      uint32_t PVPower;                 // xxxx W    * 10
      uint32_t batterySCC;              // xx.xx V   * 100
      uint32_t batteryDischargeCurrent; // xxxx A
      uint8_t deviceStatus[8];             // 8 bit binary 
      uint16_t batOffsetFan;            // Battery voltage offset for fans on  (2 numbers)
      uint16_t eepromVers;              // EEPROM version (2 numbers)
      unsigned long int PV1_chargPower; // PV1 Charging power (5 numbers)
      uint8_t deviceStatus2[4];            // Devide status 2
      uint32_t bat_backToUtilityVolts;  // Setup voltage to stop using battery and back to GRID power
      uint32_t bat_bulkChargeVolts;     // Setup voltage to 1st (elevation) and 2nd (Absorption) battery charge stages
      uint32_t bat_FloatChargeVolts;    // Setup voltage to 3rd (Floating) battery charge stage
      uint32_t bat_CutOffVolts;         // Minimum Limit voltage to discharge the Battery (used for % available battery calculation)
      uint8_t OutPutPriority;           // 0: Utility first / 1: Solar first / 2: SBU first
      uint8_t ChargerSourcePriority;    // 0: Utility first / 1: Solar first / 2: Solar + Utility / 3: Only solar charging permitted
      
    } QPIGS_values;

    struct DevStatus_t 
    {
      uint8_t changingFloatMode = 0 ;  // 10: flag for charging to floating mode
      uint8_t SwitchOn = 0 ;           // b9: Switch On
      uint8_t dustProof = 0 ;          // b8: flag for dustproof installed(1-dustproof installed,0-no dustproof, only available for Axpert V series)
      uint8_t SBUpriority = 0 ;        // b7: add SBU priority version  b7
      uint8_t ConfigStatus = 0 ;       // b6: configuration status: 1: Change 0: unchanged
      uint8_t FwUpdate = 0 ;           // b5: SCC firmware version 1: Updated 0: unchanged
      uint8_t LoadStatus = 0 ;         // b4: Load status: 0: Load off 1:Load on
      uint8_t BattVoltSteady = 0 ;     // b3: battery voltage to steady while charging
                                       // b2b1b0: 000: Do nothing 
                                                  // 110: Charging on with SCC charge on
                                                  // 101: Charging on with AC charge on
                                                  // 111: Charging on with SCC and AC charge on
      uint8_t Chargingstatus = 0 ;     // b2: Charging status( Charging on/off)
      uint8_t SCCcharge = 0 ;          // b1: Charging status( SCC charging on/off)
      uint8_t ACcharge = 0 ;           // b0: Charging status(AC charging on/off)
    } DevStatus;

    struct QpiriVals_t  // Device Rating Information inquiry
    {
      uint32_t GridRatingVoltage;           // Grid rating voltage              xx.x V * 10
      uint32_t GridRatingCurrent;           // Grid rating current              xx.x A * 10
      uint32_t OutputRatingVoltage;         // AC output rating                 voltage xxx.x V * 10
      uint32_t OutputRatingFrequency;       // AC output rating                 frequency xx.x Hz * 10
      uint32_t OutputRatingCurrent;         // AC output rating current         xx.x A * 10
      uint32_t OutputRatingApparentPower;   // AC output rating apparent power  xxxx VA
      uint32_t OutputRatingActivePower;     // AC output rating active power    xxxx W
      uint32_t BatteryRatingVoltage;        // Battery rating voltage           xx.x V * 10
      uint32_t BatteryReChargeVoltage;      // Battery re-charge voltage        xx.x V * 10
      uint32_t BatteryUnderVoltage;         // Battery under voltage            xx.x V * 10
      uint32_t BatteryBulkVoltage;          // Battery bulk voltage             xx.x V * 10
      uint32_t BatteryFloatVoltage;         // Battery float voltage            xx.x V * 10
      uint8_t  BatteryType;                 // Battery type 0-9             0: AGM 1: Flooded 2: User 3: Pylon 5: Weco 6: Soltaro 8: Lib 9: Lic (5,6,8,9 protocol 2 )
      uint32_t MaxAC_ChargingCurrent;        // Max AC charging current          xxx A
      uint32_t MaxChargingCurrent;          // Max charging current             xxx A
      uint8_t  InputVoltageRange;           // input voltage range 0-1      0: Appliance 1: UPS
      uint8_t  OutputSourcePriority;        // output source priority 0-2   0: UtilitySolarBat 1: SolarUtilityBat 2: SolarBatUtility
      uint8_t  ChargerSourcePriority;       // charger source priority 0-3  0: Utility first 1: Solar first 2: Solar + Utility 3: Only solar charging permitted ( protocol 2 1-3 )
      uint8_t  ParallelMaxNum;              // parallel max num 0-9
      uint16_t MachineType;                 // Machine type                 00: Grid tie; 01: Off Grid; 10: Hybrid.
      uint8_t  Topology;                    // Topology                     0: transformerless 1: transformer
      uint8_t  OutputMode;                  // Output mode 0-7
                                                  // 00: single machine output 
                                                  // 01: parallel output 02: Phase 1 of 3 Phase output
                                                  // 03: Phase 2 of 3 Phase output
                                                  // 04: Phase 3 of 3 Phase output
                                                  // 05: Phase 1 of 2 Phase output
                                                  // 06: Phase 2 of 2 Phase output (120°)
                                                  // 07: Phase 2 of 2 Phase output (180°)
      uint32_t BatteryReDischargeVoltage;   // Battery re-discharge voltage xx.x V *10
      uint8_t  PV_OK_ConditionParallel;     // PV OK condition for parallel
                                                  // 0: As long as one unit of inverters has connect PV, parallel system willconsider PV OK;
                                                  // 1: Only All of inverters have connect PV, parallel system will consider PV OK
      uint8_t  PV_PowerBalance;             // PV power balance
                                                  // 0: PV input max current will be the max charged current;
                                                  // 1: PV input max power will be the sum of the max charged power and loads power.
      uint32_t MaxChargingTimeAtC_V_Stage;  // Max. charging time at C.V stage (only 48V model)     xxx minute  ( Protocol 2 only )
      uint8_t  OperationLogic;              // Operation Logic (only 48V model) 0-2                             ( Protocol 2 only )
                                                  // 0: Automatically
                                                  // 1: On-line mode
                                                  // 2: ECO mode
      uint8_t  MaxDischargingCurrent;       // Max discharging current (only 48V model)     xxx A
    } QpiriVals;

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
      bool enaOverloadBypass;
      bool enaPowerSaving;
      bool enaLcdEscape;
      bool enaOverloadRestart;
      bool enaOvertempRestart;
      bool enaBacklight;
      bool enaPrimarySourceInterruptedAlarm;
      bool enaFaultCodeRecor;
    };

    struct QidMessage
    {
      char id[16];
    };

    pipVals_t QPIGS_average;


    PV_INVERTER( HardwareSerial& device) {hwStream = &device;}
    PV_INVERTER( SoftwareSerial& device) {swStream = &device;}

    void begin(uint32_t _baudRate, int _inverter_protocol = 1, uint8_t _verbose_begin = 0); // _protocol: 1 = 18 fields from QPIGS / 2 = 22 fields from QPIGS 
                                                                            // _verbose_begin: 0 = none  / 1 = Debug 
    void ESPyield();  // add yield(); command to code if platform is ESP32 or ESP8266
    int  getProtocol();                      // get protocol number
    void setProtocol(int _protocol_no);      // set protocol number  (ovverides PV_INVERTER::begin) //0 no CRC add, 1 HPS, 2 MAX

    void console_data(pipVals_t _thisPIP);
    int  handle_automation(int _hour, int _min,  bool _CRChardcoded);
    int  ask_data(uint32_t _now,  bool _CRChardcoded);
    

/***************************************** private commands *******************************/
 private:

    HardwareSerial* hwStream;
    SoftwareSerial* swStream;
    Stream* _streamRef;
    uint8_t _VERBOSE_MODE;

    int _inverter_protocol;    // "1" = 18 fields from QPIGS / "2" = 22 fields from QPIGS 
    void store_QPIRI(String value);
    void store_QPIGS(String value, uint32_t _now);
    void clear_pipvals (pipVals_t &_thisPIP);
    void smoothing_QPIGS();
    void store_status();
    bool rap();
    String addCRC(String _cmd);
    char read(char _cmd);
    int receive( String cmd, String& str_return,  bool _CRChardcoded ); // 0 = successfull
                                                            // 1 = No serial communication
                                                            // 2 = Not recognized command  // error codes should be positive integers                                                        
    int send ( String inv_command, bool _CRChardcoded = false );       // 0 = serial communication up and running
                                                            // 1 = No serial communication  // should be change to true and false
                                                           
    void ask_QPIRI( String& _result, bool _CRChardcoded);    
    

    int _average_count = 0;
    uint32_t _average_oldtime;
    pipVals_t _QPIGS_tempAverage;
    
    
    
    // ******************************************  CRC Functions  ******************************************
    uint16_t crc_xmodem_update (uint16_t crc, uint8_t data);
    uint16_t calc_crc(char *msg, int n);
    // ******************************************  inverter communication  *********************************
    
      
  
};

#endif 

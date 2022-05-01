/**********************************************************************************************************************
 * 
 * Arduino thread about communicating with the solar PV_INVERTER: https://forum.arduino.cc/t/rs232-read-data-from-mpp-solar-PV_INVERTER/600960/72
 * Thanks for "athersaleem", "DanX3", "sibianul", "Larryl79" and my other that shared their knowledge and finds in the Arduino forum.
 * Version 1.0 written by Larryl79 (https://github.com/larryl79/PV-Inverter-Communicator)
 * Version 2.0 Copied and changed to fit the Ricardo Jr´s project (https://github.com/ninguemrj/offgrid_datalogging/)
 * 
 * Contributors: Larryl79, Richardo Jr ( NiguemJr )
 * 
 ************************************************************************************************************************/

#include "PVinverter.h"

void PV_INVERTER::begin(uint32_t _baudRate, int _inverter_protocol, uint8_t _verbose_begin) // "A" = 18 fields from QPIGS / "B" = 22 fields from QPIGS 
{
  if (hwStream)
  {
    hwStream->begin(_baudRate);
  }
  else
  {
    swStream->begin(_baudRate);
  }
  _streamRef = !hwStream? (Stream*)swStream : hwStream;

  // Initialize POP control status flag
  _POP_status = "";

  //--- prepare counter for smoothing reads (average)
  _average_count = 0;

  //--- For benchmarking the Solar PV_INVERTER communication ---------------
  _average_oldtime=millis();

  _VERBOSE_MODE = _verbose_begin;

  this->setProtocol(_inverter_protocol);

  // Initialize the pipvals and QPIGS_average with zeros (before first read)
  this->clear_pipvals(QPIGS_values);
  this->clear_pipvals(QPIGS_average);


}

void PV_INVERTER::ESPyield()
{
  #if defined (ESP8266) || (defined ESP32)
  yield();
  #endif
}

void PV_INVERTER::store_QPIRI(String value)
{
  if (value == "")
  {
    //--- QPIGS without data, skip this reading and wait next one -----------------  
    QPIGS_values.bat_backToUtilityVolts = 0;
    QPIGS_values.bat_bulkChargeVolts    = 0;
    QPIGS_values.bat_FloatChargeVolts   = 0;
    QPIGS_values.bat_CutOffVolts        = 0;
    QPIGS_values.OutPutPriority         = 0;
    QPIGS_values.ChargerSourcePriority  = 0;  
  }
  else
  {
     //--- Update status with data from PV_INVERTER  ---------------------------------  
    char pipInputBuf[200];
    char *val;
    
    strcpy(pipInputBuf, value.c_str());
    
    //--- Now split the packet into the values ------------------------------------
    val = strtok((char *) pipInputBuf, " ");            // discart the 1st value
    val = strtok(0, " ");                               // discart the 2nd value
    val = strtok(0, " ");                               // discart the 3th value
    val = strtok(0, " ");                               // discart the 4th value
    val = strtok(0, " ");                               // discart the 5th value
    val = strtok(0, " ");                               // discart the 6th value
    val = strtok(0, " ");                               // discart the 7th value
    val = strtok(0, " ");                               // discart the 8th value
    val = strtok(0, " ");                               // 9th value -> backToUtilityVolts
    QPIGS_values.bat_backToUtilityVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 10th value -> bat_CutOffVolts
    QPIGS_values.bat_CutOffVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 11th value -> bat_bulkChargeVolts
    QPIGS_values.bat_bulkChargeVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 12th value -> bat_FloatChargeVolts
    QPIGS_values.bat_FloatChargeVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // discart the 13th value
    val = strtok(0, " ");                               // discart the 14th value
    val = strtok(0, " ");                               // discart the 15th value
    val = strtok(0, " ");                               // discart the 16th value
    val = strtok(0, " ");                               // 17th value -> OutPutPriority 
    QPIGS_values.OutPutPriority  = atoi(val);
    val = strtok(0, " ");                               // 18th value -> ChargerSourcePriority
    QPIGS_values.ChargerSourcePriority  = atoi(val);   

   // ignore the other QPIRI fields
    
  }

}

void PV_INVERTER::store_QPIGS(String value, uint32_t _now)
{

  if (value=="")
  {
        // Clear pipvals in case of an incomplete or no reading
     this->clear_pipvals(QPIGS_values);
        
  } else {
    
        // Sets provided unixtime as argument for the QPIGS_values
        QPIGS_values._unixtime = _now;
 
  
        //--- Update status with data read from inverter serial communication ---------------------------------  
        char pipInputBuf[500];
        char *val;
        
        strcpy(pipInputBuf, value.c_str());
        
        //--- Now split the packet into the values ------------------------------------
        val = strtok((char *) pipInputBuf, " "); // get the first value
       if (atof(val + 1) >10)   // aviod false value stored, because it shows 2-3V even if grid isn't connected.
         {
            QPIGS_values.gridVoltage = atoi(val + 1);  // Skip the initial '('
         }
         else
         {
            QPIGS_values.gridVoltage = 0;
         }
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.gridFrequency = atof(val) * 10 ;
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.acOutput = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.acFrequency = atof(val) * 10;
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.acApparentPower = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.acActivePower = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.loadPercent = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.busVoltage = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.batteryVoltage = atof(val)*100;
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.batteryChargeCurrent = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.batteryCharge = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.inverterTemperature = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.PVCurrent = atof(val)*10;
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.PVVoltage = atof(val)*10;
      
        QPIGS_values.PVPower = QPIGS_values.PVVoltage * QPIGS_values.PVCurrent; // Calculate PV Power
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.batterySCC = atof(val)*100;
      
        val = strtok(0, " "); // Get the next value
        QPIGS_values.batteryDischargeCurrent = atoi(val);
      
        val = strtok(0, " "); // Get the next value
        char ds_temp[9];
        strcpy(ds_temp, val);
        QPIGS_values.deviceStatus[0] = (int)ds_temp[0]-'0'; 
        QPIGS_values.deviceStatus[1] = (int)ds_temp[1]-'0'; 
        QPIGS_values.deviceStatus[2] = (int)ds_temp[2]-'0'; 
        QPIGS_values.deviceStatus[3] = (int)ds_temp[3]-'0'; 
        QPIGS_values.deviceStatus[4] = (int)ds_temp[4]-'0'; 
        QPIGS_values.deviceStatus[5] = (int)ds_temp[5]-'0'; 
        QPIGS_values.deviceStatus[6] = (int)ds_temp[6]-'0'; 
        QPIGS_values.deviceStatus[7] = (int)ds_temp[7]-'0'; 
        
        if ( _inverter_protocol == 2)   // 2 = 22 fields from QPIGS
        {
          val = strtok(0, " "); // Get the next value
          QPIGS_values.batOffsetFan = atoi(val);
        
          val = strtok(0, " "); // Get the next value
          QPIGS_values.eepromVers = atoi(val);
        
          val = strtok(0, " "); // Get the next value
          QPIGS_values.PV1_chargPower = atoi(val);
        
          val = strtok(0, " "); // Get the next value
          strcpy(ds_temp, String(val).substring(0,3).c_str());
          QPIGS_values.deviceStatus2[0] = (int)ds_temp[0]-'0'; 
          QPIGS_values.deviceStatus2[1] = (int)ds_temp[1]-'0'; 
          QPIGS_values.deviceStatus2[2] = (int)ds_temp[2]-'0'; 

        }
        this->store_status();
  }
}

//--- When _average_count is from 0 to 9, this function Accumulates direct readings from pipVals into _QPIGS_tempAverage structure;
//--- When _average_count is = 9, additionally it averages the accumulated readings and updates the public QPIGS_average structure with the smoothed values
void PV_INVERTER::smoothing_QPIGS()
{
  if ((this->QPIGS_values.gridVoltage == 0) && (this->QPIGS_values.acOutput == 0) && (this->QPIGS_values.batteryVoltage == 0) && (this->QPIGS_values.PVVoltage == 0))
  {
    //--- QPIGS without data, skip this reading and wait next one -----------------  
     _average_count--;  
  }
  else
  {
    // Accumulaets readings on temp structure
    _QPIGS_tempAverage._unixtime                 =  QPIGS_values._unixtime;         // take the lastest read string only
    _QPIGS_tempAverage.gridVoltage              += QPIGS_values.gridVoltage;
    _QPIGS_tempAverage.gridFrequency            += QPIGS_values.gridFrequency;
    _QPIGS_tempAverage.acOutput                 += QPIGS_values.acOutput;
    _QPIGS_tempAverage.acFrequency              += QPIGS_values.acFrequency;
    _QPIGS_tempAverage.acApparentPower          += QPIGS_values.acApparentPower;
    _QPIGS_tempAverage.acActivePower            += QPIGS_values.acActivePower;
    _QPIGS_tempAverage.loadPercent              += QPIGS_values.loadPercent;
    _QPIGS_tempAverage.busVoltage               += QPIGS_values.busVoltage;
    _QPIGS_tempAverage.batteryVoltage           += QPIGS_values.batteryVoltage;
    _QPIGS_tempAverage.batteryChargeCurrent     += QPIGS_values.batteryChargeCurrent;
    _QPIGS_tempAverage.batteryCharge            += QPIGS_values.batteryCharge;
    _QPIGS_tempAverage.inverterTemperature      += QPIGS_values.inverterTemperature;
    _QPIGS_tempAverage.PVCurrent                += QPIGS_values.PVCurrent;
    _QPIGS_tempAverage.PVVoltage                += QPIGS_values.PVVoltage;
    _QPIGS_tempAverage.PVPower                  += QPIGS_values.PVPower;
    _QPIGS_tempAverage.batterySCC               += QPIGS_values.batterySCC;
    _QPIGS_tempAverage.batteryDischargeCurrent  += QPIGS_values.batteryDischargeCurrent;
    for(int i=0; i<8; i++)  _QPIGS_tempAverage.deviceStatus[i]=QPIGS_values.deviceStatus[i];  // take the lastest read string only
    
    if ( _inverter_protocol == 2)   // 2 = 22 fields from QPIGS
    {
      _QPIGS_tempAverage.PV1_chargPower         += QPIGS_values.PV1_chargPower;
      _QPIGS_tempAverage.batOffsetFan           = QPIGS_values.batOffsetFan;       // take the lastest read string only
      _QPIGS_tempAverage.eepromVers             = QPIGS_values.eepromVers;         // take the lastest read string only
      for(int i=0; i<3; i++)  _QPIGS_tempAverage.deviceStatus2[i]=QPIGS_values.deviceStatus2[i];  // take the lastest read string only
    }

    //--- when _average_count = 9: calculate average amounts to update QPIGS_average structure ---------------
    if(_average_count == 9)
    {
        QPIGS_average._unixtime               = _QPIGS_tempAverage._unixtime;         // take the lastest read string only
        QPIGS_average.gridVoltage             = _QPIGS_tempAverage.gridVoltage/10;
        QPIGS_average.gridFrequency           = _QPIGS_tempAverage.gridFrequency/10 ;
        QPIGS_average.acOutput                = _QPIGS_tempAverage.acOutput/10;
        QPIGS_average.acFrequency             = _QPIGS_tempAverage.acFrequency/10;
        QPIGS_average.acApparentPower         = _QPIGS_tempAverage.acApparentPower/10;
        QPIGS_average.acActivePower           = _QPIGS_tempAverage.acActivePower/10;
        QPIGS_average.loadPercent             = _QPIGS_tempAverage.loadPercent/10;
        QPIGS_average.busVoltage              = _QPIGS_tempAverage.busVoltage/10;
        QPIGS_average.batteryVoltage          = _QPIGS_tempAverage.batteryVoltage/10;
        QPIGS_average.batteryChargeCurrent    = _QPIGS_tempAverage.batteryChargeCurrent/10;
        QPIGS_average.batteryCharge           = _QPIGS_tempAverage.batteryCharge/10;
        QPIGS_average.inverterTemperature     = _QPIGS_tempAverage.inverterTemperature/10;
        QPIGS_average.PVCurrent               = _QPIGS_tempAverage.PVCurrent/10;
        QPIGS_average.PVVoltage               = _QPIGS_tempAverage.PVVoltage/10;
        QPIGS_average.PVPower                 = _QPIGS_tempAverage.PVPower/10;
        QPIGS_average.batterySCC              = _QPIGS_tempAverage.batterySCC /10;
        QPIGS_average.batteryDischargeCurrent = _QPIGS_tempAverage.batteryDischargeCurrent/10;
        for(int i=0; i<8; i++)  QPIGS_average.deviceStatus[i]=_QPIGS_tempAverage.deviceStatus[i];  // take the lastest read string only
        if ( _inverter_protocol == 2 )   // 2 = 22 fields from QPIGS
        {
          QPIGS_average.PV1_chargPower          = _QPIGS_tempAverage.PV1_chargPower/10;
          QPIGS_average.batOffsetFan            = _QPIGS_tempAverage.batOffsetFan;  // take the lastest read string
          QPIGS_average.eepromVers              = _QPIGS_tempAverage.eepromVers;    // take the lastest read string
          for(int i=0; i<3; i++)  _QPIGS_tempAverage.deviceStatus2[i]=QPIGS_average.deviceStatus2[i];  // take the lastest read string only
        }

        //--- RESETs the _QPIGS_tempAverage values to not accummulate the next 10 readings with previous ones ----
        this->clear_pipvals(_QPIGS_tempAverage);

        //--- RESETs average counting -----------------------------------------------------
        _average_count = 0;  
    }
    else
    {
      //--- Prepare counting for next array posotion -------------------------------
      _average_count++;
    }
  }
}

void PV_INVERTER::clear_pipvals (pipVals_t &_thisPIP)
{
    _thisPIP.gridVoltage              = 0;
    _thisPIP.gridFrequency            = 0;
    _thisPIP.acOutput                 = 0;
    _thisPIP.acFrequency              = 0;
    _thisPIP.acApparentPower          = 0;
    _thisPIP.acActivePower            = 0;
    _thisPIP.loadPercent              = 0;
    _thisPIP.busVoltage               = 0;
    _thisPIP.batteryVoltage           = 0;
    _thisPIP.batteryChargeCurrent     = 0;
    _thisPIP.batteryCharge            = 0;
    _thisPIP.inverterTemperature      = 0;
    _thisPIP.PVCurrent                = 0;
    _thisPIP.PVVoltage                = 0;
    _thisPIP.PVPower                  = 0;
    _thisPIP.batterySCC               = 0;
    _thisPIP.batteryDischargeCurrent  = 0;
    _thisPIP.deviceStatus[0]          = 0;
    _thisPIP.deviceStatus[1]          = 0;
    _thisPIP.deviceStatus[2]          = 0;
    _thisPIP.deviceStatus[3]          = 0;
    _thisPIP.deviceStatus[4]          = 0;
    _thisPIP.deviceStatus[5]          = 0;
    _thisPIP.deviceStatus[6]          = 0;
    _thisPIP.deviceStatus[7]          = 0;

     if ( _inverter_protocol == 2 )   // 2 = 22 fields from QPIGS
    {
      _thisPIP.PV1_chargPower          = 0;
      _thisPIP.batOffsetFan            = 0;  
      _thisPIP.eepromVers              = 0;    
      _thisPIP.PV1_chargPower          = 0;
      _thisPIP.deviceStatus2[0]        = 0;
      _thisPIP.deviceStatus2[1]        = 0;
      _thisPIP.deviceStatus2[2]        = 0;
    }
    this->store_status();

}

void PV_INVERTER::store_status ()   // this need investigate why causes reboot on ESP32
{
  this->ESPyield();
  DevStatus.SBUpriority      = QPIGS_values.deviceStatus[0];
  DevStatus.ConfigStatus     = QPIGS_values.deviceStatus[1];      // configuration status: 1: Change 0: unchanged b6
  DevStatus.FwUpdate         = QPIGS_values.deviceStatus[2];      // b5: SCC firmware version 1: Updated 0: unchanged
  DevStatus.LoadStatus       = QPIGS_values.deviceStatus[3];      // b4: Load status: 0: Load off 1:Load on
  DevStatus.BattVoltSteady   = QPIGS_values.deviceStatus[4];      // b3: battery voltage to steady while charging
  DevStatus.Chargingstatus   = QPIGS_values.deviceStatus[5];      // b2: Charging status( Charging on/off)
  DevStatus.SCCcharge        = QPIGS_values.deviceStatus[6];      // b1: Charging status( SCC charging on/off)
  DevStatus.ACcharge         = QPIGS_values.deviceStatus[7];      // b0: Charging status(AC charging on/off)
  if ( _inverter_protocol == 2 )
    {
      DevStatus.dustProof              = QPIGS_values.deviceStatus[8] ;    // b8: flag for dustproof installed(1-dustproof installed,0-no dustproof, only available for Axpert V series)  
      DevStatus.SwitchOn               = QPIGS_values.deviceStatus[9] ;    // b9: Switch On
      DevStatus.changingFloatMode      = QPIGS_values.deviceStatus[10];    // b10: flag for charging to floating mode
    }
    
}

void PV_INVERTER::console_data(pipVals_t _thisPIP)
{
  Serial.println("UNIX TIME:............... " + String(_thisPIP._unixtime) + " Epoch");
  Serial.println("Grid Voltage:............ " + String(_thisPIP.gridVoltage) + " V");
  Serial.println("Grid Frequency:.......... " + String(_thisPIP.gridFrequency/10.0) + " Hz");
  Serial.println("AC Output:............... " + String(_thisPIP.acOutput) + " V");
  Serial.println("AC Frequency:............ " + String(_thisPIP.acFrequency/10.0) + " Hz");
  Serial.println("AC ApparentPower:........ " + String(_thisPIP.acApparentPower) + " VA");
  Serial.println("AC ActivePower:.......... " + String(_thisPIP.acActivePower) + " W");
  Serial.println("Load Percent:............ " + String(_thisPIP.loadPercent) + " %");
  Serial.println("Bus Voltage:............. " + String(_thisPIP.busVoltage) + " V"); 
  Serial.println("Battery Voltage:......... " + String(_thisPIP.batteryVoltage/100.00)+ " V");
  Serial.println("Battery ChargeCurrent:... " + String(_thisPIP.batteryChargeCurrent) + " A"); 
  Serial.println("Battery Charge:.......... " + String(_thisPIP.batteryCharge) + " %"); 
  Serial.println("PV_INVERTER Temperature:. " + String(_thisPIP.inverterTemperature /10.0) + " °C"); 
  Serial.println("PV Current:.............. " + String(_thisPIP.PVCurrent /10.0)+ " A");
  Serial.println("PV Voltage:.............. " + String(_thisPIP.PVVoltage /10.0) + " V"); 
  Serial.println("PV Power:................ " + String(_thisPIP.PVPower   /100.00) + " W");  
  Serial.println("Battery SCC:............. " + String(_thisPIP.batterySCC/100.00) + " V"); 
  Serial.println("Batt DischargeCurrent:... " + String(_thisPIP.batteryDischargeCurrent) + " A"); 
  Serial.println("DeviceStatus bit 0:...... " + String(_thisPIP.deviceStatus[0]));
  Serial.println("DeviceStatus bit 1:...... " + String(_thisPIP.deviceStatus[1]));
  Serial.println("DeviceStatus bit 2:...... " + String(_thisPIP.deviceStatus[2]));
  Serial.println("DeviceStatus bit 3:...... " + String(_thisPIP.deviceStatus[3]));
  Serial.println("DeviceStatus bit 4:...... " + String(_thisPIP.deviceStatus[4]));
  Serial.println("DeviceStatus bit 5:...... " + String(_thisPIP.deviceStatus[5]));
  Serial.println("DeviceStatus bit 6:...... " + String(_thisPIP.deviceStatus[6]));
  Serial.println("DeviceStatus bit 7:...... " + String(_thisPIP.deviceStatus[7]));
  
  if ( _inverter_protocol == 2 )   // 2 = 22 fields from QPIGS
  {
    Serial.println("Battery offset Fan:.... " + String(_thisPIP.batOffsetFan) + " V");
    Serial.println("EEPROM Version:........ " + String(_thisPIP.eepromVers));
    Serial.println("PV1 Charger Power:..... " + String(_thisPIP.PV1_chargPower) + " W");
    Serial.println("DeviceStatus2 bit 0:... " + String(_thisPIP.deviceStatus2[0]));
    Serial.println("DeviceStatus2 bit 1:... " + String(_thisPIP.deviceStatus2[1]));
    Serial.println("DeviceStatus2 bit 2:... " + String(_thisPIP.deviceStatus2[2]));
  }
  // QPIRI values
  Serial.println("Bat Back to Grid:........ " + String(_thisPIP.bat_backToUtilityVolts/10.0) + " V"); 
  Serial.println("Bat Bulk Charge:......... " + String(_thisPIP.bat_bulkChargeVolts/10.0) + " V"); 
  Serial.println("Bat Float Charge:........ " + String(_thisPIP.bat_FloatChargeVolts/10.0) + " V"); 
  Serial.println("Bat CutOff:.............. " + String(_thisPIP.bat_CutOffVolts/10.0) + " V"); 
  Serial.println("Output Priority:......... " + String(_thisPIP.OutPutPriority) + " | 0: Utility first / 1: Solar first / 2: SBU first"); 
  Serial.println("Charging Priority:....... " + String(_thisPIP.ChargerSourcePriority) + " | 0: Utility first / 1: Solar first / 2: Solar + Utility / 3: Only solar"); 
}


int PV_INVERTER::getProtocol()
  {
    return _inverter_protocol;
  }

void PV_INVERTER::setProtocol(int _protocol_no)
  {
    _inverter_protocol = _protocol_no;
  }


// ******************************************  CRC Functions  ******************************************
uint16_t PV_INVERTER::crc_xmodem_update (uint16_t crc, uint8_t data)
{
  int i;
  crc = crc ^ ((uint16_t)data << 8);
  for (i=0; i<8; i++) {
  if (crc & 0x8000)
    { crc = (crc << 1) ^ 0x1021; } //(polynomial = 0x1021) 
  else
    { crc <<= 1; } 
   }
return crc;
}

uint16_t PV_INVERTER::calc_crc(char *msg, int n)
{
  uint16_t _CRC = 0;
  switch ( this->getProtocol() )     // select protocol for the right CRC calculation.
  {
    case 0:  //no crc needed
    break;

    case 1:   // protocol 1 CRC HPS (PowMr ) MTTP inverter
    case 2:    // protocol 2 CRC for MAX MPPT
    default:
      
      while( n-- )
        {
        _CRC = this->crc_xmodem_update( _CRC, (uint16_t)*msg++);
        this->ESPyield();
        }
    break;

    
    case 3:   // for future
    break;

  }
return( _CRC );
}

// ******************************************  PV_INVERTER communication  *********************************

bool PV_INVERTER::rap()   //knocknock to get synced commauncation if avail
{
  bool _communication = false;
  this->_streamRef->print("QKnock-Knock\x0D");  //  knock-knock for communiction exist
  this->_streamRef->flush();          // Wait finishing transmitting before going on...
  if (this->_streamRef->readStringUntil('\x0D') == "(NAKss" )   // check if get response for "knock-knock" from PV_INVERTER on serial port.
    { _communication = true; }
    return _communication; // true if ok, false for no communication.
}

char PV_INVERTER::read(char _cmd)   // new serial read function, no ready yet, and not tested.
{
  char _str_return;
   if ( this->send(String(_cmd)) == 0 )
    {
      while ( _streamRef->available() > 0)
        {
        _str_return = _streamRef->read();
        if ( _str_return == '\x0D' )
          {
            return _str_return;
          }
        this->ESPyield();
        }
    }
  return false;
}


String PV_INVERTER::addCRC(String _cmd)
{
  if ( !_cmd )
      {
      uint16_t _vgCrcCheck;
      int _vInvCommandLen = 0;
      char _s[6];
      int _CRC1; 
      int _CRC2;
      _vInvCommandLen = _cmd.length();
      char _vInvCommandArray[_vInvCommandLen]; //Arrary define

      _cmd.toCharArray(_vInvCommandArray, _vInvCommandLen + 1);
  
      //Calculating CRC
      _vgCrcCheck = this->calc_crc((_vInvCommandArray),_vInvCommandLen);

      // CRC returns two characters - these need to be separated and send as HEX to PV_INVERTER
      String _vgCrcCheckString = String(_vgCrcCheck, HEX);
      String _vCrcCorrect = _vgCrcCheckString.substring(0,2) + " " + _vgCrcCheckString.substring(2,4);
        
      //CRC are returned as B7A9 - need to separate them and change from ASCII to HEX
      _vCrcCorrect.toCharArray(_s, 6);
      sscanf(_s, "%x %x", &_CRC1, &_CRC2);  
  
      _cmd += char(_CRC1);   // add CRC byte 1
      _cmd += char(_CRC2);   // add CRC byte 2    
    }
return _cmd;
}



int PV_INVERTER::send(String _inv_command, bool _CRChardcoded)
{
  if ( this->rap() )   // check if get response for "knock-knock" from PV_INVERTER on serial port.
  {
    if (!_CRChardcoded)
      {
        _inv_command += this->addCRC(_inv_command);   // add CRC sting to the command by protocol number
      }
    _inv_command += "\x0D";     // add CR
    //Sending Request to PV_INVERTER
    _streamRef->print(_inv_command);
    _streamRef->flush();          // Wait finishing transmitting before going on...
  }
  else
  {
    return 1; // No serial communication, error code 1
  }
   return 0; // NAKss returned, serial communication up and running
}

int PV_INVERTER::receive( String cmd, String &str_return,  bool _CRChardcoded )
{
  if ( this->send(cmd, _CRChardcoded) == 0 )
    {
      str_return = _streamRef->readStringUntil('\x0D');
      
      // checking Command not recognized 
      if (str_return == "(NAKss") 
      {
        Serial.println("PV_INVERTER: " + cmd + ": Not recognized command: " + str_return);
        return 2;   
      }

      // TODO: TEST for CRC receipt match with calculated CRC
      
      if (_VERBOSE_MODE == 1)
        Serial.println("PV_INVERTER: " + cmd + ": Command executed successfully. Returned: " + str_return);
      return 0;
    }
    else
    {
      // No serial communication
      Serial.println("PV_INVERTER: " + cmd + ": No serial communication");
      str_return = "";
      return 1;
    }
    
}

void PV_INVERTER::ask_QPIRI( String &_result, bool _CRChardcoded)
  {
      int _funct_return = 0;
      _result = "";
      
      //workaround for diff CRC then calculated. Check a better way to do it later
      if (_CRChardcoded)
      {
        _funct_return = this->receive(QPIRICRC, _result, _CRChardcoded);
      }
      else
      {
        _funct_return = this->receive(QPIRI, _result, _CRChardcoded);
      }
      
      if (_funct_return == 0) 
      {
        // checking return string lengh for QPIRI command 
        if (strlen(_result.c_str()) < 85)       
        {
          Serial.println("PV_INVERTER: QPIRI: Receipt string is not completed, size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
          _result = "";                                    // clear the string result from PV_INVERTER as it is not complete
          _funct_return = 1;                              // short string lengh for QPIRI command 
        }
      }
  }

int PV_INVERTER::ask_data(uint32_t _now,  bool _CRChardcoded)
    {
      int _funct_return = 0;
      String _result = "";
      
      //workaround for diff CRC then calculated. Check a better way to do it later
      if (_CRChardcoded)
      {
        _funct_return = this->receive(QPIGSCRC, _result, _CRChardcoded);
      }
      else
      {
        _funct_return = this->receive(QPIGS, _result, _CRChardcoded);
      } 
      if (_funct_return == 0) 
      {
        if (_VERBOSE_MODE == 1)
          Serial.println("PV_INVERTER: QPIGS: Receipt string size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
        
        // checking return string lengh for QPIGS command 
        if (strlen(_result.c_str()) < 85)       
        {
          Serial.println("PV_INVERTER: QPIGS: Receipt string is not completed, size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
          _result = "";                                 // clear the string result from PV_INVERTER as it is not complete
          _funct_return = 1;                            // short string lengh for QPIGS command 
        }

        this->store_QPIGS(_result.c_str(), _now);               // Updates with direct reading from inverter
        this->smoothing_QPIGS();                          // accumulates, average and updates QPIGS_average.
        
        // Ask Inverer for QPIRI configuration in the 10th QPIGS reading (0 to 9)
        // (when the averaged amount will be stored in the public variables)
        if (_average_count == 9)
        {
        
          // Reads QPIRI command to check battery configurations
          // TODO: Read all QPIRI fields
          
          String _QPIRI_result;
          this->ask_QPIRI(_QPIRI_result, _CRChardcoded);
  
          // store QPIRI info
          this->store_QPIRI(_QPIRI_result);
  
          //--- For benchmarking the averaged Solar PV_INVERTER communication ---------------------------      
          if (_VERBOSE_MODE == 2)
            Serial.println ("Time to read, acummulate and average QPIGS info: " + String((millis() - _average_oldtime)));
  
          // prepare for a new banchmarck
          _average_oldtime = millis();
        }
        //if (_VERBOSE_MODE == 1) this->console_data(QPIGS_values); 
      }
      return _funct_return;    
    }

int PV_INVERTER::handle_automation(int _hour, int _min,  bool _CRChardcoded)
    {
      int _funct_return = 0;
      String _result = "";
      uint32_t minutes = (_hour * 60) + _min ;              // Minutes to compare with preset time rules
      
      const uint32_t _evening_min = ( 18   * 60) +   0  ;   // 18:00hs evening start (sun stops to generating on 16:30, but 
                                                            // I just use battery as from 18hs to save them from deep discharge
                                                       
      const uint32_t _begin_min     = (  7   * 60) +   30 ; // 07:30hs start generating solar power

      // RULE #1 //////////////////////////////////////////////////////////
      //  - After "_begin_min" and before "_evening_min"
      //
      //  SET: SOLAR = Solar First (Solar, Utility and Battery) 
      /////////////////////////////////////////////////////////////////////
       
      if ((_begin_min < minutes) && (minutes < _evening_min))
      {
        // Informed time is between sun rise and evening = set as "Solar First"
        
        if (_POP_status != POP01)
        {
          // Only changes the Output Priority if previous status is different
          
          if (_CRChardcoded)
          {
            _funct_return = this->receive(POP01CRC, _result, _CRChardcoded);
          }
          else
          {
            _funct_return = this->receive(POP01, _result, _CRChardcoded);
          }
          if ( _funct_return == 0)                   
          {
             Serial.println ("--INFO: PV_INVERTER: POP01: Output Priority set to Solar/Grid/Battery");
             _POP_status = POP01;
          }
          else
          {
             // Needs to treat errors for better error messages
             Serial.println("-- ERROR: PV_INVERTER: POP01: Failed to set Output Priority to Solar/Grid/Battery. PV_INVERTER Returned: " + _result);       
          }
        }
      }

      // RULE #2 //////////////////////////////////////////////////////////
      //  - Before "_begin_min" OR after "_evening_min"
      //
      //  SET: SBU (Solar, Battery and Utility) 
      /////////////////////////////////////////////////////////////////////
       
      if ((_begin_min > minutes) || (minutes > _evening_min))
      {
        // Informed time is between sun rise and evening = set as "Solar First"
        
        if (_POP_status != POP02)
        {
          // Only changes the Output Priority if previous status is different
          if (_CRChardcoded)
          {
            _funct_return = this->receive(POP02CRC, _result, _CRChardcoded);
          }
          else
          {
            _funct_return = this->receive(POP02, _result, _CRChardcoded);
          }          
          
          if ( _funct_return == 0)                   
          {
             Serial.println ("--INFO: PV_INVERTER: POP02: Output Priority set to Solar/Battery/Grid");
             _POP_status = POP02;
          }
          else
          {
            // Needs to treat errors for better error messages
             Serial.println("-- ERROR: PV_INVERTER: POP02: Failed to set Output Priority to Solar/Battery/Grid. PV_INVERTER Returned: " + _result);       
          }
        }
      }
      return _funct_return;
    }

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

void PV_INVERTER::begin(uint32_t _baudRate, inverter_protocol_t _protocol, uint8_t _verbose_begin) // "A" = 18 fields from QPIGS / "B" = 22 fields from QPIGS 
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

  //--- sets how much fields from QPIGS
  _inverter_protocol = _protocol;
  
  //--- For benchmarking the Solar PV_INVERTER communication ---------------
  _average_oldtime=millis();

  _VERBOSE_MODE = _verbose_begin;

}

void PV_INVERTER::store_QPIRI(String value)
{
  if (value == "")
  {
    //--- QPIGS without data, skip this reading and wait next one -----------------  
    pipVals.bat_backToUtilityVolts = 0;
    pipVals.bat_bulkChargeVolts    = 0;
    pipVals.bat_FloatChargeVolts   = 0;
    pipVals.bat_CutOffVolts        = 0;
    pipVals.OutPutPriority         = 0;
    pipVals.ChargerSourcePriority  = 0;  
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
    pipVals.bat_backToUtilityVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 10th value -> bat_CutOffVolts
    pipVals.bat_CutOffVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 11th value -> bat_bulkChargeVolts
    pipVals.bat_bulkChargeVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // 12th value -> bat_FloatChargeVolts
    pipVals.bat_FloatChargeVolts = atof(val) * 10 ;
    val = strtok(0, " ");                               // discart the 13th value
    val = strtok(0, " ");                               // discart the 14th value
    val = strtok(0, " ");                               // discart the 15th value
    val = strtok(0, " ");                               // discart the 16th value
    val = strtok(0, " ");                               // 17th value -> OutPutPriority 
    pipVals.OutPutPriority  = atoi(val);
    val = strtok(0, " ");                               // 18th value -> ChargerSourcePriority
    pipVals.ChargerSourcePriority  = atoi(val);   

   // ignore the other QPIRI fields
    
  }

}

void PV_INVERTER::store_QPIGS(String value)
{
  if (_average_count < 10)
  {
      //--- Accumulates readings from 0 to 9 ------------------------------------------
      if (value == "")
      {
        //--- QPIGS without data, skip this reading and wait next one -----------------  
         _average_count--;  
      }
      else
      {
         //--- Update status with data from PV_INVERTER  ---------------------------------  
        char pipInputBuf[500];
        char *val;
        
        strcpy(pipInputBuf, value.c_str());
        
        //--- Now split the packet into the values ------------------------------------
        val = strtok((char *) pipInputBuf, " "); // get the first value
        if (atof(val + 1) >10)   // aviod false value stored, because it shows 2-3V even if grid isn't connected.
          {
            _pip_average.gridVoltage += atoi(val + 1);  // Skip the initial '('
          }
          else
          {
            _pip_average.gridVoltage += 0;
          }
      
        val = strtok(0, " "); // Get the next value
        _pip_average.gridFrequency += atof(val) * 10 ;
      
        val = strtok(0, " "); // Get the next value
        _pip_average.acOutput += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.acFrequency += atof(val) * 10;
      
        val = strtok(0, " "); // Get the next value
        _pip_average.acApparentPower += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.acActivePower += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.loadPercent += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.busVoltage += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.batteryVoltage += atof(val)*100;
      
        val = strtok(0, " "); // Get the next value
        _pip_average.batteryChargeCurrent += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.batteryCharge += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.inverterTemperature += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        _pip_average.PVCurrent += atof(val)*10;
      
        val = strtok(0, " "); // Get the next value
        _pip_average.PVVoltage += atof(val)*10;
      
        _pip_average.PVPower += (_pip_average.PVVoltage/10) * (_pip_average.PVCurrent/10) * 10; // Calculate PV Power
      
        val = strtok(0, " "); // Get the next value
        _pip_average.batterySCC += atof(val)*100;
      
        val = strtok(0, " "); // Get the next value
        _pip_average.batteryDischargeCurrent += atoi(val);
      
        val = strtok(0, " "); // Get the next value
        strcpy(_pip_average.deviceStatus, val);
        
        if ( _inverter_protocol == 'B')   // "B" = 22 fields from QPIGS
        {
          val = strtok(0, " "); // Get the next value
          _pip_average.batOffsetFan = atoi(val);
        
          val = strtok(0, " "); // Get the next value
          _pip_average.eepromVers = atoi(val);
        
          val = strtok(0, " "); // Get the next value
          _pip_average.PV1_chargPower += atoi(val);
        
          val = strtok(0, " "); // Get the next value
          strcpy(_pip_average.deviceStatus2, String(val).substring(0,3).c_str());        }
       }
   
      //--- Prepare counting for next array posotion -------------------------------
      _average_count++;
  }
  else
  {
      //--- 10 = calculate average amounts to populate pipvals variables ---------------

        pipVals.gridVoltage             = _pip_average.gridVoltage/10;
        pipVals.gridFrequency           = _pip_average.gridFrequency/10 ;
        pipVals.acOutput                = _pip_average.acOutput/10;
        pipVals.acFrequency             = _pip_average.acFrequency/10;
        pipVals.acApparentPower         = _pip_average.acApparentPower/10;
        pipVals.acActivePower           = _pip_average.acActivePower/10;
        pipVals.loadPercent             = _pip_average.loadPercent/10;
        pipVals.busVoltage              = _pip_average.busVoltage/10;
        pipVals.batteryVoltage          = _pip_average.batteryVoltage/10;
        pipVals.batteryChargeCurrent    = _pip_average.batteryChargeCurrent/10;
        pipVals.batteryCharge           = _pip_average.batteryCharge/10;
        pipVals.inverterTemperature     = _pip_average.inverterTemperature/10;
        pipVals.PVCurrent               = _pip_average.PVCurrent/10;
        pipVals.PVVoltage               = _pip_average.PVVoltage/10;
        pipVals.PVPower                 = _pip_average.PVPower/10;
        pipVals.batterySCC              = _pip_average.batterySCC /10;
        pipVals.batteryDischargeCurrent = _pip_average.batteryDischargeCurrent/10;
        strcpy(pipVals.deviceStatus,  _pip_average.deviceStatus);     // take the lastest read string

        if ( _inverter_protocol == 'B')   // "B" = 22 fields from QPIGS
        {
          pipVals.PV1_chargPower          = _pip_average.PV1_chargPower/10;
          pipVals.batOffsetFan            = _pip_average.batOffsetFan;  // take the lastest read string
          pipVals.eepromVers              = _pip_average.eepromVers;    // take the lastest read string
          strcpy(pipVals.deviceStatus2, _pip_average.deviceStatus2);    // take the lastest read string
          
          //--- Update status2 with latest read data from PV_INVERTER ---------------------------
          store_status2 ();
        }
        
        //--- Update status with latest read data from PV_INVERTER ---------------------------
        store_status ();

        //--- RESETs the _pip_average values to not accummulate the next readings with previous ones ----
        _pip_average.gridVoltage              = 0;
        _pip_average.gridFrequency            = 0;
        _pip_average.acOutput                 = 0;
        _pip_average.acFrequency              = 0;
        _pip_average.acApparentPower          = 0;
        _pip_average.acActivePower            = 0;
        _pip_average.loadPercent              = 0;
        _pip_average.busVoltage               = 0;
        _pip_average.batteryVoltage           = 0;
        _pip_average.batteryChargeCurrent     = 0;
        _pip_average.batteryCharge            = 0;
        _pip_average.inverterTemperature      = 0;
        _pip_average.PVCurrent                = 0;
        _pip_average.PVVoltage                = 0;
        _pip_average.PVPower                  = 0;
        _pip_average.batterySCC               = 0;
        _pip_average.batteryDischargeCurrent  = 0;

         if ( _inverter_protocol == 'B')   // "B" = 22 fields from QPIGS
        {
          _pip_average.PV1_chargPower           = 0;
        }

      //--- RESETs average counting -----------------------------------------------------
      _average_count = 0;  
  }
}

void PV_INVERTER::store_status ()
{
  char val[8];
  strcpy(val, pipVals.deviceStatus);		// get the first value
  DevStatus.SBUpriority      = val[0];
  DevStatus.ConfigStatus     = val[1];		// configuration status: 1: Change 0: unchanged b6
  DevStatus.FwUpdate         = val[2];      // b5: SCC firmware version 1: Updated 0: unchanged
  DevStatus.LoadStatus       = val[3];      // b4: Load status: 0: Load off 1:Load on
  DevStatus.BattVoltSteady   = val[4];		// b3: battery voltage to steady while charging
  DevStatus.Chargingstatus   = val[5];		// b2: Charging status( Charging on/off)
  DevStatus.SCCcharge        = val[6];		// b1: Charging status( SCC charging on/off)
  DevStatus.ACcharge         = val[7];      // b0: Charging status(AC charging on/off)
}

void PV_INVERTER::store_status2 ()
{
  char val[4];
  strcpy(val, pipVals.deviceStatus2);		// get the first value
  DevStatus2.changingFloatMode			 = val[0] ;		// 10: flag for charging to floating mode
  DevStatus2.SwitchOn       				 = val[1] ;		// b9: Switch On
  DevStatus2.dustProof			  	     = val[2] ;		// b8: flag for dustproof installed(1-dustproof installed,0-no dustproof, only available for Axpert V series)
}

void PV_INVERTER::console_data()
{
  Serial.println("UNIX TIME:............ |" + String(pipVals._unixtime) + "| Epoch");
  Serial.println("Grid Voltage:......... |" + String(pipVals.gridVoltage) + "| V");
  Serial.println("Grid Frequency:....... |" + String(pipVals.gridFrequency/10.0) + "| Hz");
  Serial.println("AC Output:............ |" + String(pipVals.acOutput) + "| V");
  Serial.println("AC Frequency:......... |" + String(pipVals.acFrequency/10.0) + "| Hz");
  Serial.println("AC ApparentPower:..... |" + String(pipVals.acApparentPower) + "| VA");
  Serial.println("AC ActivePower:....... |" + String(pipVals.acActivePower) + "| W");
  Serial.println("Load Percent:......... |" + String(pipVals.loadPercent) + "| %");
  Serial.println("Bus Voltage:.......... |" + String(pipVals.busVoltage) + "| V"); 
  Serial.println("Battery Voltage:...... |" + String(pipVals.batteryVoltage/100.00)+ "| V");
  Serial.println("Battery ChargeCurrent: |" + String(pipVals.batteryChargeCurrent) + "| A"); 
  Serial.println("Battery Charge:....... |" + String(pipVals.batteryCharge) + "| %"); 
  Serial.println("PV_INVERTER Temperature:. |" + String(pipVals.inverterTemperature) + "| C"); 
  Serial.println("PV Current:........... |" + String(pipVals.PVCurrent /10.0)+ "| A");
  Serial.println("PV Voltage:........... |" + String(pipVals.PVVoltage /10.0) + "| V"); 
  Serial.println("PV Power:............. |" + String(pipVals.PVPower   /10.0) + "| W");  
  Serial.println("Battery SCC:.......... |" + String(pipVals.batterySCC/100.00) + "| V"); 
  Serial.println("Batt DischargeCurrent: |" + String(pipVals.batteryDischargeCurrent) + "| A"); 
  Serial.println("DeviceStatus:......... |" + String(pipVals.deviceStatus) + "|");
  
  if ( _inverter_protocol = B)   // "B" = 22 fields from QPIGS
  {
    Serial.println("Battery offset Fan:... |" + String(pipVals.batOffsetFan) + "| V");
    Serial.println("EEPROM Version:....... |" + String(pipVals.eepromVers) + "|");
    Serial.println("PV1 Charger Power:.... |" + String(pipVals.PV1_chargPower) + "| W");
    Serial.println("DeviceStatus2:........ |" + String(pipVals.deviceStatus2) + "|");
  }

  Serial.println("Bat Back to Grid:..... |" + String(pipVals.bat_backToUtilityVolts/10.0) + "| V"); 
  Serial.println("Bat Bulk Charge:...... |" + String(pipVals.bat_bulkChargeVolts/10.0) + "| V"); 
  Serial.println("Bat Float Charge:..... |" + String(pipVals.bat_FloatChargeVolts/10.0) + "| V"); 
  Serial.println("Bat CutOff:........... |" + String(pipVals.bat_CutOffVolts/10.0) + "| V"); 
  Serial.println("Output Priority:...... |" + String(pipVals.OutPutPriority) + "| 0: Utility first / 1: Solar first / 2: SBU first"); 
  Serial.println("Charging Priority:.... |" + String(pipVals.ChargerSourcePriority) + "| 0: Utility first / 1: Solar first / 2: Solar + Utility / 3: Only solar"); 
}

// ******************************************  CRC Functions  ******************************************
uint16_t PV_INVERTER::crc_xmodem_update (uint16_t crc, uint8_t data)
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

uint16_t PV_INVERTER::calc_crc(char *msg, int n)
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

// ******************************************  PV_INVERTER communication  *********************************

int PV_INVERTER::inverter_send(String inv_command)
{
	_streamRef->print("QRST\r");  //  knock-knock for communiction exist
	_streamRef->flush();          // Wait finishing transmitting before going on...
	if (_streamRef->readStringUntil('\r') == "(NAKss" )   // check if get response for "knock-knock" from PV_INVERTER on serial port.
	{
 /* 		uint16_t vgCrcCheck;
  		int vRequestLen = 0;
  		char s[6];
  		int xx; 
  		int yy;
  
  		vRequestLen = inv_command.length();
  		char vRequestArray[vRequestLen]; //Arrary define
  		inv_command.toCharArray(vRequestArray, vRequestLen + 1);
  
  		//Calculating CRC
  		vgCrcCheck = calc_crc(vRequestArray,vRequestLen);
  
  		// CRC returns two characters - these need to be separated and send as HEX to PV_INVERTER
  		String vgCrcCheckString = String(vgCrcCheck, HEX);
  		String vCrcCorrect = vgCrcCheckString.substring(0,2) + " " + vgCrcCheckString.substring(2,4);
  			
  		//CRC are returned as B7A9 - need to separate them and change from ASCII to HEX
  		vCrcCorrect.toCharArray(s, 6);
  		sscanf(s, "%x %x", &xx, &yy);  
  
  		inv_command += char(xx);   // add CRC byte 1
  		inv_command += char(yy);   // add CRC byte 2
 
*/
      inv_command += "\x0D";     // add CR
  		//Sending Request to PV_INVERTER
  		_streamRef->print(inv_command);
  		_streamRef->flush();          // Wait finishing transmitting before going on...
  }
  else
  {
		return -1; // No serial communication
  }
   return 0; // NAKss returned, serial communication up and running
}

int PV_INVERTER::inverter_receive( String cmd, String& str_return )
{
  if ( inverter_send(cmd)==0 )
    {
      str_return = _streamRef->readStringUntil('\x0D');
      
      // checking Command not recognized 
      if (str_return == "(NAKss") 
      {
        Serial.println("PV_INVERTER: " + cmd + ": Not recognized command: " + str_return);
        return -2;   
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
   	  return -1;
	  }
    
}

void PV_INVERTER::ask_QPIRI( String& _result)
  {
      int _funct_return = 0;
      _result = "";
      _funct_return = inverter_receive(QPIRI, _result);
      if (_funct_return == 0) 
      {
        // checking return string lengh for QPIRI command 
        if (strlen(_result.c_str()) < 85)       
        {
          Serial.println("PV_INVERTER: QPIRI: Receipt string is not completed, size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
          _result = "";                                    // clear the string result from PV_INVERTER as it is not complete
          _funct_return = -1;                              // short string lengh for QPIRI command 
        }
      }
  }

int PV_INVERTER::ask_data(uint32_t _now)
    {
      int _funct_return = 0;
      String _result = "";
      _funct_return = inverter_receive(QPIGS, _result);
      if (_funct_return == 0) 
      {
        if (_VERBOSE_MODE == 1)
          Serial.println("PV_INVERTER: QPIGS: Receipt string size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
        
        // checking return string lengh for QPIGS command 
        if (strlen(_result.c_str()) < 85)       
        {
          Serial.println("PV_INVERTER: QPIGS: Receipt string is not completed, size = |" + String(strlen(_result.c_str())) + "|.  Returned: " + _result);
          _result = "";                                  // clear the string result from PV_INVERTER as it is not complete
          _funct_return = -1;                            // short string lengh for QPIGS command 
        }
      }
      store_QPIGS(_result.c_str());                      // accumulates, average and store in pipVals the PV_INVERTER response or nothing.

      // Ask Inverer for QPIRI configuration in the 10th QPIGS reading (0 to 9)
      // (when the averaged amount will be stored in the public variables)
      if (_average_count == 9)
      {
        // Sets provided unixtime as argument for the calculated averaged 10 readings
        pipVals._unixtime = _now;
        
        // Reads QPIRI command to check battery configurations
        // TODO: Read all QPIRI fields
        
        String _QPIRI_result;
        ask_QPIRI(_QPIRI_result);

        // store QPIRI info
        store_QPIRI(_QPIRI_result);

        //--- For benchmarking the averaged Solar PV_INVERTER communication ---------------------------      
        if (_VERBOSE_MODE == 1)
          Serial.println ("Time to read, acummulate and average QPIGS info: " + String((millis() - _average_oldtime)));

        // prepare for a new banchmarck
        _average_oldtime = millis();
      }
      return (int)_funct_return;    
    }

int PV_INVERTER::handle_automation(int _hour, int _min)
    {
      String _resultado = "";
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
          if (inverter_receive(POP01, _resultado) == 0)                   
          {
             Serial.println ("--INFO: PV_INVERTER: POP01: Output Priority set to Solar/Grid/Battery");
             _POP_status = POP01;
          }
          else
          {
             // Needs to treat errors for better error messages
             Serial.println("-- ERROR: PV_INVERTER: POP01: Failed to set Output Priority to Solar/Grid/Battery. PV_INVERTER Returned: " + _resultado);       
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
          if (inverter_receive(POP02, _resultado) == 0)                   
          {
             Serial.println ("--INFO: PV_INVERTER: POP02: Output Priority set to Solar/Battery/Grid");
             _POP_status = POP02;
          }
          else
          {
            // Needs to treat errors for better error messages
             Serial.println("-- ERROR: PV_INVERTER: POP02: Failed to set Output Priority to Solar/Battery/Grid. PV_INVERTER Returned: " + _resultado);       
          }
        }
      }
    }

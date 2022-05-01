/**************************************************************************************
 * SQLITE_INVERTER.cpp (v0.1)
 * Library to be used (or not) with the PV_inverter library.
 * Created to store read data in SD card for backup purposes.
 * 
 * 
 * TODO:
 *    - Change code to enable removing and replacing
 *      SD card without turning off the ESP32;
 *    - Receive information if the data was updated on cloud or not.
 *      If not, store in a separated file the offline readings; 
 *    - When ESP32 become online again, main code will call a function 
 *      to SQLITE_INVERTER provide to WIFI module the off line readings
 *      to update the cloud server;
 *    - Create function to store QPIRI data (different file name);
 *    - Create function to store other Inverter feedback commands;
 * 
 * Contributors: Larryl79, Richardo Jr ( NiguemJr )
 * Version info: 
 *    - v0.1: Initial implementation for testing only
 **************************************************************************************/
 
#include "SQLite_inverter.h"

////////// Date and time function for error mesages //////////
extern String _errorDateTime();


void SQLITE_INVERTER::begin(uint8_t _verbose_begin)
{
    // Sharing the same verbose mode from main code
    _VERBOSE_MODE = _verbose_begin;
}

uint8_t SQLITE_INVERTER::sd_StoreQPIGS(PV_INVERTER::pipVals_t _thisPIP, bool _stored_online)
{
    /// Benchmark
    uint32_t oldtime = millis();

    //----- Prepared QPIGS INSERT SQL Statement ----------------
    String _QPIGS_line = "INSERT INTO 'QPIGS' VALUES (" +
      String(_thisPIP._unixtime)                + "," +
      String(_thisPIP.gridVoltage)              + "," +
      String(_thisPIP.gridFrequency)            + "," +
      String(_thisPIP.acOutput)                 + "," +
      String(_thisPIP.acFrequency)              + "," +
      String(_thisPIP.acApparentPower)          + "," +
      String(_thisPIP.acActivePower)            + "," +
      String(_thisPIP.loadPercent)              + "," +
      String(_thisPIP.busVoltage)               + "," +
      String(_thisPIP.batteryVoltage)           + "," +
      String(_thisPIP.batteryChargeCurrent)     + "," +
      String(_thisPIP.batteryCharge)            + "," +
      String(_thisPIP.inverterTemperature)      + "," +
      String(_thisPIP.PVCurrent)                + "," +
      String(_thisPIP.PVVoltage)                + "," +
      String(_thisPIP.PVPower)                  + "," +
      String(_thisPIP.batterySCC)               + "," +
      String(_thisPIP.batteryDischargeCurrent)  + "," +
      String(_thisPIP.deviceStatus[0])          + "," +
      String(_thisPIP.deviceStatus[1])          + "," +
      String(_thisPIP.deviceStatus[2])          + "," +
      String(_thisPIP.deviceStatus[3])          + "," +
      String(_thisPIP.deviceStatus[4])          + "," +
      String(_thisPIP.deviceStatus[5])          + "," +
      String(_thisPIP.deviceStatus[6])          + "," +
      String(_thisPIP.deviceStatus[7])          + "," +
      String(_thisPIP.batOffsetFan)             + "," +
      String(_thisPIP.eepromVers)               + "," +
      String(_thisPIP.PV1_chargPower)           + "," +
      String(_thisPIP.deviceStatus2[0])         + "," +
      String(_thisPIP.deviceStatus2[1])         + "," +
      String(_thisPIP.deviceStatus2[2])         +
      ");";
      
    if (_VERBOSE_MODE == 1) Serial.println(_errorDateTime() +"-- - VERBOSE: SQLITEr: SQL Cmd line: |" + _QPIGS_line + "|END.");
    

    // Run SQL Insert statement 
    rc = db_exec(db1, _QPIGS_line.c_str());
    if (rc != SQLITE_OK) 
    {
       Serial.println(_errorDateTime() +"--- ERROR: SQLITE: INSERT SQL Cmd error code: " + String(rc));
       sqlite3_close(db1);
       
       return 1;
    }
    
    if (_VERBOSE_MODE == 2) Serial.println(_errorDateTime() +"--- VERBOSE: SQLITE: information INSERTed in the database row: |" + String((long)sqlite3_last_insert_rowid(db1)) + "|END.");
    if (_VERBOSE_MODE == 1) Serial.println(_errorDateTime() +"--- VERBOSE: SQLITE: MEM USED: |" + String((long)sqlite3_memory_used()) + "|END.");
    if (_VERBOSE_MODE == 1) Serial.println(_errorDateTime() +"--- VERBOSE: SQLITE: MEM HighWater: |" + String((long)sqlite3_memory_highwater(1)) + "|END.");

    //sqlite3_close(db1);

  /* /////// SAMPLE CODE FOR SQLite3 SELECT STATEMENT /////////////////
    
    // Clears previous Select results from RES pointer
    sqlite3_finalize(res);
    rc = sqlite3_prepare_v2(db1, "Select * from 'QPIGS'"  , 1000, &res, &tail);
    bool first = true;
    uint32_t rows = 0;
    int _count = 0;
    while (sqlite3_step(res) == SQLITE_ROW) 
    {
//        Serial.println("====================================================================================================");
        if (first) 
        {
          _count = sqlite3_column_count(res);
//          Serial.println("COUNT: " + String(_count));
          
          if (_count == 0) 
          {
 //           Serial.printf("Rec Count: %s\n", sqlite3_changes(db1));
              break;
          }
          for (int i = 0; i<_count; i++) 
          {
 //           Serial.printf("Column Name: %s\n", sqlite3_column_name(res, i));
          }
          first = false;
        }
        _count = sqlite3_column_count(res);
        int32_t test[_count];
        for (int i = 0; i<_count; i++) 
        {
          test[i] =sqlite3_column_int(res, i);
//          Serial.println("Column num: " + String(i) + " | Data: " + String(test[i]));
        }
        rows ++;

    }

   Serial.println("Columns: "+String(_count)+" | rows: "+String(rows)+" elapsed time:" + String(millis() - oldtime));
   sqlite3_close(db1);
*/    
     
  
/*  
  //------------   Check if there is SD and available size ----------------------
  if (card_inserted() != 0) 
  {
    Serial.println(_errorDateTime() + "-- ERROR: SQLITE: QPIGS not stored in SD Card! -----");
    return 1;
  }

  // Convert unixtime from smoothed read to readable date to define file name
  ts = *localtime((time_t*)_thisPIP._unixtime);
  
  //------- file name ----------------

  String _file = "/"+String(ts.tm_year+1900)+String(ts.tm_mon+1)+String(ts.tm_mday)+"_QPIGS.csv";

  //----- Prepared QPIGS string for writing ----------------
  String _QPIGS_line = 
      String(_thisPIP._unixtime)                + "," +
      String(_thisPIP.gridVoltage)              + "," +
      String(_thisPIP.gridFrequency)            + "," +
      String(_thisPIP.acOutput)                 + "," +
      String(_thisPIP.acFrequency)              + "," +
      String(_thisPIP.acApparentPower)          + "," +
      String(_thisPIP.acActivePower)            + "," +
      String(_thisPIP.loadPercent)              + "," +
      String(_thisPIP.busVoltage)               + "," +
      String(_thisPIP.batteryVoltage)           + "," +
      String(_thisPIP.batteryChargeCurrent)     + "," +
      String(_thisPIP.batteryCharge)            + "," +
      String(_thisPIP.inverterTemperature)      + "," +
      String(_thisPIP.PVCurrent)                + "," +
      String(_thisPIP.PVVoltage)                + "," +
      String(_thisPIP.PVPower)                  + "," +
      String(_thisPIP.batterySCC)               + "," +
      String(_thisPIP.batteryDischargeCurrent)  + "," +
      _thisPIP.deviceStatus[0]                  + "," +
      _thisPIP.deviceStatus[1]                  + "," +
      _thisPIP.deviceStatus[2]                  + "," +
      _thisPIP.deviceStatus[3]                  + "," +
      _thisPIP.deviceStatus[4]                  + "," +
      _thisPIP.deviceStatus[5]                  + "," +
      _thisPIP.deviceStatus[6]                  + "," +
      _thisPIP.deviceStatus[7]                  + "," +
      String(_thisPIP.batOffsetFan)             + "," +
      String(_thisPIP.eepromVers)               + "," +
      String(_thisPIP.PV1_chargPower)           + "," +
      _thisPIP.deviceStatus2[0]                 + "," +
      _thisPIP.deviceStatus2[1]                 + "," +
      _thisPIP.deviceStatus2[2];

    if (_VERBOSE_MODE == 1)
    {
      Serial.println(_errorDateTime() + "-- VERBOSE: SQLITE: File name: " + _file);
      Serial.println(_errorDateTime() + "-- VERBOSE: SQLITE: QPIGS String |" + _QPIGS_line + "|");
    }

  appendFile(SD, _file.c_str(), _QPIGS_line.c_str());

  */
  return 0;

}


/// PRIVATE FUNCTIONS ///////////////////////////////////////////////////


//// START SQLite3 //////////////////////
int SQLITE_INVERTER::callback(void *data, int argc, char **argv, char **azColName){
   int i;
   Serial.println(_errorDateTime() +"-- VERBOSE: SQLITE: SQLite callback: " + String((const char*)data));
/*   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   */
   return 0;
}

int SQLITE_INVERTER::openDb(const char *filename, sqlite3 **db) {
   int _result = sqlite3_open(filename, db);
   if (_result) {
       Serial.println(_errorDateTime() +"-- ERROR: SQLITE: SQL error: " + String(sqlite3_errmsg(*db)));
       return _result;
   } else {
       if (_VERBOSE_MODE == 1) Serial.println(_errorDateTime() +"-- VERBOSE: SQLITE: Open database successfully");
   }
   return _result;
}

int SQLITE_INVERTER::db_exec(sqlite3 *db, const char *sql) {
   //Serial.println(sql);
   uint32_t start = millis();
   int _result = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (_result != SQLITE_OK) {
       Serial.println(_errorDateTime() +"-- ERROR: SQLITE: SQL error: " + String(zErrMsg));
       sqlite3_free(zErrMsg);
   } else {
       if (_VERBOSE_MODE == 1) Serial.println(_errorDateTime() +"--  VERBOSE: SQLITE: db_exec function ran SQL Statement successfully");
   }
   if (_VERBOSE_MODE == 2)
   {
     Serial.print("Time taken:");
     Serial.println(millis()-start);
   }
   return _result;
}

//// END SQLite3 //////////////////////


//// SD Card File manipulation //////////////////////
uint8_t SQLITE_INVERTER::card_inserted()
{
    if(SD.cardType() == CARD_NONE){
        Serial.println(_errorDateTime() + "-- ERROR: SQLITE: No SD card attached.");
        return 1;
    }
    
    // --- only 0.5mb available ERROR
    if((SD.cardSize() - SD.usedBytes()) < (500 * 1024) )
    {
        Serial.println(_errorDateTime() + "--- ERROR: SQLITE: less than 500kb available, STORE function not executed!");
        return 2;
    }

    // --- less then 100mb available warning
    if((SD.cardSize() - SD.usedBytes()) < (100 * 1024 * 1024) )
    {
        Serial.println(_errorDateTime() + "--- WARNING: SQLITE: less than 100mb available, please change SD Card!");
    }    
    
    if (_VERBOSE_MODE == 1)
    {
      Serial.print(_errorDateTime() + "--- VERBOSE: SQLITE_INVERTER: SD Card Size: ");
      Serial.print(SD.cardSize()/1024/1024.00);
      Serial.print(" Mb | Available: ");
      Serial.print((SD.cardSize() - SD.usedBytes()) /1024/1024.00);
      Serial.print(" Mb | Used: ");
      Serial.print(SD.usedBytes() /1024/1024.00);
      Serial.println(" Mb ");
    }
    return 0;
}


// Missing better implementation: Read line by line into an feedback array

void SQLITE_INVERTER::readFile(fs::FS &fs, const char * path){
   File file = fs.open(path);
    if(!file){
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to open the file for reading: ");
        Serial.println(path);
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void SQLITE_INVERTER::writeFile(fs::FS &fs, const char * path, const char * message)
{
    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to open file for writing: ");
        Serial.println(path);
        return;
    }
    if(!file.print(message))
    {
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to write data in the file: ");
        Serial.println(path);
    }
    file.close();
}

void SQLITE_INVERTER::appendFile(fs::FS &fs, const char * path, const char * message)
{
    File file = fs.open(path, FILE_APPEND);
    if(!file)
    {
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to open file for appending: ");
        Serial.println(path);
        return;
    }
    if(!file.println(message))
    {
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to append data in the file: ");
        Serial.println(path);
    }
    file.close();
}

void SQLITE_INVERTER::deleteFile(fs::FS &fs, const char * path)
{
    if(!fs.remove(path))
    {
        Serial.print("--- ERROR: SQLITE_INVERTER: Failed to delete file: ");
        Serial.println(path);
    }
}

//// END SD Card File manipulation //////////////////////

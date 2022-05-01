#include "webserver_inverter.h"

////////// Date and time function for error mesages //////////
extern String _errorDateTime();


void WEBSERVER_INVERTER::begin(String _ssid, String _password, PV_INVERTER::pipVals_t *_thisPIP)
{
      //--- Initialize ESP SPIFFS (flash filesystem) to recover the index.html file --------
      if(!SPIFFS.begin()){
        Serial.println(_errorDateTime() + "--- ERROR: WERBERSER_INVERTER: An Error has occurred while mounting SPIFFS");
        return;
      }
      //--- Connect to Wi-Fi ---------------------
      WiFi.begin(_ssid.c_str(), _password.c_str());
      Serial.print("Connecting to WiFi.");
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(100);
        Serial.print(".");
      }
      Serial.println(" CONECTED !");
    
      //--- Print ESP32 Local IP Address  ------------
      Serial.println(WiFi.localIP());

  
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html");
  });

  //--- PV CHARGER POWER from pipvals  -----------------

  _server.on("/PVPower", HTTP_GET, [_thisPIP](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(_thisPIP->PV1_chargPower).c_str() );
  });

  //--- ACTIVE POWER from pipvals ---------------------
  _server.on("/ActivePower", HTTP_GET, [_thisPIP](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(_thisPIP->acActivePower).c_str());
  });

  //--- BATTERY VOLTAGE from pipvals ---------------------
  _server.on("/BatVoltage", HTTP_GET, [_thisPIP](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(_thisPIP->batteryVoltage/100.00).c_str());
  });

  //--- BATTERY CHARGE from pipvals ---------------------
  _server.on("/BatCharge", HTTP_GET, [_thisPIP](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(_thisPIP->batteryChargeCurrent).c_str());
  });

  //--- BATTERY DISCHARGE from pipvals ---------------------
  _server.on("/BatDischarge", HTTP_GET, [_thisPIP](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(_thisPIP->batteryDischargeCurrent).c_str());
  });

  //--- Start web server ------------
  _server.begin();

}

#include "Arduino_Otahelper.h"

#if defined(ESP32_RTOS) && defined(ESP32)
void ota_handle( void * parameter ) {
  for (;;) {
    ArduinoOTA.handle();
    delay(3500);
  }
}
#endif

void ota_init(const char* nameprefix) {

  //uint16_t maxlen = strlen(nameprefix) + 7;
  //char *fullhostname = new char[maxlen];

  //const char* fullhostname ="ESP32-OTA" + nameprefix;
  ArduinoOTA.setHostname(nameprefix);
  //delete[] fullhostname;


  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start OTA updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd OTA UPDATE.");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("\nOTA Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("\nOTA Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("\nOTA Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("\nOTA Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("\nOTA End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("OTA Initialized");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println("");

#if defined(ESP32_RTOS) && defined(ESP32)
  TaskHandle_t OTATaskHandle;

  xTaskCreate(
    ota_handle,         /* Task function. */
    "OTA_HANDLE",       /* String with name of task. */
    10000,              /* Stack size in bytes. */
    NULL,               /* Parameter passed as input of the task */
    1,                  /* Priority of the task. */
    &OTATaskHandle );   /* Task handle. */
#endif
}

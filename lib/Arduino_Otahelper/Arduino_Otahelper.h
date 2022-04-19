#ifndef OTAHELPER_H
 #define OTAHELPER_H
 
 #ifndef OTA_HOST
  #define OTA_HOST "Esp32-OTA"
 #endif

#include <ArduinoOTA.h>

void ota_init();

#endif

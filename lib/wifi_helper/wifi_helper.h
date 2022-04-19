#ifndef WIFI_HELPER_H
 #define WIFI_HELPER_H
 #endif
 #ifdef ESP32
   #include <WiFi.h>
   #include <ESPmDNS.h>
 #else 
   #include <ESP8266WiFi.h>
   #include <ESP8266mDNS.h>
 #endif

 #ifndef AP_SSID
   #pragma message("No AP_SSID configured")
   #define AP_SSID "Arduino-SoftAP"      // Set AP default SSID
 #else
   #define AP_ON
 #endif
 #ifndef AP_PASS
   #pragma message("No AP_PASS configured")
   #define AP_PASS "Arduino"             //set AP default password
 #endif
 
 #ifndef CLIENTSSID
   #pragma message("No CLIENTSSID configured")
   #define CLIENTSSID ""  
 #endif
 #ifndef CLIENTPASSWORD
   #pragma message("No CLIENTPASSWORD configured")
   #define CLIENTPASSWORD "" 
 #endif
 #ifndef HOSTNAME
   #pragma message("No HOSTNAME configured")
   #define HOSTNAME "Arduino" 
 #endif
 
 
 #include <WiFiUdp.h>

 void wifi_init();
 
 int _wifiConnectRetry = 3;

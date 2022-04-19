#ifndef SERIAL_COMMAND_H
#define SERIAL_COMMAND_H
#include <Arduino.h>


void helpmsg();
void keycommand();


int incomingByte = 0;
#endif

void keycommand()
{

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("");
    Serial.print("Command received: ");
    //Serial.println(incomingByte, DEC);
    Serial.println((char)(incomingByte));

    switch (incomingByte)
    {
    case 'R' :
      ESP.restart();
      break;
    
    case 'I' :
      Serial.print(AP_SSID);
      Serial.print("       Password: ");
      Serial.print(AP_PASS);
	
      Serial.print("       AP IP: ");
      Serial.println(WiFi.softAPIP());

      //connect WIfi-Client
      Serial.println("Intit Wifi...    SSID: " + String(CLIENTSSID) + "    Password: " + String(CLIENTPASSWORD));
      
      Serial.print("IP address: ");   Serial.println(WiFi.localIP());
      Serial.print("MASK: ");         Serial.println(WiFi.subnetMask());
      Serial.print("GATEWAY: ");      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");          Serial.println(WiFi.dnsIP());
      Serial.println("");
      break;
    

    case 'h' :
    //default:
       helpmsg();
      break;
    }
  }
}

void helpmsg()
{
  Serial.println("");
  Serial.println("char    command description");
  Serial.println("R       Reset ESP");
  Serial.println("I       IP configuration");

  Serial.println("h       this help");

}

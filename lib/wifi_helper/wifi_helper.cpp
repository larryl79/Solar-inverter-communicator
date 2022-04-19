/*
*
*  Starts wifi client connection, and tries 3 times to connect,
*  then displays IP information on serial comsole.
*  
*  ver: 1.0
*/

#include "wifi_helper.h"

void wifi_init() 
{

   /*
  gateway = WiFi.gatewayIP();
  Serial.print("GATEWAY: ");
  Serial.println(gateway);
  WiFi.config(ip, dns, gateway, subnet);
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

     */
  //wifi init
 // Start AP mode first.
    /*
     uint8_t macAddr[6];
     WiFi.softAPmacAddress(macAddr);
     Serial.printf("MAC address = %02x:%02x:%02x:%02x:%02x:%02x\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);

     IPAddress local_IP(192,168,4,22);
     IPAddress gateway(192,168,4,9);
     IPAddress subnet(255,255,255,0);
     Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
     WiFi.softAPdisconnect(wifioff);
     */
    //WiFi.softAP(ssid, psk, channel, hidden, max_connection)
    //softAPConfig (local_ip, gateway, subnet)

/*
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("Iinit AP mode... SSID: ");
    Serial.print(AP_SSID);
    Serial.print("       Password: ");
    Serial.print(AP_PASS);
	
    Serial.print("       AP IP: ");
    Serial.println(WiFi.softAPIP());
*/
   //connect WIfi-Client
   Serial.println("Intit Wifi...    SSID: " + String(CLIENTSSID) + "    Password: " + String(CLIENTPASSWORD));
   WiFi.setHostname(HOSTNAME);
   WiFi.begin(CLIENTSSID, CLIENTPASSWORD);
   
   while (WiFi.waitForConnectResult() != WL_CONNECTED and _wifiConnectRetry<=3) {
    WiFi.begin(CLIENTSSID, CLIENTPASSWORD);
    Serial.println("Retrying connection... "+String(_wifiConnectRetry));
    delay(500);
    _wifiConnectRetry++; 
  }
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
   {
    //wifiok=false;
    WiFi.disconnect(true);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.println("Wifi fail. AP mode on.");
    //delay(2000);
    //wifiok=true;
   }
   else
   {
    delay(500);
    Serial.println("Wifi connected.");
    Serial.print("IP address: ");   Serial.println(WiFi.localIP());
    Serial.print("MASK: ");         Serial.println(WiFi.subnetMask());
    Serial.print("GATEWAY: ");      Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: ");          Serial.println(WiFi.dnsIP());
    delay(2000);
    _wifiConnectRetry=0;
   }

  if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("http", "udp", 1900);
    //MDNS.addService("_http", "tcp", 1901);

}

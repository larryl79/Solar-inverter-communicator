#include <SimpleHeartBeat.h>

HeartBeat heartbeat(2);           // create instance named as heartbeat and give pin nuber for the (internal) led.

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Starting demo.");
  }

void loop()
{
Serial.println("simply turn on led");
heartbeat.ledon();      //simply turn on led
delay(4000);

Serial.println("simply turn off led");
heartbeat.ledoff();     //simply turn off led
delay(4000);

Serial.println("Old style blink with static delay times.");
for (int i =0; i<50; i++)
 {
heartbeat.run();        // old blink example using delay staement, may will be removed in the future.
//your code in loop
//end of your code in loop
 }
delay(2000);

Serial.println("toggle led state. State before toggle:" + String(heartbeat.getstate()));
heartbeat.ledtoggle();  // change state of led on->off and off->on
delay(2000);

Serial.println("toggle led state again. State before toggle:" + String(heartbeat.getstate()));
heartbeat.ledtoggle();  // change state of led on->off and off->on
delay(2000);

Serial.println("simple blinking with a delay same of on/off time. toggle after 500 ticks");
for (int i =0; i<50; i++)
 {
 heartbeat.blink(500);        //simple blink with a delay same of on/off time. It's useful to don't pause the running code, call this rapidly in your code.
 //your code in loop
 delay(100);
 //end of your code in loop
 }
delay(2000);

Serial.println("simple blinking with a delay same of on/off time. toggle on after 300 ticks, turn off after 500 ticks");
for (int i =0; i<50; i++)
 {
 heartbeat.blinkasync(300, 500);    //simple blink with a different delay time of on/off. It's useful to don't pause the running code, call this rapidly in your code.
 //your code in loop
 delay(100);
 //end of your code in loop
 } 
delay(2000);
Serial.println("Restarting demo");
heartbeat.ledoff();
delay(3000);
}

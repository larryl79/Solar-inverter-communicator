#ifndef SIMPLEHEARTBEAT_H
#define SIMPLEHEARTBEAT_H
#include <Arduino.h>

class HeartBeat
{
public:
    HeartBeat(int pin);
    void begin(int pin);
    void run();
    void ledtoggle();
    void ledoff();
    void ledon();
    void blink(int delay);
    void blinkasync(int delayon, int delayoff);
	bool getstate();
 
private:
  int _heartbeatpin = 2;
  int _state = 0;
  unsigned long _currentMillis = millis();
  unsigned long _previousMillis = 0;
};

#endif   // SIMPLEHEARTBEAT_H
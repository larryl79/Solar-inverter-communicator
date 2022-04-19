<H1>SimpleHeartBeat</h1>

This library helps make simple heartbeat sunction to your application

<h2>Installation</h2>

1. Simply copy a downloaded master into your Arduino IDE library folder, and Rename it to SimpleHeartBeat form SimpleHeartBeat-master

2. Load your program, and put "#include <SimpleHeartBeat.h>", into your first lines in your program without qoutes.
3. e.g. Put "HeartBeat heartbeat(2);" into your code before setup for name your instane as heartbeat, and use pin 2 for your led.
4. start using lib even in setup() or loop(). e.g.: heartbeat.ledtoggle();

<H2>Commands</h2>

    HeartBeat(int pin);                              // constructor, define pin
    void begin();                                    // doesn't need it really, considering to remove.
    void run();                                      // 1x on and 1x off LED, using delay statement with a static time.
    void ledtoggle();                                // toggle state of LED
    void ledoff();                                   // turn off LED
    void ledon();                                    // turn on LED
    void blink(int delay);                           // blink whith given time
    void blinkasync(int delayon, int delayoff);      // blink with on and off time
    bool getstate();                                 // returns a state of the led. 
    
See example folder for a demo.


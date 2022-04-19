# Inverter-Communicator
Solar inverter communicator with ESP32

This application based on ESP32, using Serial2 or SoftwareSerial for communicate with MPP Solar inverters, PowMR and another China brands.

code starts with src/main.cpp

not all files are in use (yet)!

Used modules:
- ESP32
- MAX232 DB9 Male
- I2C 2004 LCD

Code editor: VSCode

Stage: 
- Establsihed communication with inverter, show basic info values (QPIGS) on LCD 
- LCD menu added, woking either rotary switch, either 3 buttons.
- reforming code for add next commands to the menu, and print results on LCD, and Serial Console.

Future plans:
- implemant all query commands
- implement configuration commands
- web server
- inverter logging
- multiple inverter conncetion

Useful materials:
- Documents folder
- https://forum.arduino.cc/t/rs232-read-data-from-mpp-solar-inverter/600960

My Mpp Solar device: PS-300VA-24V (PowMr)
https://sunrayups.en.made-in-china.com/product/vSFmfUxjZNpc/China-New-Model-2400W-230VAC-24VDC-PWM-50A-High-Frequency-Pure-Sine-Wave-off-Grid-Hybrid-Solar-Inverter-HPS-3K-.html


Contribuitors, Developers are welcome

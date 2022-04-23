#include <SPI.h>
#include <SD.h>

File myFileIn;
File myFileOut;

void setup() {
  Serial.begin(115200);
  if (!SD.begin(4)) {
    Serial.println(F("initialization failed!"));
  }
  if (!SD.exists("fromFile.txt")) {
    myFileIn = SD.open("fromFile.txt", FILE_WRITE);
    myFileIn.println(F("one two three"));
    myFileIn.println(F("four five six"));
    myFileIn.close();
  }
  if (SD.exists("toFile.txt")) {
    SD.remove("toFile.txt");
  }
  myFileIn = SD.open("fromFile.txt", FILE_READ);
  myFileOut = SD.open("toFile.txt", FILE_WRITE);
  while (myFileIn.available()) {
    myFileOut.write(myFileIn.read());
  }
  myFileIn.close();
  myFileOut.close();
  myFileIn = SD.open("toFile.txt", FILE_READ);
  while (myFileIn.available()) {
    Serial.write(myFileIn.read());
  }
  myFileIn.close();
}

void loop() {}
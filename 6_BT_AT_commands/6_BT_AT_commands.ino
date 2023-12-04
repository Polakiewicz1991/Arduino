#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <PpBluetooth.h>

SoftwareSerial BtDevice(10,11); //RX / TX

void setup() {

  //Serial monitor - Initialization
  Serial.begin(9600);
  while (!Serial) {
    //wait for connection witch Serial Monitor
  }
  BtDevice.begin(9600);
  while (!BtDevice) {
    //wait for connection witch Serial Monitor
  }
  Serial.print("Initialization done");
}

void loop() {


  if (Serial.available() > 0) {
    BtDevice.write(Serial.read());
  }
   if (BtDevice.available() > 0) {
    Serial.write(BtDevice.read());
  }
}





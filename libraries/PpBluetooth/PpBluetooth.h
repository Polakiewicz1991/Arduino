#ifndef PpBluetooth_h
#define PpBluetooth_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class PpBluetooth {
public:
	PpBluetooth(int iBaud, int rxPin, int txPin); // Konstruktor
  
	void ble_help();
private:
	SoftwareSerial ble_device;
	String str_ii = "";
	int ii_0 = 0;
};

#endif

#include "PpBluetooth.h"

PpBluetooth::PpBluetooth(int iBaud, int rxPin, int txPin) : ble_device(rxPin, txPin) {
   ble_device.begin(iBaud);
}

void PpBluetooth::ble_help() {
	
  ble_device.println("AT+HELP"); // list all AT+ commands
  while (true){ // loop to print all AT+ commands
    char in_char = ble_device.read();
    if (int(in_char)==-1 or int(in_char)==42){continue;}
    str_ii+=in_char;
    if (in_char=='\n'){
      if (str_ii==String('\r')+String('\n')){
        if (ii_0 == 0){
          ii_0 = 1;
          continue;
        }
        break; // break after more than 1 empty carriage return and newline
      }
      Serial.print(str_ii);
      str_ii = "";
    }
  }
}
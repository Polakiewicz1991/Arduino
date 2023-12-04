#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <PpBluetooth.h>

//Hardware controll - cycle time
const int iCycleTime = 100;

//Hardware Liquid Crystal Display - configuration
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5,d6, d7);
//Software Liquid Crystal Display - variables declaration
int iDisplayX = 1,  iDisplayY = 1, iCursor = 0;
bool bSwitchMemoX = false, bSwitchMemoY = false;
String sDisplayLines[10][10];

//Hardware JoyStick - configuration
const int x_pin = A0, y_pin = A1, sw_pin = 8;
//Software JoyStick - variables declaration
int x_data, y_data, sw_data, x_data_raw, y_data_raw;
/*
//Hardware Servo 9g - configuration
Servo miniServo9g;
const int servoPin = 9;
//Software Servo 9g - variables declarations
int iServoPos = 90;
*/
//Hardware Bluetooth module - Initialization
//PpBluetooth BtDevice(9600, 11, 12);
SoftwareSerial BtDevice(10,11); //RX / TX
String receivedBtDeciceString = "";
String receivedSerialString = "";
int receivedDataLenght = 0;

void setup() {
  //Flashing diode - Initialization
  pinMode(13, OUTPUT);
  pinMode(sw_pin, INPUT);
  digitalWrite(sw_pin, HIGH);

  //Servo 9g - Initialization
  x_data_raw = analogRead(x_pin);
  y_data_raw = analogRead(y_pin);

  //LCD - Initaialization
  lcd.begin(16, 2); //inicjalizacja wyświetlacza o liczbie zanków 16 i 2 iniach
  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.cursor();
  /*
  //Mini servo 9g - Initialization
  miniServo9g.attach(servoPin);
  miniServo9g.write(iServoPos);
  */
  //Serial monitor - Initialization
  Serial.begin(9600);
  while (!Serial) {
    //wait for connection witch Serial Monitor
  }
  //Bluetooth - Initalization
  BtDevice.begin(9600);
  pinMode(12, OUTPUT); //Eable bit


  while (!BtDevice) {
    //wait for connection witch HM-10 module
  }

  //Serial.print("Initialization done \n");

}

/* Opowiedź na pytanie czemu komendy AT nie dziłają
https://forum.arduino.cc/t/hm-10-bluetooth-module-is-not-responding-to-at-commands-with-arduino-uno/641873
Quite a lot, but nothing damaging. These modules have two exclusive modes:
AT mode - by default
Bluetooth Communications mode(i.e. wireless) by initiation.
This means you can only be in AT mode when you are not communicating. Result? you can only send AT modes from Arduino, not your Android app.
*/

void loop() {

  if (BtDevice.available()) {
    char receivedCharBtDevice = BtDevice.read();
    receivedBtDeciceString += receivedCharBtDevice;

    if ((receivedCharBtDevice == '\n') or (BtDevice.available() == 1)){

      Serial.println(receivedBtDeciceString);

      // Wyczyść łańcuch po przetworzeniu
      receivedBtDeciceString = "";

    }
    //Serial.print(receivedChar);  // Wyświetl odczytane dane z modułu BT w monitorze szeregowym Arduino
  }

  if (Serial.available() > 0) {
    if (Serial.available() > receivedDataLenght){
      receivedDataLenght = Serial.available();
    }

    
    char receivedCharSerial = Serial.read();
    Serial.print(receivedCharSerial);

    // Dodaj znaki do ciągu znaków, aż napotkasz znak nowej linii
    receivedSerialString += receivedCharSerial;
     
  } else if ((Serial.available() == 0) and (receivedSerialString.length() > 0)){

      if (receivedSerialString.indexOf("AT ON") == 0){
        digitalWrite(12, HIGH);
        Serial.println("Tryb AT ON");

      } else if(receivedSerialString.indexOf("AT OFF") == 0){
        digitalWrite(12, LOW);      
        Serial.println("Tryb AT OFF");

      } else if(receivedSerialString.indexOf("send AT+HELP?") == 0){      
        Serial.println("Komenda AT+HELP?");
        BtDevice.print("at+help?");

      } else if(receivedSerialString.indexOf("send AT") == 0){      
        Serial.println("Komenda AT");
        BtDevice.print("at");

      } else {
        //char receivedCharSerial = Serial.read();
        //receivedSerialString += receivedCharSerial;
        BtDevice.print(receivedSerialString); 
        BtDevice.print("Recived string lenght: ");
        BtDevice.print(receivedDataLenght); 
        BtDevice.print("\n");

        // Wyczyść łańcuch po przetworzeniu
      }
      receivedSerialString = "";
      receivedDataLenght = 0;

  }
}





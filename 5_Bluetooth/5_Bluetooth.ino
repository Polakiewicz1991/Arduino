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

//Hardware Servo 9g - configuration
Servo miniServo9g;
const int servoPin = 9;
//Software Servo 9g - variables declarations
int iServoPos = 90;

//Hardware Bluetooth module - Initialization
//PpBluetooth BtDevice(9600, 11, 12);
SoftwareSerial BtDevice(10,11); //RX / TX

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

  //Mini servo 9g - Initialization
  miniServo9g.attach(servoPin);
  miniServo9g.write(iServoPos);

  //Serial monitor - Initialization
  Serial.begin(9600);
  delay(1000);
  //Bluetooth - Initalization
  BtDevice.begin(9600);
 //BtDevice.ble_help();
  delay(1000); 

  Serial.print("Initialization done \n");

}

void loop() {
/*
  digitalWrite(13, HIGH);
  delay(iCycleTime);


  //Prit serial message
  if (Serial.available() > 0) {
    // Odczytanie pojedynczego znaku
    char receivedChar = Serial.read();

    // Wyświetlenie odczytanego znaku w monitorze szeregowym
    Serial.print("Odczytano:\n");
    Serial.println(receivedChar);

    if(receivedChar == '1'){
      char chMessage[] = "AT";
      Serial.print("Wysłano dane AT \n");
      // BtDevice.write(chMessage,sizeof(chMessage));
      BtDevice.write("AT");

      delay(100);
      int iAvailable = BtDevice.available();
      Serial.print("W buforze znajduje się : ");
      Serial.print(iAvailable);      
      Serial.print(" znaków.\n");

    }
    
  }

  // Odczytaj wszystkie dostępne znaki
  if (BtDevice.available() > 0){
      Serial.print("Odczytano: ");
  }
  while (BtDevice.available()) {

    /*int iAvailable = BtDevice.available();
    Serial.print("W buforze znajduje się : ");
    Serial.println(iAvailable);      
    Serial.print(" znaków.\n");

    char receivedChar = BtDevice.read();

    if (isdigit(receivedChar)) {
      int receivedInt = atoi(&receivedChar);
      Serial.print("Odczytano jako int: ");
      Serial.println(receivedInt);
    } else {
      int asciiValue = int(receivedChar);
      Serial.print("Odczytano jako wartość ASCII: ");
      Serial.println(asciiValue);      
    }
  
    Serial.print("\n\n");

  char receivedChar = BtDevice.read();
  Serial.print(receivedChar);  

  
  Serial.println(BtDevice.available());
  digitalWrite(13, LOW);
  delay(iCycleTime);    
  */
  if (BtDevice.available()) {
    char receivedChar = BtDevice.read();
    Serial.print(receivedChar);  // Wyświetl odczytane dane z modułu BT w monitorze szeregowym Arduino
  }

  if (Serial.available()) {
    char charToSend = Serial.read();
    BtDevice.print(charToSend);  // Wyślij dane z monitora szeregowego do modułu BT
  }
}



//Funkcje



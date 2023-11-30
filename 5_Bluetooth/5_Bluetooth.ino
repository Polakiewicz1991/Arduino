#include <LiquidCrystal.h>
#include <Arduino_JSON.h>
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
PpBluetooth BtDevice(9600, 11, 12);

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
  //LCD - Initialization displayed data
  initDisplayStrings();

  //Mini servo 9g - Initialization
  miniServo9g.attach(servoPin);
  miniServo9g.write(iServoPos);

  //Serial monitor - Initialization
  Serial.begin(115200);
  delay(1000);
  //Bluetooth - Initalization
  BtDevice.ble_help();
  delay(1000); 

}

void loop() {

  digitalWrite(13, HIGH);
  delay(iCycleTime);

  readAnalogData();
  //writeSerialData();
  changeDisplayData();
  displayData();

  //Prit serial message
  Serial.print("Serial 115200 baud OK \n");


  digitalWrite(13, LOW);
  delay(iCycleTime);

}

//Funkcje
void readAnalogData(){
  //read joystick data
  x_data = analogRead(x_pin) - x_data_raw;
  y_data = analogRead(y_pin) - y_data_raw;
  sw_data = digitalRead(sw_pin);}
void initDisplayStrings() {
  for (int i = 0; i <= 9; ++i) {
    for (int j = 0; j <= 9; ++j) {
      sDisplayLines[i][j] = String(char('A' + i)) + " " + String(char('A' + j));
    }
  }}
void changeDisplayData(){
  // change display data
  if (x_data > 300) {
    if (!bSwitchMemoX) {
      //X data
      if (iCursor == 4){
        iDisplayX--;
      }
        //Y data
     if (iCursor == 5){
        iDisplayY--;
      }
    }
    bSwitchMemoX = true;
  } else if (x_data < -300) {
    if (!bSwitchMemoX) {
      //X data
      if (iCursor == 4){
        iDisplayX++;
      }
       //Y data
      if (iCursor == 5){
        iDisplayY++;
      }
    }
    bSwitchMemoX = true;
  } else {
    bSwitchMemoX = false;
  }
 
  if (y_data > 300) {
    if (!bSwitchMemoY) {
      iCursor++;
    }
    bSwitchMemoY = true;
  } else if (y_data < -300) {
    if (!bSwitchMemoY) {
      iCursor--;
   }
    bSwitchMemoY = true;
  } else {
    bSwitchMemoY = false;
  }

  // iDisplayX range {1:9}, not 10 becouse 2 lines on display {9,10}
  if (iDisplayX < 1) {
    iDisplayX = 1;
  } else if (iDisplayX > 9) {
    iDisplayX = 9;
  }
  if (iDisplayY < 1) {
    iDisplayY = 1;
  } else if (iDisplayY > 10) {
    iDisplayY = 10;
  }}
void displayData(){

  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.clear();
  lcd.print("XY: ");
  lcd.print(iDisplayX);
  lcd.print(iDisplayY);
  lcd.print(" Data: ");
  lcd.print(sDisplayLines[iDisplayX - 1][iDisplayY - 1]);
  lcd.setCursor(0, 1);
  lcd.print("XY: ");
  lcd.print(iDisplayX + 1);
  lcd.print(iDisplayY);
  lcd.print(" Data: ");
  lcd.print(sDisplayLines[iDisplayX][iDisplayY - 1]);
  /*
  lcd.print("x ");//kursor porusza się z tekstem
  lcd.print(x_data);//kursor porusza się z tekstem
  lcd.print(" y ");//kursor porusza się z tekstem
  lcd.print(y_data);//kursor porusza się z tekstem
  lcd.setCursor(0, 1);
  lcd.print("sw ");//kursor porusza się z tekstem
  lcd.print(sw_data);//kursor porusza się z tekstem
  */

  lcd.setCursor(iCursor, 0);

  //Serial.print(digitalRead(sw_pin));
  }
void writeSerialData(){
  Serial.print("Switch:  ");
  Serial.print(digitalRead(sw_pin));
  Serial.print(" | ");
  Serial.print("X-axis: ");
  Serial.print(x_data);
  Serial.print(" | ");
  Serial.print("Y-axis: ");
  Serial.print(y_data);
  Serial.println(" | ");
  Serial.print("Servo pos:  ");
  Serial.print(iServoPos);
  }
void servoControl(){
  /*
    if (iServoPos >= 180){
      bServoDir  = false;
    } else if (iServoPos <= -180) {
      bServoDir  = true;
    }

    if (bServoDir){
      iServoPos += 10;
    } else if (!bServoDir) {
      iServoPos -= 10;
    }
  */

  iServoPos = map(x_data, -500, 500, 0, 180);
  miniServo9g.write(iServoPos);
  /*
  if (Serial.available() > 0) {
    // Jeśli dostępne są dane w Serial Monitor, odczytaj je
    iServoPos = Serial.parseInt();

    // Wyświetl odczytaną wartość w Serial Monitor
    Serial.print("Odczytano wartość: ");
    Serial.println(iServoPos);
    myServo.write(iServoPos);
  }
  */
  }

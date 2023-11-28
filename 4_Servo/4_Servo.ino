#include <LiquidCrystal.h>
#include <Arduino_JSON.h>
#include <Servo.h>

const int iCycleTime = 10;

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int iDisplayX = 1,  iDisplayY = 1, iCursor = 0;
bool bSwitchMemoX = false, bSwitchMemoY = false;
String sDisplayLines[10][10];
LiquidCrystal lcd(rs, en, d4, d5,d6, d7);

const int x_pin = A0, y_pin = A1, sw_pin = 8;
int x_data, y_data, sw_data, x_data_raw, y_data_raw;

Servo myServo;
const int servoPin = 9;
int iServoPos = 90;
bool bServoDir = true;


void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(sw_pin, INPUT);
  digitalWrite(sw_pin, HIGH);
  x_data_raw = analogRead(x_pin);
  y_data_raw = analogRead(y_pin);

  lcd.begin(16, 2); //inicjalizacja wyświetlacza o liczbie zanków 16 i 2 iniach
  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.cursor();

  myServo.attach(servoPin);
  myServo.write(iServoPos);

  initDisplayStrings();

  Serial.begin(9600);
}

void loop() {

  digitalWrite(13, HIGH);
  delay(iCycleTime);

  readAnalogData();
  writeSerialData();
  changeDisplayData();
  displayData();
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
  myServo.write(iServoPos);
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
  digitalWrite(13, LOW);
  delay(iCycleTime);

0

}

//Funkcje
void readAnalogData(){
  //read joystick data
  x_data = analogRead(x_pin) - x_data_raw;
  y_data = analogRead(y_pin) - y_data_raw;
  sw_data = digitalRead(sw_pin);
}

void initDisplayStrings() {
  for (int i = 0; i <= 9; ++i) {
    for (int j = 0; j <= 9; ++j) {
      sDisplayLines[i][j] = String(char('A' + i)) + " " + String(char('A' + j));
    }
  }
  /*String sDisplayLines[10][10] = {
  {"1 1", "1 2", "1 3", "1 4", "1 5", "1 6", "1 7", "1 8", "1 9", "1 10"},
  {"2 1", "2 2", "2 3", "2 4", "2 5", "2 6", "2 7", "2 8", "2 9", "2 10"},
  {"3 1", "3 2", "3 3", "3 4", "3 5", "3 6", "3 7", "3 8", "3 9", "3 10"},
  {"4 1", "4 2", "4 3", "4 4", "4 5", "4 6", "4 7", "4 8", "4 9", "4 10"},
  {"5 1", "5 2", "5 3", "5 4", "5 5", "5 6", "5 7", "5 8", "5 9", "5 10"},
  {"6 1", "6 2", "6 3", "6 4", "6 5", "6 6", "6 7", "6 8", "6 9", "6 10"},
  {"7 1", "7 2", "7 3", "7 4", "7 5", "7 6", "7 7", "7 8", "7 9", "7 10"},
  {"8 1", "8 2", "8 3", "8 4", "8 5", "8 6", "8 7", "8 8", "8 9", "8 10"},
  {"9 1", "9 2", "9 3", "9 4", "9 5", "9 6", "9 7", "9 8", "9 9", "9 10"},
  {"10 1", "10 2", "10 3", "10 4", "10 5", "10 6", "10 7", "10 8", "10 9", "10 10"}
};*/
}
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
  }
}
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
  Serial.println(" | ");
  Serial.print("Servo dir:  ");
  Serial.print(bServoDir);

}
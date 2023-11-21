#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int x_pin = A0, y_pin = A1, sw_pin = 8;
int x_data, y_data, sw_data, x_data_raw, y_data_raw;

LiquidCrystal lcd(rs, en, d4, d5,d6, d7);
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(sw_pin, INPUT);
  digitalWrite(sw_pin, HIGH);
  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
  //pinMode(2, INPUT);

  lcd.begin(16, 2); //inicjalizacja wyświetlacza o liczbie zanków 16 i 2 iniach
  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.print("xD");//kursor porusza się z tekstem

  x_data_raw = analogRead(x_pin);
  y_data_raw = analogRead(y_pin);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    
  x_data = analogRead(x_pin) - x_data_raw;
  y_data = analogRead(y_pin) - y_data_raw;
  sw_data = digitalRead(sw_pin);

  //String s_x_data = String(x_data);
 // String s_y_data = String(y_data);
  //String s_sw_data = String(sw_data);

  digitalWrite(13, HIGH);
  delay(100);
  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.clear();
  lcd.print("x ");//kursor porusza się z tekstem
  lcd.print(x_data);//kursor porusza się z tekstem
  lcd.print(" y ");//kursor porusza się z tekstem
  lcd.print(y_data);//kursor porusza się z tekstem
  lcd.setCursor(0, 1);
  lcd.print("sw ");//kursor porusza się z tekstem
  lcd.print(sw_data);//kursor porusza się z tekstem

  digitalWrite(13, LOW);
  delay(100);
  /*
  lcd.home();// to to samo co lcd.setCursor(0, 0)
  lcd.clear();
  lcd.print("KOCHAM");//kursor porusza się z tekstem
  lcd.setCursor(0, 1);
  lcd.print("BURSZTYNKA");//kursor porusza się z tekstem
  */
  Serial.print(digitalRead(sw_pin));
}

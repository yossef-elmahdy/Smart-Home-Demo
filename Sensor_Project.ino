#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <dht.h>
#include <SoftwareSerial.h>


//temperature sensor LM37DZ
#define temp_sensor A0

//LDR 1&2
#define ldr1 A1
#define ldr2 A2

//motor pins
#define motor_up 5
#define motor_down 6

//lightning system pin (transistor)
#define leds 3

//bluetooth module
SoftwareSerial mySerial(5, 4); // RX, TX

//fan (transistor)
#define fan 

//ultraSonic
#define ultra_trig 8
#define ultra_echo 9
float distance;
float t;

//servo motor
#define servo_pin //(Search if analog)
Servo myServo
//we can add position and delay to move slow 


//DHT11 (temperature - humidity)
#define DHT_bin dht 



 
//Password of our system 
char Password[4] = {'1', '2', '3', '4'};

//keypad configuration
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char Keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad left to right R1 R2 R3 R4 C1 C2 C3 C4
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad right to left
Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS );

//LCD configuration 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
bool start_system = false; //start the system 
int screen_number = 1; //switching between screens of LCD 



void setup() {
  pinMode(motor_up, OUTPUT);
  pinMode(motor_down, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(leds, OUTPUT);
  pinMode(ultra_trig, OUTPUT);
  pinMode(ultra_echo, INPUT);

  myServo.attach(servo_pin); //setup servo pin 
  myServo.write(90); //close state
  mySerial.begin(9600); //begin the bluetooth module 
  
  Serial.begin(9600); //to debug our code
  lcd.begin(16, 2); //setup the lcd
  lcd.print("Enter The password");
  welcome_screen();
  start_system = check_pass(); 
}

void loop() {

  if(start_system)  //open system 
  {
    //TO-Do 
  }
  else
  {
    start_system = check_pass();
  }
  

  Serial.print("Distance: ");
  Serial.println(ultra_reading());
  


}


//temperature reading (to be displayed in LCD & turn in the fan if currnet temp. > 25)
int temp_reading()
{
  int temp = analogRead(temp_sensor) * 0.48828;
  if (temp > 25) digitalWrite(fan, HIGH);
  else if (temp < 23) digitalWrite(fan, LOW); //avoid hystersis
  return temp;
}

void lighting_system()
{
  //more light, more voltage
  if (analogRead(ldr1) < /*value*/ && analogRead(ldr2) < /*some value*/) digitalWrite(leds, HIGH); // can be analogWrite() with levels debending on the outside 
}

bool keypad_security() 
{
  char pass[4];
  byte i = 0;

  //set the cursor to the second row 
  lcd.setCursor(0, 1);
  while(i < 4)
  {
    char c = keypad.getKey();
    if(c)
    {
      //print * in lcd
      lcd.print("*");
      pass[i] = c;
      ++i;
    }
  }

  /*
   * bool true_pass = true;
   * 
   * if(c)
   * {
   *    if(c != Password[i]) true_pass = false; 
   *    ++i;
   * }
   * 
   * return true_pass;
  */ 
  
  for(int j=0; j < 4; ++j)
  {
    if(pass[j] != Password[j])
    {
      return false;
    }
  }
  return true;
}

void welcome_screen()
{
  lcd.begin(16, 2); //setup the LCD
  lcd.print("WELCOME TO ");
  for(int i=0;i<3;++i) { lcd.print("."); delay(1000); }
  lcd.setCursor(0, 1);
  lcd.print("SMART MOSQUE :)");
  for(int i=0;i<3;++i) { lcd.print("."); delay(1000); }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password: ");
  lcd.setCursor(0, 1);
}

bool check_pass()
{
  while(true)
  {
    if(!keypad_security())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      lcd.print("WRONG :( ");
      for(int i=0;i<3;++i) { lcd.print("."); delay(1000); }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
    }
    else 
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CORRECT ^_^ ");
      for(int i=0;i<3;++i) { lcd.print("."); delay(1000); }
      lcd.clear();
      return true;
    }
  } 
}

int ultra_reading()
{
  digitalWrite(ultra_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultra_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultra_trig, LOW);
  t = pulseIn(ultra_echo, HIGH);
  return t * 0.034 / 2; 
}

void servo_reaction()
{
  int d = ultra_reading();
  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" CM");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
  if(d > 3 && d > 15)
  {
    myServo.write(0);  //open state
    delay(3000);
  }
  myServo.write(90); //close state 
  delay(10); //for ultrasonic reading
}

void lcd_functions()
{
  char c = keypad.getKey()
  if(c)
  {
    if(c == 'A') ++screen_number;
    else if(c == 'C') --screen_number;
    if(i > 4) screen_number = 1;
    if(i < 1) screen_number = 4;
  }

  switch(screen_number)
  {
    case 1: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("YA HALA ^_^");
      lcd.setCursor(0, 1);
      lcd.print(millis()/1000);
      lcd.setCursor(13, 1);
      lcd.print("1/4");
      break;
     case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TEMPERATUER: ");
      lcd.setCursor(0, 1);
      lcd.print(temp_reading());
      lcd.print(" C");
      lcd.setCursor(13, 1);
      lcd.print("2/4");
      break;
     case 3:
      lcd.clear();
      lock_system();
     case 4:
      //configurations     
  }
}

void lock_system()
{ 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOCK SYSTEM? ");
  char c;
  while(!c)
  {
    c = keypad.getKey();
    if(c)
    {
      if(c == 'B')
      {
        lcd.setCursor(0, 1);
        lcd.print("OK");
        delay(200);
        start_system = false;
      }

      else if(c == 'D')
      {
        lcd.setCursor(0, 1);
        lcd.print("CANCEL");
        delay(200);
        start_system = true;
      }
      
    }
  }
}

void bluetooth_control()
{
  char data;
  if(mySerial.available())
  {
    data = mySerial.read();
    Serial.print("Data: ");
    Serial.print(data);
  }
  if(data == '1')
  {
    analogWrite(motor_down, 200);
    analogWrite(motor_up, 0);
  }
  else
  {
    analogWrite(motor_down, 0);
    analogWrite(motor_up, 200);
  }
}

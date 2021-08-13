// makes my robot drive forwards, and uses the LCD screen.

// LCD settings
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//should be 26 characters total.
char message[] = "Visit beeclick.org!       ";
int pos = 0; //keep track of next char to print
int del = 250; //delay time
LiquidCrystal_I2C lcd(0x27,16,2);

// motor settings
// L=left motor, R=right motor
// enables
const int enR = 10;
const int enL = 9;
// inputs
const int inR1 = 5;
const int inR2 = 4;
const int inL1 = 2;
const int inL2 = 3;

// ultrasonic sensor settings
const int ping = 7;
const int echo = 6;

void setup()
{
  // motor pins
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(inR1, OUTPUT);
  pinMode(inR2, OUTPUT);
  pinMode(inL1, OUTPUT);
  pinMode(inL2, OUTPUT);
  // lcd screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(15,0);
  // ultrasonic sensor
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);
}
void loop()
{
  long clearance = ping_sonar();
  // if there's nothing in front of us, drive forward
  if (clearance > 8) {
    // both motors forwards
    digitalWrite(enR, HIGH);
    digitalWrite(enL, HIGH);
    digitalWrite(inL1, HIGH);
    digitalWrite(inL2, LOW);
    digitalWrite(inR1, HIGH);
    digitalWrite(inR2, LOW);
    // wait a lil bit
    delay(250);
  } else {
    // stop the motors
    digitalWrite(enR, LOW);
    digitalWrite(enL, LOW);
    // use the LCD screen
    scroll_LCD();
  }
}

// ping the ultrasonic sensor to get the distance in inches
long ping_sonar() {
   digitalWrite(ping, LOW);
   delayMicroseconds(2);
   digitalWrite(ping, HIGH);
   delayMicroseconds(10);
   digitalWrite(ping, LOW);
   long duration = pulseIn(echo, HIGH);
   return duration / 74 / 2;
}

// scroll the message across the LCD screen
void scroll_LCD() {
  lcd.setCursor(15,0);
  for (int pos=0; pos<26; pos++) {
    lcd.scrollDisplayLeft();
    lcd.print(message[pos]);
    delay(250);
  }
  lcd.clear();
}

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// imu object and parameters
Adafruit_BNO055 bno = Adafruit_BNO055(55);
float yaw = 0;
float roll = 0;
float pitch = 0;

// motor pins
const int enable_R = 9;
const int enable_L = 10;
const int in_R1 = 3;
const int in_R2 = 2;
const int in_L1 = 4;
const int in_L2 = 5;
// sonar sensor pins
const int ping = 7;
const int echo = 6;
// LED pins
const int led_red = 11;
const int led_green = 12;
const int led_blue = 13;

// serial msg from Pi will be L, R, F, B, or S
String msg;
// allow led color to change as visual indicator
int col = led_red;

void setup() {
  // setup all GPIO pins
  pinMode(enable_R, OUTPUT);
  pinMode(enable_L, OUTPUT);
  pinMode(in_R1, OUTPUT);
  pinMode(in_R2, OUTPUT);
  pinMode(in_L1, OUTPUT);
  pinMode(in_L2, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  // serial monitor
  Serial.begin(9600);
  // init the IMU
//  if (!bno.begin()) {
//    Serial.print("No BNO055 detected.");
//    while(1);
//  }
//  delay(1000);
//  bno.setExtCrystalUse(true);
}

void loop() {
  // get message from the Pi
  readSerialPort();
  // set motors based on msg received
  if (msg == "") { // no message received
  } else if (msg == "S") { // stop
    digitalWrite(enable_R, LOW);
    digitalWrite(enable_L, LOW);
    col = led_red;
  } else if (msg == "F") { // forward
    digitalWrite(enable_R, HIGH);
    digitalWrite(enable_L, HIGH);
    // left forwards
    digitalWrite(in_L1, HIGH);
    digitalWrite(in_L2, LOW);
    // right forwards
    digitalWrite(in_R1, HIGH);
    digitalWrite(in_R2, LOW);
    col = led_green;
  } else if (msg == "B") { // backward
    digitalWrite(enable_R, HIGH);
    digitalWrite(enable_L, HIGH);
    // left backwards
    digitalWrite(in_L1, LOW);
    digitalWrite(in_L2, HIGH);
    // right backwards
    digitalWrite(in_R1, LOW);
    digitalWrite(in_R2, HIGH);
    col = led_green;
  } else if (msg == "L") { // turn left
    digitalWrite(enable_R, HIGH);
    digitalWrite(enable_L, HIGH);
    // left backwards
    digitalWrite(in_L1, LOW);
    digitalWrite(in_L2, HIGH);
    // right forwards
    digitalWrite(in_R1, HIGH);
    digitalWrite(in_R2, LOW);
    col = led_blue;
  } else if (msg == "R") { // turn right
    digitalWrite(enable_R, HIGH);
    digitalWrite(enable_L, HIGH);
    // left forwards
    digitalWrite(in_L1, HIGH);
    digitalWrite(in_L2, LOW);
    // right backwards
    digitalWrite(in_R1, LOW);
    digitalWrite(in_R2, HIGH);
    col = led_blue;
  } else {
    Serial.print("Not a known message.");
  }
  // reply with IMU or sonar data only when we get a message
  if (msg != "") {
    //sendData();
    Serial.print("Received ");
    Serial.print(msg);
  }
  
  // update at 10Hz
  //delay(100);
  flash(col);
}

void readSerialPort() {
  msg = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      msg += (char)Serial.read();
    }
    Serial.flush();
  }
}

// print data to Pi over serial
void sendData() {
  // for now just get sonar data
  long clearance = ping_sonar();
  Serial.print("Clearance: ");
  Serial.print(clearance, 4);
}

// get a distance measurement from the sonar sensor
long ping_sonar() {
  digitalWrite(ping, LOW);
  delayMicroseconds(2);
  digitalWrite(ping, HIGH);
  delayMicroseconds(10);
  digitalWrite(ping, LOW);
  long duration = pulseIn(echo, HIGH);
  return duration / 74 / 2;
}

// flash an LED. Contains the delay for the clock cycle.
void flash(int pin_to_flash) {
  // ensure all are off
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_blue, LOW);
  // turn on desired
  digitalWrite(pin_to_flash, HIGH);
  // clock cycle delay
  delay(100);
  // turn it back off
  digitalWrite(pin_to_flash, LOW);
}

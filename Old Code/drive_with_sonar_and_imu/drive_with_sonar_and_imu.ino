// makes my robot drive forwards, printing IMU and sonar data to serial monitor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
float yaw = 0;
float roll = 0;
float pitch = 0;

// L=left motor, R=right motor
// enables
const int enR = 9;
const int enL = 10;
// inputs
const int inR1 = 3;
const int inR2 = 2;
const int inL1 = 4;
const int inL2 = 5;
// lights
const int ledR = 11;
const int ledG = 12;
const int ledB = 13;
// ultrasonic sensor
const int ping = 7;
const int echo = 6;

void setup()
{
  // motors
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(inR1, OUTPUT);
  pinMode(inR2, OUTPUT);
  pinMode(inL1, OUTPUT);
  pinMode(inL2, OUTPUT);
  // LEDs
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  // ultrasonic sensor
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);

  // IMU and serial monitor
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  // initialize the BNO
  if(!bno.begin())
  {
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}
void loop()
{
  // sonar check ------------------------------------
  long clearance = ping_sonar();
  Serial.print("clearance: ");
  Serial.print(clearance, 4);
  if (clearance > 15) {
    // turn on green LED
    digitalWrite(ledG, HIGH);
    // drive forwards then stop
    drive(1000);
  } else if (clearance > 5) {
    // turn on red LED
    digitalWrite(ledR, HIGH);
    // turn left
    turn_left(1000);
  } else {
    // just stop and turn on red LED
    digitalWrite(ledR, HIGH);
    // turn everything off
    digitalWrite(enR, LOW);
    digitalWrite(enL, LOW);
    delay(1000);
  }
  // flash blue
  flash(ledB);

  // imu check --------------------------------------
  // get a new sensor event
  sensors_event_t event; 
  bno.getEvent(&event);
  yaw = event.orientation.x;
  pitch = event.orientation.y;
  roll = event.orientation.z;
  // show IMU data on the serial monitor
  Serial.print("\tyaw: ");
  Serial.print(yaw, 4);
  Serial.print("\tpitch: ");
  Serial.print(pitch, 4);
  Serial.print("\troll: ");
  Serial.print(roll, 4);
  Serial.println("");
}

// drive forwards for 'wait_time' milliseconds
void drive(int wait_time) {
  // enable both motors
  digitalWrite(enR, HIGH);
  digitalWrite(enL, HIGH);
  // left forwards
  digitalWrite(inL1, HIGH);
  digitalWrite(inL2, LOW);
  // right forwards
  digitalWrite(inR1, HIGH);
  digitalWrite(inR2, LOW);
  // wait
  delay(wait_time);
  // turn everything off
  digitalWrite(enR, LOW);
  digitalWrite(enL, LOW);
}

// turn left for 'wait_time' ms
void turn_left(int wait_time) {
  // enable right motor
  digitalWrite(enR, HIGH);
  // left stationary
  // right forwards
  digitalWrite(inR1, HIGH);
  digitalWrite(inR2, LOW);
  // wait
  delay(wait_time);
  // turn everything off
  digitalWrite(enR, LOW);
}

// quickly flash the LED a certain color
void flash(int pin_to_flash) {
  // ensure all are off
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);
  // turn on the desired
  digitalWrite(pin_to_flash, HIGH);
  // only keep on for a short time
  delay(200);
  digitalWrite(pin_to_flash, LOW);
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

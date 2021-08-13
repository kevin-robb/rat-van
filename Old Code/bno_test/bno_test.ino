#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int led1 = 12;
int led2 = 10;
int led3 = 8;
float prev_yaw = 0;
float prev_roll = 0;
float prev_pitch = 0;
float yaw = 0;
float roll = 0;
float pitch = 0;

void setup(void) 
{
  // init LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
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

void loop(void) 
{
  // turn all LEDs off
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  delay(100);
  
  // get a new sensor event
  sensors_event_t event; 
  bno.getEvent(&event);

  yaw = event.orientation.x;
  pitch = event.orientation.y;
  roll = event.orientation.z;
  
  // show IMU data on the serial monitor
  Serial.print("yaw: ");
  Serial.print(yaw, 4);
  Serial.print("\tpitch: ");
  Serial.print(pitch, 4);
  Serial.print("\troll: ");
  Serial.print(roll, 4);
  Serial.println("");

  // blink LEDs corresponding to changing orientation
  if (abs(yaw - prev_yaw) > 5.0) {
    digitalWrite(led1, HIGH);
  }
  if (abs(pitch - prev_pitch) > 5.0) {
    digitalWrite(led2, HIGH);
  }
  if (abs(roll - prev_roll) > 5.0) {
    digitalWrite(led3, HIGH);
  }

  prev_yaw = yaw;
  prev_pitch = pitch;
  prev_roll = roll;
  
  delay(100);
}

// makes my robot drive forwards
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
}
void loop()
{
  long clearance = ping_sonar();
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

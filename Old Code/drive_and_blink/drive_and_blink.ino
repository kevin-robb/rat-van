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

void setup()
{
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(inR1, OUTPUT);
  pinMode(inR2, OUTPUT);
  pinMode(inL1, OUTPUT);
  pinMode(inL2, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}
void loop()
{
  // turn on green LED while driving
  digitalWrite(ledG, HIGH);
  // drive forwards then stop
  drive(2000);
  // flash blue
  flash(ledB);
  // switch to red LED
  digitalWrite(ledR, HIGH);
  // stay stopped for a time
  delay(2000);
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

// makes my robot drive forwards
// L=left motor, R=right motor
// enables
const int enR = 10;
const int enL = 9;
// inputs
const int inR1 = 5;
const int inR2 = 4;
const int inL1 = 2;
const int inL2 = 3;

void setup()
{
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(inR1, OUTPUT);
  pinMode(inR2, OUTPUT);
  pinMode(inL1, OUTPUT);
  pinMode(inL2, OUTPUT);
}
void loop()
{
  // both motors forwards
  digitalWrite(enR, HIGH);
  digitalWrite(enL, HIGH);
  // left forwards
  digitalWrite(inL1, HIGH);
  digitalWrite(inL2, LOW);
  // right forwards
  digitalWrite(inR1, HIGH);
  digitalWrite(inR2, LOW);
  // wait
  delay(2000);
  // turn everything off
  digitalWrite(enR, LOW);
  digitalWrite(enL, LOW);
  // wait
  delay(2000);
}

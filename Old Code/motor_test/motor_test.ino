// original program developed by R.GIRISH
const int Enable_A = 9;
const int Enable_B = 10;
const int inputA1 = 2;
const int inputA2 = 3;
const int inputB1 = 4;
const int inputB2 = 5;
void setup()
{
pinMode(Enable_A, OUTPUT);
pinMode(Enable_B, OUTPUT);
pinMode(inputA1, OUTPUT);
pinMode(inputA2, OUTPUT);
pinMode(inputB1, OUTPUT);
pinMode(inputB2, OUTPUT);
}
void loop()
{
// test motor A
digitalWrite(Enable_A, HIGH);
digitalWrite(inputA1, HIGH);
digitalWrite(inputA2, LOW);
delay(2000);
digitalWrite(Enable_A, LOW);
delay(2000);
digitalWrite(Enable_A, HIGH);
digitalWrite(inputA1, LOW);
digitalWrite(inputA2, HIGH);
delay(2000);
digitalWrite(Enable_A, LOW);

delay(2000);

// test motor B
digitalWrite(Enable_B, HIGH);
digitalWrite(inputB1, HIGH);
digitalWrite(inputB2, LOW);
delay(2000);
digitalWrite(Enable_B, LOW);
delay(2000);
digitalWrite(Enable_B, HIGH);
digitalWrite(inputB1, LOW);
digitalWrite(inputB2, HIGH);
delay(2000);
digitalWrite(Enable_B, LOW);

delay(2000);
}

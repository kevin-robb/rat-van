// motor pins
const int enable_R = 9;
const int enable_L = 10;
const int in_R1 = 3;
const int in_R2 = 2;
const int in_L1 = 4;
const int in_L2 = 5;
// LED pins
const int led_red = 11;
const int led_green = 12;
const int led_blue = 13;

// serial msg from Pi
String msg;
int comma_pos;
int forward_speed; // +255 = full forward, -255 = full backward
int side_speed; // +255 = full right, -255 = full left

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
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  // serial monitor
  Serial.begin(9600);
}

void loop() {
  // get message from the Pi
  readSerialPort();
  // set motors based on msg received. format = "int,int"
  comma_pos = msg.indexOf(',');
  forward_speed = msg.substring(0,comma_pos).toInt();
  side_speed = msg.substring(comma_pos+1).toInt();
  
  // flash led and delay
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

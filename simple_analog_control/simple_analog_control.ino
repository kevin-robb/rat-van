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
int y_speed; // +255 = full forward, -255 = full backward
int x_speed; // +255 = full right, -255 = full left
int motor_L_speed;
int motor_R_speed;

// allow led color to change as visual indicator
int col = led_blue;

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
  if (msg != "") {
    // set motors based on msg received. format = "int,int"
    comma_pos = msg.indexOf(',');
    y_speed = msg.substring(0,comma_pos).toInt();
    x_speed = msg.substring(comma_pos+1).toInt();
    motor_L_speed = abs(y_speed);
    motor_R_speed = abs(y_speed);
    // modify speeds based on turn intensity, and clamp to [0,255]
    if (x_speed < 0) {
      // left turn
      motor_L_speed += x_speed;
      motor_L_speed = max(motor_L_speed, 0);
    } else {
      // right turn
      motor_R_speed -= x_speed;
      motor_R_speed = max(motor_R_speed, 0);
    }
    // set motor speeds
    analogWrite(enable_R, motor_R_speed);
    analogWrite(enable_L, motor_L_speed);
    // set motor directions
    if (y_speed > 0) {
      // left forwards
      digitalWrite(in_L1, HIGH);
      digitalWrite(in_L2, LOW);
      // right forwards
      digitalWrite(in_R1, HIGH);
      digitalWrite(in_R2, LOW);
    } else {
      // left backwards
      digitalWrite(in_L1, LOW);
      digitalWrite(in_L2, HIGH);
      // right backwards
      digitalWrite(in_R1, LOW);
      digitalWrite(in_R2, HIGH);
    }
  }
  
  // flash led for delay
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

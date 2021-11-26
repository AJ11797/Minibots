#include <SparkFun_TB6612.h>
#include<Wire.h>
#include <math.h>
#include <time.h>
#include <Servo.h>

int motorPin = 2;
const int MPU=0x68;

const int offsetA = 1;
const int offsetB = 1;
const int MIN_DISTANCE = 10;

const double MOVE_VEL = 10; //cm per millisecond
const double TURN_VEL = 10; //degree per millisecond
double x_dist = 0;
double y_dist = 0;
double orientation = 0;
int grab_count = 0;

#define AIN1 7
#define BIN1 5
#define AIN2 8
#define BIN2 4
#define PWMA 9
#define PWMB 3
#define STBY 6
#define TRIG 11
#define ECHO 12
#define GRABL 10
#define SWITCH 13

double yaw = 0;
uint32_t timer;
uint32_t timer1;
long dist = 0;

Servo servo;

void ultrasonic_setup() {
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void wire_setup() {
  Wire.begin(); //initiate wire library and I2C
  Wire.beginTransmission(MPU); //begin transmission to I2C slave device
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)  
  Wire.endTransmission(true); //ends transmission to I2C slave device
}

double get_change() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);
  double accX, accY, accZ, tempRaw, gyroX, gyroY, gyroZ;
  accX=(Wire.read()<<8|Wire.read());
  accY=(Wire.read()<<8|Wire.read());
  accZ=(Wire.read()<<8|Wire.read());
  tempRaw=(Wire.read()<<8|Wire.read());
  gyroX=(Wire.read()<<8|Wire.read());
  gyroY=(Wire.read()<<8|Wire.read());
  gyroZ=(Wire.read()<<8|Wire.read());
  return gyroZ;
}

long get_ultrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  pinMode(ECHO, INPUT);
  long duration = pulseIn(ECHO, HIGH);
  long cm = (duration / 2) / 29.1;
  return cm;
}

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
  // put your setup code here, to run once:
  pinMode(SWITCH, INPUT);
  while (!digitalRead(SWITCH)) {
    continue;
  }
  Serial.begin(9600);
  ultrasonic_setup();
  wire_setup();
  servo.attach(GRABL);
  servo.write(150);
  //Drive towards centre
  while (dist > 10) { //get_ultrasonic() > 10 or get distance from picture
    dist = random(0, 100);
    forward(motor1, motor2, 255);
    delay(100);
    x_dist += 100 * MOVE_VEL * cos(orientation * M_PI / 180);
    y_dist += 100 * MOVE_VEL * sin(orientation * M_PI / 180);
  }
  //grab thing
  servo.write(175);
  ++grab_count;
  //reset position
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 20; ++i) {
    dist = random(0, 100);
    //rotate robot
    left(motor1, motor2, 100);
    timer1 = micros();
    for (int j = 0; j < 80; ++j) {
      double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
      timer = micros();
      delay(2);
      yaw += get_change() * dt;
    }
    double dt1 = (double)(micros() - timer1) / 1000000;
    orientation += dt1 * TURN_VEL;
    if (dist < 10) { //get picture
      break;
      //grab
      ++grab_count;
    }
  }
  brake(motor1, motor2);
  delay(50);
  Serial.println("Yaw, Orientation");
  Serial.println(yaw);
  Serial.println(orientation);

  if (grab_count < 2) {
    forward(motor1, motor2, 255);
    for (int i = 0; i < 10; ++i) {
      dist = random(0, 100); 
      //get_ultrasonic();
      //Serial.println(dist);
      if (dist < 10) {
        brake(motor1, motor2);
        break;
      }
      else {
        delay(100);
        x_dist += 100 * MOVE_VEL * cos(orientation * M_PI / 180);
        y_dist += 100 * MOVE_VEL * sin(orientation * M_PI / 180);
      }
    }
    brake(motor1, motor2);
    delay(50);
  }
  else {
    //Return to original position
    orientation -= int(orientation / 360) * 360;
    double required_orientation = atan2(y_dist, x_dist);
    if (required_orientation > orientation) {
      left(motor1, motor2, 100);
      delay((required_orientation - orientation) / TURN_VEL);
    }
    else {
      right(motor1, motor2, 100);
      delay((orientation - required_orientation) / TURN_VEL);
    }
    brake(motor1, motor2);
    orientation = required_orientation;
    forward(motor1, motor2, 255);
    delay(pow(x_dist*x_dist+y_dist*y_dist, 0.5) / MOVE_VEL);
    x_dist = 0;
    y_dist = 0;
    //Drop blocks
    servo.write(150);
  }
}

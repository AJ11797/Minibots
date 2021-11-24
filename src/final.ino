#include <SparkFun_TB6612.h>
#include<Wire.h>
#include <math.h>
#include <time.h>

int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
int motorPin = 2;
const int MPU=0x68;

const int offsetA = 1;
const int offsetB = 1;
const int MIN_DISTANCE = 10;

#define AIN1 7
#define BIN1 5
#define AIN2 8
#define BIN2 4
#define PWMA 9
#define PWMB 3
#define STBY 6
#define TRIG 11
#define ECHO 12

double yaw = 0;
uint32_t timer;
long dist = 0;

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
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);
  long cm = (duration / 2) / 29.1;
  return cm;
}

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
  // put your setup code here, to run once:
  delay(5000);
  //switch!
  Serial.begin(9600);
  ultrasonic_setup();
  wire_setup();
  //Drive towards centre
  while (dist > 10) { //get_ultrasonic() > 10 or get distance from picture
    dist = random(0, 100);
    forward(motor1, motor2, 255);
    delay(50);
  }
  //grab thing
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 20; ++i) {
    dist = random(0, 100);
    //rotate robot
    left(motor1, motor2, 100);
    for (int j = 0; j < 80; ++j) {
      double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
      timer = micros();
      delay(2);
      yaw += get_change() * dt;
    }
    if (dist < 10) { //get picture
      break;
    }
  }
  brake(motor1, motor2);
  delay(50);
  forward(motor1, motor2, 255);
  for (int i = 0; i < 10; ++i) {
    dist = random(0, 100); 
    //get_ultrasonic();
    //Serial.println(dist);
    if (dist < 10) {
      brake(motor1, motor2);
    }
    delay(100);
  }
  Serial.println(yaw);
}

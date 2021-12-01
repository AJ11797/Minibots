#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include <Wire.h>
#include <Servo.h>
#include <math.h>

const int addr = 0x8;
int x_dist, y_dist = 0;

const double MOVE_VEL = 0.21; //mm per millisecond
const double TURN_VEL = M_PI / 2300; //rad per millisecond
double pos[2] = {0.0, 0.0};
double angl = 0;
int grabL = 0;
int grabR = 0;
const int offsetA = 1;
const int offsetB = 1;

const int MIN_DISTANCE = 10;

#define AIN1 12
#define BIN1 10
#define AIN2 13
#define BIN2 9
#define PWMA 6
#define PWMB 5
#define STBY 11
#define GRABL 7
#define GRABR 2
int SWITCH = 13;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

Servo servoL;
Servo servoR;

char grabber = "L";

int recieve[2];
int count = 0;

void receiveEvent(){
  //while(Wire.available()){
    //int c = Wire.read();
    //Serial.print(c);
  //}
  //for (int i = 0; i<4; i++){
    recieve[count] = Wire.read();
    Serial.println(recieve[count]);
    //Serial.println(recieve[count]);
    count ++;

    if (count==2){
      count = 0;
      recieve[0] -= 128;
      recieve[1] *= 10;
      //recieve[1] *= 10;
      Serial.println("Next Ord");
      Serial.println((int)recieve[0]);
      Serial.println((int)recieve[1]);
  //}
}
}

void go_forward(Motor motor1, Motor motor2, int speed){
  motor1.drive(speed);
  motor2.drive(0.97*speed);

}

void turn_left(Motor motor1,Motor motor2 ,int speed, int duration){
  motor1.drive(-1*speed);
  motor2.drive(speed);
  delay(duration);
  brake(motor1, motor2);
}

void drive(int x, int y) { //x: horizontal, y: forward
  /*float theta = atan2(x,y);
  float turn = theta * 2 * 650/ M_PI ;
  float radius = pow((x*x + y*y),0.5);

  Serial.print("here it is");
  Serial.print(theta);
  Serial.print(turn);
  Serial.println(radius);
  if (turn <0) {
    turn = 2600 + turn;
  }*/
  //Rotate
  int  x_org = x;
  if (grabber == "L"){
    x -= 40;
  } else{
    x += 40;
  }
  if (x_org > 0) {
    turn_left(motor1, motor2, 100, 615);

  }
  else if (x_org < 0) {
    turn_left(motor1, motor2, -100, 570);
  }
  //side to side motion
  go_forward(motor1, motor2, -100);
  delay(abs(x)/MOVE_VEL);
  brake(motor1, motor2);

  //Rotate
  if (x_org < 0) {
    turn_left(motor1, motor2, 100, 615);
  }
  else if (x_org > 0) {
    turn_left(motor1, motor2, -100, 570);
  }


  //forward motion
  if (y > 0) {
    go_forward(motor1, motor2, -100);
  }
  else if (y < 0) {
    go_forward(motor1, motor2, 100);
  }
  delay(abs(y) / MOVE_VEL);
  brake(motor1, motor2);



  /*
  double ang = atan2(x, y) - angl;
  angl += ang;
  if (ang < 0) {
    left(motor1, motor2, 100);
  }
  else if (ang > 0) {
    right(motor1, motor2, 100);
  }
  delay(abs(ang)/TURN_VEL);
  brake(motor1, motor2);
  double dis = pow(x*x + y*y, 0.5);
  forward(motor1, motor2, -255);
  delay(dis/MOVE_VEL);
  brake(motor1, motor2);
  */
}


void setup() {
  /*
  switch_setup();
  ultrasonic_setup();
  forward(motor1, motor2, 255);
  delay(2000);
  right(motor1, motor2, 255);
  delay(2000);
  forward(motor1, motor2, 255);
  //Go forward to the middle block
  //Grab middle block
  //Turn
  //Go to the second block
  //Grab second block
  //Scan environment for blocks
  //Need to run this constantly, say every .5 second?
  if (get_ultrasonic() < MIN_DISTANCE) {
    //Stop robot

  }*/
  Serial.begin(9600);
  Wire.begin(addr);

  Wire.onReceive(receiveEvent);
  //Serial.println("Up and running");

  delay(3000);
  servoL.attach(GRABL);
  servoR.attach(GRABR);
  servoL.write(90);
  servoR.write(90);



  //servoL.write(120);
  int token[2] = {recieve[0], recieve[1]};
  //drive(-1*token[0], token[1]);
  //drive(token[0], -1* token[1]);

  //turn_left(motor1, motor2, 100, 9788);

  go_forward(motor1, motor2 , -100);
  delay(1210/ MOVE_VEL);
  brake(motor1, motor2);
  servoL.write(270);

  turn_left(motor1, motor2, 100, 1224);
  go_forward(motor1, motor2, -100);
  delay(1410/ MOVE_VEL);
  brake(motor1, motor2);

  servoL.write(90);
  go_forward(motor1, motor2, 100);
  delay(300/MOVE_VEL);
  brake(motor1, motor2);
}



void loop() {
  /* int token[2] = {recieve[0], recieve[1]};
  drive(-1*token[0], token[1]);
  servoL.write(175);
  drive(token[0], -1* token[1]);
  turn_left(motor1, motor2, 100, 1224);
  go_forward(motor1, motor2, 100);
  delay(300/MOVE_VEL);
  brake(motor1, motor2);
  servoL.write(90);

  go_forward(motor1, motor2, 100);
  delay(300/MOVE_VEL);
  brake(motor1, motor2);
  turn_left(motor1, motor2, 100, 1224);*/


  delay(100);
}

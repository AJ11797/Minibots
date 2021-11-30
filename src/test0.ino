#include <Servo.h>
#include <math.h>
#include <SparkFun_TB6612.h>

const double MOVE_VEL = 0.25; //mm per millisecond
const double TURN_VEL = M_PI / 2300; //rad per millisecond
double pos[2] = {0.0, 0.0};
double angl = 0;
int grabL = 0;
int grabR = 0;
const int offsetA = 1;
const int offsetB = 1;

int ord[2] = {0, 0};

#define AIN1 12
#define BIN1 10
#define AIN2 13
#define BIN2 9
#define PWMA 6
#define PWMB 5
#define STBY 11
#define GRABL 7
#define GRABR 2
#define SWITCH 13

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

Servo servoL;
Servo servoR;

void drive(double x, double y) { //x: horizontal, y: forward
  if (y > 0) {
    forward(motor1, motor2, -100);
  }
  else if (y < 0) {
    forward(motor1, motor2, 100);
  }
  delay(y / MOVE_VEL);
  brake(motor1, motor2);
  
  if (x < 0) {
    left(motor1, motor2, 100);
  }
  else if (x > 0) {
    right(motor1, motor2, 100);  
  }
  delay(M_PI/(2*TURN_VEL));
  brake(motor1, motor2);
  
  forward(motor1, motor2, -100);
  delay(abs(x)/MOVE_VEL);
  brake(motor1, motor2);

  servoL.write(175);
  
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
  // put your setup code here, to run once:
  //pinMode(SWITCH, INPUT);
  //while (!digitalRead(SWITCH)) {
  //  continue;
  //}
  delay(5000);
  Serial.begin(9600);
  servoL.attach(GRABL);
  servoR.attach(GRABR);
  servoR.write(90);

  drive(-200, 400);

  

  drive(200, -400);

  servoL.write(120);
  /*
  //Left grab
  //ask for distance
  ord[0] = 0;
  ord[1] = 500;
  drive(ord[0] - 50, ord[1]);
  pos[0] += ord[0] - 50;
  pos[1] += ord[1];
  //Grab
  //ask for distance again 
  servoL.write(175);
  grabL = 1;

  //Right grab
  ord[0] = -200;
  ord[1] = -300;
  drive(ord[0] + 50, ord[1]);
  pos[0] += ord[0] + 50;
  pos[1] += ord[1];
  //Grab
  servoR.write(150);
  grabR = 1;

  //Head back
  drive(-200, -200);
  //Release
  servoL.write(120);
  grabL = 0;
  servoR.write(30);
  delay(1000);
  servoR.write(90);
  grabR = 0;
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  

  
}

#include <SparkFun_TB6612.h>


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

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //serial communication at 9600 bauds
  delay(10000);
  forward(motor1, motor2, 100);
  delay(2000);
  left(motor1, motor2, 100);
  delay(2600);
  brake(motor1, motor2);
  delay(1000);
  forward(motor1, motor2, 100);
  delay(2000);
  brake(motor1, motor2);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:


}

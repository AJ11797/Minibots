#include <Servo.h>
#define GRABL 10

Servo servo;
int angle = 10;

void setup() {
  // put your setup code here, to run once:
  servo.attach(GRABL);
  servo.write(150); // normal angle
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo.write(175); // angle to grab the block
}

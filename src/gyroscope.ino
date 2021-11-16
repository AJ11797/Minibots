#include<Wire.h>
#include <math.h>
#include <time.h>
#include <Kalman.h>


const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int count = 0;
double pitch,roll, yaw = 0;
Kalman kalmanX;
Kalman kalmanY;
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;
double posX = 0, posY = 0;
double velX = 0, velY = 0;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter


uint32_t timer;
uint32_t timer2;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  accX=(Wire.read()<<8|Wire.read());
  accY=(Wire.read()<<8|Wire.read());
  accZ=(Wire.read()<<8|Wire.read());
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  Wire.endTransmission(true);
}

void loop(){
  ++count;
  //Read data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);
  accX=(Wire.read()<<8|Wire.read());
  accY=(Wire.read()<<8|Wire.read());
  accZ=(Wire.read()<<8|Wire.read());
  tempRaw=(Wire.read()<<8|Wire.read());
  gyroX=(Wire.read()<<8|Wire.read());
  gyroY=(Wire.read()<<8|Wire.read());
  gyroZ=(Wire.read()<<8|Wire.read());
  
  
  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  yaw += (gyroZ + 46) * dt;
  
  
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s
  
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    kalAngleY = pitch;
  } 
  else {
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
  }
  if (abs(kalAngleY) > 90) {
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  }
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  //Rotate accelerometer signal
  //Convert angles into radians
  kalAngleX *= (M_PI/180);
  kalAngleY *= (M_PI/180);
  //Roll
  double accX1 = accX;
  double accY1 = cos(kalAngleX)*accY + sin(kalAngleX)*accZ;
  double accZ1 = -sin(kalAngleX)*accY + cos(kalAngleX)*accZ;
  //Pitch
  double accX2 = cos(kalAngleY)*accX1 - sin(kalAngleY)*accZ1;
  double accY2 = accY1;
  double accZ2 = -sin(kalAngleY)*accX1 + cos(kalAngleX)*accZ1;
  //Yaw
  double accX3 = cos(yaw)*accX2 + sin(yaw)*accY2;
  double accY3 = -sin(yaw)*accX2 + cos(yaw)*accY2;
  double accZ3 = accZ2;
  velX += accX3 * dt;
  velY += accY3 * dt;
  posX += velX * dt;
  posY += velY * dt;
  
  //Print results
  if (count % 500 == 0) {
    posX += 50000;
    posY += 50000;
    Serial.print(yaw); Serial.println("\t");
    
    Serial.print("Position: ");
    Serial.print(posX); Serial.print("\t");
    Serial.print(posY); Serial.println("\t");
    /*
    Serial.print("Velocity: ");
    Serial.print(velX); Serial.print("\t");
    Serial.print(velY); Serial.println("\t");
    */
  }
  delay(2);
  
  /*
  int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
  int toff;
  double t,tx,tf;
  
  //Acceleration data correction
  AcXoff = 0;
  AcYoff = 0;
  AcZoff = 0;
  
  //Temperature correction
  toff = 0;
  
  //Gyro correction
  GyXoff = 0;
  GyYoff = 0;
  GyZoff = 0;
  int AcX[100], AcY[100], AcZ[100], GyX[100], GyY[100], GyZ[100], temp[100];
  
  for (int i = 0; i < 100; ++i) {
    //read accel data
    AcX[i]=(Wire.read()<<8|Wire.read()) + AcXoff;
    AcY[i]=(Wire.read()<<8|Wire.read()) + AcYoff;
    AcZ[i]=(Wire.read()<<8|Wire.read()) + AcYoff;
    
    //read temperature data
    int reading=(Wire.read()<<8|Wire.read()) + toff;
    t = reading/340 + 36.53;
    temp[i] = t;
    //tf = (t * 9/5) + 32;
    
    //read gyro data
    GyX[i]=(Wire.read()<<8|Wire.read()) + GyXoff;
    GyY[i]=(Wire.read()<<8|Wire.read()) + GyYoff;
    GyZ[i]=(Wire.read()<<8|Wire.read()) + GyZoff;
    delay(10);
  }
  //get pitch/roll
  int mean_AcX, mean_AcY, mean_AcZ, mean_GyX, mean_GyY, mean_GyZ, mean_temp;
  mean_AcX = average(AcX, 100);
  mean_AcY = average(AcY, 100);
  mean_AcZ = average(AcZ, 100);
  mean_GyX = average(GyX, 100);
  mean_GyY = average(GyY, 100);
  mean_GyZ = average(GyZ, 100);
  mean_temp = average(temp, 100);
  getAngle(mean_AcX,mean_AcY,mean_AcZ);
  
  //send the data out the serial port
  
  Serial.print("Angle: ");
  Serial.print("Pitch = "); Serial.print(pitch);
  Serial.print(" | Roll = "); Serial.println(roll);
  
  Serial.print("Temp: ");
  //Serial.print("Temp(F) = "); Serial.print(tf);
  Serial.print(" | Temp(C) = "); Serial.println(t);
  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(mean_AcX);
  Serial.print(" | Y = "); Serial.print(mean_AcY);
  Serial.print(" | Z = "); Serial.println(mean_AcZ);
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(mean_GyX);
  Serial.print(" | Y = "); Serial.print(mean_GyY);
  Serial.print(" | Z = "); Serial.println(mean_GyZ);
  Serial.println(" ");
  */
}

void getAngle(int Vx,int Vy,int Vz) {
  double x = Vx;
  double y = Vy;
  double z = Vz;
  pitch = atan(x/sqrt((y*y) + (z*z)));
  roll = atan(y/sqrt((x*x) + (z*z)));
  //convert radians into degrees
  pitch = pitch * (180.0/3.14);
  roll = roll * (180.0/3.14) ;
}

int average(int* vec, int len) {
  int total = 0;
  for (int i = 0; i < len; ++i) {
    total += vec[i];
  }
  return total;
}

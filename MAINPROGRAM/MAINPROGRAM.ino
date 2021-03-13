#include <Servo.h>
#include <Wire.h>

Servo motor1, motor2, motor3, motor4;

// RECEIVER global variables.
float throttle, yaw, pitch, roll;
bool isArmed;

// GY521 global variables.
float gyroValuesDgps[3];
float gyroDgpsError[3];

// PID global variables.
float kp, ki, kd;
float yawPid, pitchPid, rollPid;

// Loop time variables.
float elapsedTime, time, timePrev;

void setup(){
  Serial.begin(9600);
  
  // Set gyro sensor.
  setGyro();
  // Calibrate gyro error at 0 deg/s.
  calibrateGyroError();
  
  motor1.attach(8, 1000, 2000);
  motor2.attach(9, 1000, 2000);
  motor3.attach(10, 1000, 2000);
  motor4.attach(11, 1000, 2000);
  
  isArmed = false;
}

void loop(){
  // Calculate loop time.
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000;
  
  // Read receiver input.
  receiver();
  // Read gyro values.
  readGyroDgps(gyroValuesDgps, gyroDgpsError);
  // Calculate PIDs.
  calculatePid(gyroValuesDgps);

  
  if (isArmed){
    Serial.print("Throttle: ");
    Serial.print(throttle);
    Serial.print("   Yaw: ");
    Serial.print(yaw);
    Serial.print("   Picth: ");
    Serial.print(pitch);
    Serial.print("   Roll: ");
    Serial.print(roll);
    Serial.print("");
    Serial.print("   Armed: ");
    Serial.println(isArmed);
//    motor1.write(throttleMap);
//    motor2.write(throttleMap);
//    motor3.write(throttleMap);
//    motor4.write(throttleMap);
  }
  else {
    Serial.println("NOT ARMED!!!");
  }  
}

#include <Servo.h>
#include <Wire.h>

Servo motor1, motor2, motor3, motor4;

// RECEIVER global variables.
float Throttle, Yaw, Pitch, Roll;
bool IsArmed;

// GY521 global variables.
float GyroValuesDgps[3];
float GyroDgpsError[3];

// PID global variables.
float YawPid, PitchPid, RollPid;

// Loop time variables.
float elapsedTime, time, timePrev;

void setup(){
  Serial.begin(9600);
  
  // Set gyro sensor.
  SetGyro();
  // Calibrate gyro error at 0 deg/s.
  CalibrateGyroError();
  
  motor1.attach(8, 1000, 2000);
  motor2.attach(9, 1000, 2000);
  motor3.attach(10, 1000, 2000);
  motor4.attach(11, 1000, 2000);
  
  IsArmed = false;
}

void loop(){
  // Calculate loop time.
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000;
  
  // Read receiver input.
  Receiver();
  // Read gyro values.
  ReadGyroDgps();
  // Calculate PIDs.
  CalculatePid();

  
  if (IsArmed){
    Serial.print("Throttle: ");
    Serial.print(Throttle);
    Serial.print("   Yaw: ");
    Serial.print(Yaw);
    Serial.print("   Picth: ");
    Serial.print(Pitch);
    Serial.print("   Roll: ");
    Serial.print(Roll);
    Serial.print("");
    Serial.print("   Armed: ");
    Serial.println(IsArmed);
//    motor1.write(throttleMap);
//    motor2.write(throttleMap);
//    motor3.write(throttleMap);
//    motor4.write(throttleMap);
  }
  else {
    Serial.println("NOT ARMED!!!");
  }  
}

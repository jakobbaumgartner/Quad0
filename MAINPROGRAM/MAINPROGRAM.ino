#include <Servo.h>
#include <Wire.h>

Servo Motor1, Motor2, Motor3, Motor4;

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
  // Set motors.
  //MotorSetup();
  
  
  IsArmed = false;
}

void loop(){
  // Calculate loop time.
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000;
  
  // Read receiver input.
  //Receiver();
  // Read gyro values.
  ReadGyroDgps();
  // Calculate PIDs.
  //CalculatePid();
  // Write to motors.

  
  Serial.println(GyroValuesDgps[1]);
}

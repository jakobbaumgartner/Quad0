#include <Servo.h>
#include <Wire.h>

Servo Motor1, Motor2, Motor3, Motor4;

// RECEIVER global variables.
float Throttle, Yaw, Pitch, Roll;
float ReceiverInputs[5];
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

  InterruptSetup();
  // Set gyro sensor.
  SetGyro();
  // Calibrate gyro error at 0 deg/s.
  CalibrateGyroError();
  // Set motors.
  MotorSetup();
  
  IsArmed = false;
  Serial.println("SETUP COMPLETED");
}

void loop(){
  // Calculate loop time.
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000;
  
  // Read gyro values.
  ReadGyroDgps();
  // Calculate PIDs.
  CalculatePid();
  // Write to motors.

  
  Serial.print(GyroValuesDgps[0]);
  Serial.print("    ");
  Serial.print(GyroValuesDgps[1]);  
  Serial.print("    ");
  Serial.print(GyroValuesDgps[2]);
  Serial.print("    ");
//  Serial.print(ReceiverInputs[3]);
//  Serial.print("    ");
//  Serial.print(ReceiverInputs[4]);
//  Serial.print("    ");
  Serial.print(elapsedTime);
  Serial.println("");

}

#include <Servo.h>
#include <Wire.h>

Servo Motor1, Motor2, Motor3, Motor4;

// RECEIVER global variables.
float Throttle, Yaw, Pitch, Roll;
// Index: 0 - Throttle, 1 - Yaw, 2 - Pitch, 3 - Roll, 4 - Arm.
float ReceiverInputs[5];
bool IsArmed;
float linearCoefYaw[2];
float linearCoefPitch[2];
float linearCoefRoll[2];
int nL[3] = {0, 0, 80};
int nH[3] = {-4, -12, 0};

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
  // Receiver calibration.
  LinearScaling(nL, nH);
  
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

  
  Serial.print(ReceiverInputs[0]);
  Serial.print("    ");
  Serial.print(ReceiverInputs[1]);  
  Serial.print("    ");
  Serial.print(ReceiverInputs[2]);
  Serial.print("    ");
  Serial.print(ReceiverInputs[3]);
  Serial.print("    ");
  Serial.print(elapsedTime);
  Serial.println("");

}

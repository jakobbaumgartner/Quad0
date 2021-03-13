// PID calculation.
// GLOBAL VARIABLES: float kp, float ki, float kd, float yawPid, float pitchPid, float rollPid.

float yawPrevError;
float pitchPrevError;
float rollPrevError;

void calculatePid(float gyroValues[]){
  // YAW PID
  float yawError = yaw - gyroValues[3];
  float yawP = kp * yawError;
  float yawI = yawI + ki * yawError;
  float yawD = kd * ((yawError - yawPrevError) / elapsedTime);
  float yawPrevError = yawError;
  yawPid = yawP + yawI + yawD;

  // PITCH PID
  float pitchError = pitch - gyroValues[0];
  float pitchP = kp * pitchError;
  float pitchI = pitchI + ki * pitchError;
  float pitchD = kd * ((pitchError - pitchPrevError) / elapsedTime);
  float pitchPrevError = pitchError;
  pitchPid = pitchP + pitchI + pitchD;

  // ROLL PID
  float rollError = roll - gyroValues[2];
  float rollP = kp * rollError;
  float rollI = rollI + ki * rollError;
  float rollD = kd * ((rollError - rollPrevError) / elapsedTime);
  float rollPrevError = rollError;
  rollPid = rollP + rollI + rollD; 
}

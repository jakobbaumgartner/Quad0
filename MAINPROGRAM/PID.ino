// PID calculation.
// GLOBAL FUNCTIONS: void calculatePid().
// GLOBAL VARIABLES: float YawPid, float PitchPid, float RollPid.

float kpYaw, kiYaw, kdYaw, kpPitch, kiPitch, kdPitch, kpRoll, kiRoll, kdRoll;
float yawPrevError, pitchPrevError, rollPrevError;

// Calculate PIDs for each axis.
void CalculatePid(){
  // YAW PID
  float yawError = Yaw - GyroValuesDgps[3];
  float yawP = kpYaw * yawError;
  float yawI = yawI + kiYaw * yawError;
  float yawD = kdYaw * ((yawError - yawPrevError) / elapsedTime);
  yawPrevError = yawError;
  YawPid = yawP + yawI + yawD;

  // PITCH PID
  float pitchError = Pitch - GyroValuesDgps[0];
  float pitchP = kpPitch * pitchError;
  float pitchI = pitchI + kiPitch * pitchError;
  float pitchD = kdPitch * ((pitchError - pitchPrevError) / elapsedTime);
  pitchPrevError = pitchError;
  PitchPid = pitchP + pitchI + pitchD;

  // ROLL PID
  float rollError = Roll - GyroValuesDgps[2];
  float rollP = kpRoll * rollError;
  float rollI = rollI + kiRoll * rollError;
  float rollD = kdRoll * ((rollError - rollPrevError) / elapsedTime);
  rollPrevError = rollError;
  RollPid = rollP + rollI + rollD; 
}

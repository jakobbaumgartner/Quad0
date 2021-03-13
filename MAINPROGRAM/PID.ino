// PID calculation.
// GLOBAL VARIABLES: float yawPid, float pitchPid, float rollPid.

float kpYaw, kiYaw, kdYaw, kpPitch, kiPitch, kdPitch, kpRoll, kiRoll, kdRoll;
float yawPrevError, pitchPrevError, rollPrevError;

void calculatePid(float gyroValues[]){
  // YAW PID
  float yawError = yaw - gyroValues[3];
  float yawP = kpYaw * yawError;
  float yawI = yawI + kiYaw * yawError;
  float yawD = kdYaw * ((yawError - yawPrevError) / elapsedTime);
  yawPrevError = yawError;
  yawPid = yawP + yawI + yawD;

  // PITCH PID
  float pitchError = pitch - gyroValues[0];
  float pitchP = kpPitch * pitchError;
  float pitchI = pitchI + kiPitch * pitchError;
  float pitchD = kdPitch * ((pitchError - pitchPrevError) / elapsedTime);
  pitchPrevError = pitchError;
  pitchPid = pitchP + pitchI + pitchD;

  // ROLL PID
  float rollError = roll - gyroValues[2];
  float rollP = kpRoll * rollError;
  float rollI = rollI + kiRoll * rollError;
  float rollD = kdRoll * ((rollError - rollPrevError) / elapsedTime);
  srollPrevError = rollError;
  rollPid = rollP + rollI + rollD; 
}

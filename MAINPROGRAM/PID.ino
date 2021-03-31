// PID calculation.
// GLOBAL FUNCTIONS: void CalculatePid().
// GLOBAL VARIABLES: float YawPid, float PitchPid, float RollPid.

float kpYaw = 1;
float kiYaw = 0; 
float kdYaw = 0;
float kpPitch = 1;
float kiPitch = 0; 
float kdPitch = 0;
float kpRoll = 1;
float kiRoll = 0;
float kdRoll = 0;
float yawPrevError, pitchPrevError, rollPrevError;

float limit = 400;

// Calculate PIDs for each axis.
void CalculatePid(){
  // YAW PID
  float yawError = ReceiverInputs[1]- GyroValuesDgps[2];
  float yawP = kpYaw * yawError;
  float yawI = yawI + kiYaw * yawError;
  float yawD = kdYaw * ((yawError - yawPrevError) / elapsedTime);
  yawPrevError = yawError;
  YawPid = yawP + yawI + yawD;
  if (YawPid > limit){
    YawPid = limit;
  }
  else if (YawPid < -limit){
    YawPid = -limit;
  }
  Serial.println(YawPid);

  // PITCH PID
  float pitchError = ReceiverInputs[2] - GyroValuesDgps[0];
  float pitchP = kpPitch * pitchError;
  float pitchI = pitchI + kiPitch * pitchError;
  float pitchD = kdPitch * ((pitchError - pitchPrevError) / elapsedTime);
  pitchPrevError = pitchError;
  PitchPid = pitchP + pitchI + pitchD;
    if (PitchPid > limit){
    PitchPid = limit;
  }
  else if (PitchPid < -limit){
    PitchPid = -limit;
  }

  // ROLL PID
  float rollError = ReceiverInputs[3] - GyroValuesDgps[1];
  float rollP = kpRoll * rollError;
  float rollI = rollI + kiRoll * rollError;
  float rollD = kdRoll * ((rollError - rollPrevError) / elapsedTime);
  rollPrevError = rollError;
  RollPid = rollP + rollI + rollD; 
  if (RollPid > limit){
    RollPid = limit;
  }
  else if (RollPid < -limit){
    RollPid = -limit;
  }
}

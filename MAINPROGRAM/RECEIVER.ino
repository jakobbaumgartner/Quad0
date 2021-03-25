// RECEIVER functionality: read raw values, convert them into rate curves (for yaw, pitch and roll) and map throttle.
// GLOBAL FUNCTIONS: void Receiver().
// GLOBAL VARIABLES: float Throttle, float Yaw, float Pitch, float Roll, bool IsArmed.
// Throttle -> ch 2, Yaw -> ch3, Pitch -> ch 1, Roll -> ch 0, Arm -> ch 4.

// Upper mapped throttle boundary.
int maxThrottle = 1800;
// Min and max boundaries of yaw, pitch and roll raw inputs.
int minRadioInput = -450;
int maxRadioInput = 450;
// Middle value offset.
int midValueOffset = 20;

int currentTime;
int timerThrottle, timerYaw, timerPitch, timerRoll, timerArm;
int lastValueThrottle, lastValueYaw, lastValuePitch, lastValueRoll, lastValueArm;


void InterruptSetup(){
  // Enable interrupts for pins 23:16.
  PCICR = 0b00000100;
  // Enable interrupts for pins D7:D3.
  PCMSK2 = 0b11111000;
  Serial.println("Interrupt enabled");
}

ISR(PCINT2_vect){
    currentTime = micros();

  // Throttle (pin D5)
  if (PIND & 0b00100000){
    if(lastValueThrottle == 0){
      lastValueThrottle = 1;
      timerThrottle = currentTime;
    }
  }
  else if(lastValueThrottle == 1){
    lastValueThrottle = 0;
    ReceiverInputs[0] = currentTime - timerThrottle;
  }

  // Yaw (pin D4)
  if (PIND & 0b00010000){
    if(lastValueYaw == 0){
      lastValueYaw = 1;
      timerYaw = currentTime;
    }
  }
  else if(lastValueYaw == 1){
    lastValueYaw = 0;
    ReceiverInputs[1] = currentTime - timerYaw;
  }

  // Pitch (pin D6)
  if (PIND & 0b01000000){
    if(lastValuePitch == 0){
      lastValuePitch = 1;
      timerPitch = currentTime;
    }
  }
  else if(lastValuePitch == 1){
    lastValuePitch = 0;
    ReceiverInputs[2] = currentTime - timerPitch;
  }

  // Roll (pin D7)
  if (PIND & 0b10000000){
    if(lastValueRoll == 0){
      lastValueRoll = 1;
      timerRoll = currentTime;
    }
  }
  else if(lastValueRoll == 1){
    lastValueRoll = 0;
    ReceiverInputs[3] = currentTime - timerRoll;
  }

  // Arm (pin D3)
  if (PIND & 0b00001000){
    if(lastValueArm == 0){
      lastValueArm = 1;
      timerArm = currentTime;
    }
  }
  else if(lastValueArm == 1){
    lastValueArm = 0;
    ReceiverInputs[4] = currentTime - timerArm;
  }
}

//void Receiver(){
//  currentTime = micros();
//
//  // Throttle
//  if (digitalRead(5)){
//    if(lastValueThrottle == 0){
//      lastValueThrottle = 1;
//      timerThrottle = currentTime;
//    }
//  }
//  else if(lastValueThrottle == 1){
//    lastValueThrottle = 0;
//    ReceiverInputs[0] = currentTime - timerThrottle;
//  }
//
//  // Yaw
//  if (digitalRead(4)){
//    if(lastValueYaw == 0){
//      lastValueYaw = 1;
//      timerYaw = currentTime;
//    }
//  }
//  else if(lastValueYaw == 1){
//    lastValueYaw = 0;
//    ReceiverInputs[1] = currentTime - timerYaw;
//  }
//
//  // Pitch
//  if (digitalRead(6)){
//    if(lastValuePitch == 0){
//      lastValuePitch = 1;
//      timerPitch = currentTime;
//    }
//  }
//  else if(lastValuePitch == 1){
//    lastValuePitch = 0;
//    ReceiverInputs[2] = currentTime - timerPitch;
//  }
//
//  // Roll
//  if (digitalRead(7)){
//    if(lastValueRoll == 0){
//      lastValueRoll = 1;
//      timerRoll = currentTime;
//    }
//  }
//  else if(lastValueRoll == 1){
//    lastValueRoll = 0;
//    ReceiverInputs[3] = currentTime - timerRoll;
//  }
//
//  // Arm
//  if (digitalRead(3)){
//    if(lastValueArm == 0){
//      lastValueArm = 1;
//      timerArm = currentTime;
//    }
//  }
//  else if(lastValueArm == 1){
//    lastValueArm = 0;
//    ReceiverInputs[4] = currentTime - timerArm;
//  }
//}
// Read raw values from receiver and convert them in [us] pulses (throttle) and reference [deg/s] pids inputs (yaw, pitch, roll).
//void Receiver(){
//  // Read raw receiver input.
//  int throttleRaw = pulseIn(5, HIGH);
//  int yawRaw = pulseIn(4, HIGH, 2000);
//  int pitchRaw = pulseIn(6, HIGH, 2000);
//  int rollRaw = pulseIn(7, HIGH, 2000);
//  int armRaw = pulseIn(3, HIGH, 2000);
//
//  // Map raw values.
//  float throttleMap = map(throttleRaw, 986, 1945, 1000, 2000);
//  float yawMap = map(yawRaw, 986, 1962, minRadioInput, maxRadioInput); 
//  float pitchMap = map(pitchRaw, 995, 1972, minRadioInput, maxRadioInput);
//  float rollMap = map(rollRaw, 1018, 1989, minRadioInput, maxRadioInput);
//
//  // Cut lower or higher values than maximum or minimum values.
//  throttleMap = lowHighCut(throttleMap, 1000, maxThrottle);
//  yawMap = lowHighCut(yawMap, minRadioInput, maxRadioInput);
//  pitchMap = lowHighCut(pitchMap, minRadioInput, maxRadioInput);
//  rollMap = lowHighCut(rollMap, minRadioInput, maxRadioInput);
//  
//  // Set mid point values of inputs (0 and 1500).
//  Throttle = calibrateMidPoint(throttleMap, 1500);
//  yawMap = calibrateMidPoint(yawMap, 0);
//  pitchMap = calibrateMidPoint(pitchMap, 0);
//  rollMap = calibrateMidPoint(rollMap, 0);
//
//  // Convert to third order polynomial curve (rate curves).
//  // Min/max value cca +/- 1080.
//  Yaw = 0.00000664 * pow(yawMap, 3) + 0.000002 * pow(yawMap, 2) + yawMap;
//  Pitch = 0.00000664 * pow(pitchMap, 3) + 0.000002 * pow(pitchMap, 2) + pitchMap;
//  Roll = 0.00000664 * pow(rollMap, 3) + 0.000002 * pow(rollMap, 2) + rollMap;
//
//  IsArmed = armRaw > 1500 ? true : false;  
//}
//
//// Cut values lower than minValue and higher than maxValue.
//float lowHighCut(float input, int minValue, int maxValue){
//  if (input > maxValue){
//    input = maxValue;
//  }
//  if (input < minRadioInput){
//    input = minValue;
//  }
//
//  return input;
//}

// Set midpoint to 1500 (throttle) or to 0 (yaw, pitch, roll) when joysticks are on middle.
float calibrateMidPoint(float input, int midValue){
  if (input < midValue + midValueOffset && input > midValue - midValueOffset){
    input = midValue;
  }
   return input;
}

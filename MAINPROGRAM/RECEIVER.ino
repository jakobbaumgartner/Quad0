// RECEIVER functionality: read raw values, convert them into rate curves (for yaw, pitch and roll) and map throttle.
// GLOBAL VARIABLES: float throttle, float yaw, float pitch, float roll, bool isArmed.
// Throttle -> ch 2, Yaw -> ch3, Pitch -> ch 1, Roll -> ch 0, Arm -> ch 4.

// Upper mapped throttle boundary.
int maxThrottle = 1800;
// Min and max boundaries of yaw, pitch and roll raw inputs.
int minRadioInput = -450;
int maxRadioInput = 450;
// Middle value offset.
int midValueOffset = 20;

void receiver(){
  // Read raw receiver input.
  int throttleRaw = pulseIn(5, HIGH);
  int yawRaw = pulseIn(4, HIGH);
  int pitchRaw = pulseIn(6, HIGH);
  int rollRaw = pulseIn(7, HIGH);
  int armRaw = pulseIn(3, HIGH);

  // Map raw values.
  float throttleMap = map(throttleRaw, 986, 1945, 1000, 2000);
  float yawMap = map(yawRaw, 986, 1962, minRadioInput, maxRadioInput); 
  float pitchMap = map(pitchRaw, 995, 1972, minRadioInput, maxRadioInput);
  float rollMap = map(rollRaw, 1018, 1989, minRadioInput, maxRadioInput);

  // Cut lower or higher values than maximum or minimum values.
  throttleMap = lowHighCut(throttleMap, 1000, maxThrottle);
  yawMap = lowHighCut(yawMap, minRadioInput, maxRadioInput);
  pitchMap = lowHighCut(pitchMap, minRadioInput, maxRadioInput);
  rollMap = lowHighCut(rollMap, minRadioInput, maxRadioInput);
  
  // Set mid point values of inputs (0 and 1500).
  throttle = calibrateMidPoint(throttleMap, 1500);
  yawMap = calibrateMidPoint(yawMap, 0);
  pitchMap = calibrateMidPoint(pitchMap, 0);
  rollMap = calibrateMidPoint(rollMap, 0);

  // Convert to third order polynomial curve (rate curves).
  // Min/max value cca +/- 1080.
  yaw = 0.00000664 * pow(yawMap, 3) + 0.000002 * pow(yawMap, 2) + yawMap;
  pitch = 0.00000664 * pow(pitchMap, 3) + 0.000002 * pow(pitchMap, 2) + pitchMap;
  roll = 0.00000664 * pow(rollMap, 3) + 0.000002 * pow(rollMap, 2) + rollMap;

  isArmed = armRaw > 1500 ? true : false;  
}

// Cut values lower than minValue and higher than maxValue.
float lowHighCut(float input, int minValue, int maxValue){
  if (input > maxValue){
    input = maxValue;
  }
  if (input < minRadioInput){
    input = minValue;
  }

  return input;
}

// Set midpoint to 1500 when joysticks are on middle.
float calibrateMidPoint(float input, int midValue){
  if (input < midValue + midValueOffset && input > midValue - midValueOffset){
    input = midValue;
  }
   return input;
}

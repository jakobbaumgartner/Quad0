#include <Servo.h>

void MotorSetup(){
  Motor1.attach(8, 1000, 2000);
  Motor2.attach(9, 1000, 2000);
  Motor3.attach(10, 1000, 2000);
  Motor4.attach(11, 1000, 2000);
}

void MotorMixingAlgorithm(){
  Motor1.write(ReceiverInputs[0] - abs(YawPid) + abs(PitchPid) - abs(RollPid));
  Motor2.write(ReceiverInputs[0] + abs(YawPid) + abs(PitchPid) + abs(RollPid));
  Motor3.write(ReceiverInputs[0] + abs(YawPid) - abs(PitchPid) + abs(RollPid));
  Motor4.write(ReceiverInputs[0] - abs(YawPid) - abs(PitchPid) + abs(RollPid));
}

#include <Servo.h>

void MotorSetup(){
  Motor1.attach(8, 1000, 2000);
  Motor2.attach(9, 1000, 2000);
  Motor3.attach(10, 1000, 2000);
  Motor4.attach(11, 1000, 2000);
}

void MotorMixingAlgorithm(){
  Motor1.write(Throttle);
  Motor2.write(Throttle);
  Motor3.write(Throttle);
  Motor4.write(Throttle);
}

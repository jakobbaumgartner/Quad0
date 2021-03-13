// GYRO functionality: read raw gyro values and convert them into deg/s. 
// setGyro() - gyro setup,
// calibrateGyroError() - calculate gyro error at 0 deg/s.
// readGyroDgps() - read deg/s from gyro.
// GLOBAL VARIABLES: float gyroValuesDgps [], float gyroDgpsError [].

// I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
const int MPU_ADDR = 0x68; 
// Variables for gyro raw data.
int16_t gyroX, gyroY, gyroZ; 
int16_t gyroXDgps, gyroYDgps, gyroZDgps;

void calibrateGyroError () {
// calibrateGyroError() - calculate gyro error at 0 deg/s.

  float gyroErrorX = 0;
  float gyroErrorY = 0;
  float gyroErrorZ = 0;

  for (int i = 0; i <1000; i++) {
      Wire.beginTransmission(MPU_ADDR);
      // Starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40].
      Wire.write(0x3B); 
      // The parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
      Wire.endTransmission(false); 
      // Request a total of 7*2=14 registers.
      Wire.requestFrom(MPU_ADDR, 7*2, true); 
      
      // Reading registers: 0x43 (gyroXOUT_H) and 0x44 (gyroXOUT_L).
      gyroX = Wire.read()<<8 | Wire.read(); 
      // Reading registers: 0x45 (gyroYOUT_H) and 0x46 (gyroYOUT_L).
      gyroY = Wire.read()<<8 | Wire.read(); 
      // Reading registers: 0x47 (gyroZOUT_H) and 0x48 (gyroZOUT_L).
      gyroZ = Wire.read()<<8 | Wire.read(); 

      // Convert into deg/s with range of 1000deg/s.
      gyroXDgps = gyroX/32.8;
      gyroYDgps = gyroY/32.8;
      gyroZDgps = gyroZ/32.8;
      
      gyroErrorX += gyroXDgps;
      gyroErrorY += gyroYDgps;
      gyroErrorZ += gyroZDgps;
  }

  // Calculate average error, after 1000 measurments.
  gyroDgpsError[0] = gyroErrorX / 1000;
  gyroDgpsError[1] = gyroErrorY / 1000;
  gyroDgpsError[2] = gyroErrorZ / 1000;
}

void setGyro () {
  // setGyro() - gyro setup routine, sets sensitivity and begins transmission

  Wire.begin();
  // Begins a transmission to the I2C slave (GY-521 board).
	Wire.beginTransmission(MPU_ADDR); 
  // PWR_MGMT_1 register.
	Wire.write(0x6B); 
  // set to zero (wakes up the MPU-6050).
	Wire.write(0); 
	Wire.endTransmission(true);

  // Begins a transmission to the I2C slave (GY-521 board).
	Wire.beginTransmission(MPU_ADDR); 
  // GYRO_CONFIG register - set sensitivity.
	Wire.write(0x1B); 
  // Send new values.
  Wire.write(0x10); 
  // End transmission.
	Wire.endTransmission(true); 
}

void readGyroDgps (float gyroValuesDgps [], float gyroDgpsError []) {
// readGyroDgps() - read deg/s from gyro and write them into global variables

  Wire.beginTransmission(MPU_ADDR);
  // Starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.write(0x3B); 
  // The parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.endTransmission(false); 
  // Request a total of 7*2=14 registers
	Wire.requestFrom(MPU_ADDR, 7*2, true); 
 
	// Reading registers: 0x43 (gyroXOUT_H) and 0x44 (gyroXOUT_L).
  gyroX = Wire.read()<<8 | Wire.read();
  // Reading registers: 0x45 (gyroYOUT_H) and 0x46 (gyroYOUT_L).
	gyroY = Wire.read()<<8 | Wire.read(); 
  // Reading registers: 0x47 (gyroZOUT_H) and 0x48 (gyroZOUT_L).
	gyroZ = Wire.read()<<8 | Wire.read(); 

  // Convert into deg/s with range of 1000deg/s and compensate error.
	gyroXDgps = gyroX/32.8 - gyroDgpsError[0];
	gyroYDgps = gyroY/32.8- gyroDgpsError[1];
	gyroZDgps = gyroZ/32.8 - gyroDgpsError[2];

  // Write deg/s values into global array.
  gyroValuesDgps[0] = gyroXDgps;
  gyroValuesDgps[1] = gyroYDgps;
  gyroValuesDgps[2] = gyroZDgps;
}

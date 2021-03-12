// #include <Arduino.h>
// #include "Wire.h"

// NUJNE GLOBALNE SPREMENLJIVKE:
// int gyroValuesDeg []

void setGyro () {

    const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

    Wire.begin();
	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
	Wire.write(0x6B); // PWR_MGMT_1 register
	Wire.write(0); // set to zero (wakes up the MPU-6050)
	Wire.endTransmission(true);

	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
	Wire.write(0x1B); // GYRO_CONFIG register
    Wire.write(0x10); // send new values
	Wire.endTransmission(true); // end transmission

}

void readGyroDgps (int gyroValuesDgps [],int Gyro_raw_error [] ) {
    // return dgps gyro [x, y, z] values in deg

    const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.


    int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
    int16_t gyro_x_dgps, gyro_y_dgps, gyro_z_dgps;
    int16_t Gyro_raw_error_x = 0, Gyro_raw_error_y = 0, Gyro_raw_error_z = 0;

    Wire.beginTransmission(MPU_ADDR);
	Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
	
    gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
	gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
	gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

    	// converting gyro in degrees/s

	gyro_x_dgps = gyro_x/32.8 - Gyro_raw_error_x;
	gyro_y_dgps = gyro_y/32.8- Gyro_raw_error_y;
	gyro_z_dgps = gyro_z/32.8 - Gyro_raw_error_z;

    gyroValuesDgps[0] = gyro_x_dgps;
    gyroValuesDgps[1] = gyro_y_dgps;
    gyroValuesDgps[2] = gyro_z_dgps;
}


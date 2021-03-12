#include <Arduino.h>
#include "Wire.h"



const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
int8_t gyro_config;

int16_t gyro_x_dgps, gyro_y_dgps, gyro_z_dgps;
int16_t Gyro_raw_error_x = 0, Gyro_raw_error_y = 0, Gyro_raw_error_z = 0;
int16_t gyro_x_angle, gyro_y_angle, gyro_z_angle;

float elapsedTime = 0;
float timeRead = 0;

float rad_to_deg = 57.2; //180

float Acc_angle_x;
float Acc_angle_y;


char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
	sprintf(tmp_str, "%6d", i);
	return tmp_str;
}

int gyroValuesDgps[3];
int Gyro_raw_error[3];


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

void readGyroDgps () {
    // return dgps gyro [x, y, z] values in deg

    const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.


    int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
    int16_t gyro_x_dgps, gyro_y_dgps, gyro_z_dgps;
    // int16_t Gyro_dgps_error_x = 0, Gyro_dgps_error_y = 0, Gyro_dgps_error_z = 0;

    Wire.beginTransmission(MPU_ADDR);
	Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
	
    gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
	gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
	gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

    	// converting gyro in degrees/s

	gyro_x_dgps = gyro_x/32.8 - Gyro_raw_error[0];
	gyro_y_dgps = gyro_y/32.8 - Gyro_raw_error[1];
	gyro_z_dgps = gyro_z/32.8 - Gyro_raw_error[2];

    gyroValuesDgps[0] = gyro_x_dgps;
    gyroValuesDgps[1] = gyro_y_dgps;
    gyroValuesDgps[2] = gyro_z_dgps;
}


void calibrateGyroError () {

    const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

    int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
    int16_t gyro_x_dgps, gyro_y_dgps, gyro_z_dgps;
    // int16_t Gyro_dgps_error_x = 0, Gyro_dgps_error_y = 0, Gyro_dgps_error_z = 0;

    float gyro_error_x = 0;
    float gyro_error_y = 0;
    float gyro_error_z = 0;

    for (int i = 0; i <1000; i++) {

        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
        Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
        Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
        
        gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
        gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
        gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

        gyro_x_dgps = gyro_x/32.8;
        gyro_y_dgps = gyro_y/32.8;
        gyro_z_dgps = gyro_z/32.8;

        gyro_error_x += gyro_x_dgps;
        gyro_error_y += gyro_y_dgps;
        gyro_error_z += gyro_z_dgps;
		//Serial.println(gyro_y_dgps);

    }
    
    Gyro_raw_error[0] = gyro_error_x / 1000;
    Gyro_raw_error[1] = gyro_error_y / 1000;
    Gyro_raw_error[2] = gyro_error_z / 1000;
}









void setup() {
Serial.begin(9600);
setGyro();
calibrateGyroError();
Serial.println("---------------------");
Serial.println(Gyro_raw_error[0]);
Serial.println(Gyro_raw_error[1]);
Serial.println(Gyro_raw_error[2]);
Serial.println("---------------------");

Wire.beginTransmission(MPU_ADDR);
Wire.write(0x1B);
Wire.endTransmission(true);
Wire.requestFrom(MPU_ADDR, 1, true);
Serial.print(Wire.read());
}

void loop() {
readGyroDgps();
//Serial.println(gyroValuesDgps[2]);
	
}
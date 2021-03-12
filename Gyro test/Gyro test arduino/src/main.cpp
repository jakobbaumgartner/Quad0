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


void setup() {

	Serial.begin(9600);
	Wire.begin();
	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
	Wire.write(0x6B); // PWR_MGMT_1 register
	Wire.write(0); // set to zero (wakes up the MPU-6050)
	Wire.endTransmission(true);

	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
	Wire.write(0x1B); // GYRO_CONFIG register
	//Wire.requestFrom(MPU_ADDR, 8);
	// Wire.endTransmission();
	// Serial.println(gyro_config);
	// gyro_config = Wire.read();
	// Serial.println(gyro_config);
	//gyro_config |= 0b00010000; // change bit to 1000 scale of range
	// Serial.println(gyro_config);
	// Wire.write(0x1B); // select regitster GYRO_CONFIG
	Wire.write(0x10); // send new values
	Wire.endTransmission(true); // end transmission

	Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
	Wire.write(0x1B); // GYRO_CONFIG register
	Wire.endTransmission(true); // end transmission
	Wire.requestFrom(MPU_ADDR, 1, true);

	gyro_config = Wire.read();
	Serial.print(gyro_config);

}

void loop() {

	Wire.beginTransmission(MPU_ADDR);
	Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
	Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
	Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
	
	// "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
	accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
	accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
	accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
	temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
	gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
	gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
	gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
	
	// print out data
	// Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
	// Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
	// Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
	// // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
	// Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
	// Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
	// Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
	// Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
	// Serial.println("");


	// converting gyro in degrees/s

	gyro_x_dgps = gyro_x/32.8 - Gyro_raw_error_x;
	gyro_y_dgps = gyro_y/32.8- Gyro_raw_error_y;
	gyro_z_dgps = gyro_z/32.8 - Gyro_raw_error_z;

	// integrating degrees gyro

	elapsedTime = (millis() - timeRead)/1000;
	timeRead = millis();

	gyro_x_angle = gyro_x_angle + gyro_x_dgps*elapsedTime;
	gyro_y_angle = gyro_y_angle + gyro_y_dgps*elapsedTime;
	gyro_z_angle = gyro_z_angle + gyro_z_dgps*elapsedTime;



	// printing new values

	// Serial.print(" | gX_angle = "); Serial.print(convert_int16_to_str(gyro_x_angle));
	// Serial.print(" | gY_angle = "); Serial.print(convert_int16_to_str(gyro_y_angle));
	// Serial.print(" | gZ_angle = "); Serial.print(convert_int16_to_str(gyro_z_angle));
	// Serial.println("");
	// Serial.println(elapsedTime);
	
	// Serial.print(" | gX_angle dpgs = "); Serial.println(convert_int16_to_str(gyro_x_dgps));
	// Serial.print(" | gY_angle dpgs = "); Serial.println(convert_int16_to_str(gyro_y_dgps));
	// Serial.print(" | gZ_angle dpgs = "); Serial.println(convert_int16_to_str(gyro_z_dgps));
	// Serial.println("");
	

	// acc angle converter

	 /*---X---*/
	Acc_angle_x = (atan((accelerometer_y)/sqrt(pow((accelerometer_x),2) + pow((accelerometer_z),2)))*rad_to_deg); // - Acc_angle_error_x;
	/*---Y---*/
	Acc_angle_y = (atan(-1*(accelerometer_x)/sqrt(pow((accelerometer_y),2) + pow((accelerometer_z),2)))*rad_to_deg); // - Acc_angle_error_y; 
	/*---Z---*/
	// Acc_angle_z = (atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_y; 
	
	// Serial.print("\n\n | gX_angle acc = "); Serial.print((Acc_angle_x));
	// Serial.print(" | gY_angle acc = "); Serial.print((Acc_angle_y));


	// delay();
	// complementary filter
}
//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Licensed under the CC BY-NC SA 4.0

//Include the header file for this class

#include "MPU6050.h"
#include <chrono>

using namespace std;

MPU6050::MPU6050(int8_t addr, int8_t bus_num, bool run_update_thread) {
	int status;

	MPU6050_addr = addr;
	dt = 0.009; //Loop time (recalculated with each loop)
	_first_run = 1; //Variable for whether to set gyro angle to acceleration angle in compFilter
	calc_yaw = false;

	string i2c_bus = "/dev/i2c-" + to_string(bus_num);

	f_dev = open(i2c_bus.c_str(), O_RDWR); //Open the I2C device file
	if (f_dev < 0) { //Catch errors
		cout << "ERR (MPU6050.cpp:MPU6050()): Failed to open /dev/i2c-7. Please check that I2C is enabled\n"; //Print error message
	}

	status = ioctl(f_dev, I2C_SLAVE, MPU6050_addr); //Set the I2C bus to use the correct address
	if (status < 0) {
		cout << "ERR (MPU6050.cpp:MPU6050()): Could not get I2C bus with " << addr << " address. Please confirm that this address is correct\n"; //Print error message
	}

	config[0] = 0x6B; // PWR_MGMT_1 register
    config[1] = 0x00; // Wake up device
	write(f_dev, config, 2); //Take MPU6050 out of sleep mode - see Register Map

	// char data[2];

	// data[0] = 0x1a;
	// data[1] = 0b00000011;
	// write(f_dev, data, 2); // Set DLPF (low pass filter) to 44Hz (so no noise above 44Hz will pass through)

	// data[0] = 0x19;
	// data[1] = 0b00000100;
	// write(f_dev, data, 2); // Set sample rate divider (to 200Hz) - see Register Map

	// data[0] = 0x1b;
	// data[1] = GYRO_CONFIG;
	// write(f_dev, data, 2); // Configure gyroscope settings - see Register Map (see MPU6050.h for the GYRO_CONFIG parameter)

	// data[0] = 0x1c;
	// data[1] = ACCEL_CONFIG;
	// write(f_dev, data, 2); // Configure accelerometer settings - see Register Map (see MPU6050.h for the GYRO_CONFIG parameter)

	// // //Set offsets to zero
	// data[0] = 0x06;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x07;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x08;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x09;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x0A;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x0B;
	// data[1] = 0b00000000;
	// write(f_dev, data, 2);

	// data[0] = 0x00;
	// data[1] = 0b10000001;
	// write(f_dev, data, 2);

	// data[0] = 0x01;
	// data[1] = 0b00000001;
	// write(f_dev, data, 2);

	// data[0] = 0x02;
	// data[1] = 0b10000001;
	// write(f_dev, data, 2);

	// if (run_update_thread){
	// 	thread(&MPU6050::_update, this).detach(); //Create a seperate thread, for the update routine to run in the background, and detach it, allowing the program to continue
	// }

	cout << "MPU6050 initialized" << endl; //Print message
}

MPU6050::MPU6050(int8_t addr) : MPU6050(addr, 7, true){}

MPU6050::MPU6050() : MPU6050(0x68, 7, true){}

void MPU6050::getSensorRaw(float *x, float *y, float *z, float *roll, float *pitch, float *yaw, float *temp) {
	write(f_dev, config, 2);
	char reg[1] = {0x3B};
	write(f_dev, reg, 1); //Set the register to read from

	char data[14] = {0};
	if (read(f_dev, data, 14) != 14) {
		cout << "ERR (MPU6050.cpp:getSensorRaw()): Failed to read from the I2C bus\n"; //Print error message
	}	

	// raw acceleration
	int16_t aX = (data[0] << 8) | data[1]; //Read X registers
	int16_t aY = (data[2] << 8) | data[3]; //Read Y registers
	int16_t aZ = (data[4] << 8) | data[5]; //Read Z registers
	*x = (float)aX;
	*y = (float)aY;
	*z = (float)aZ;

	// raw temperature
	int16_t T = data[6] << 8 | data[7]; //Read temperature registers
	*temp = (float)T; //Store in variable

	// raw gyroscope
	int16_t gX = (data[8] << 8) | data[9]; //Read X registers
	int16_t gY = (data[10] << 8) | data[11]; //Read Y registers
	int16_t gZ = (data[12] << 8) | data[13]; //Read Z registers
	*roll = (float)gX; //Roll on X axis
	*pitch = (float)gY; //Pitch on Y axis
	*yaw = (float)gZ; //Yaw on Z axis	
}

void MPU6050::getIMU(float *ax, float *ay, float *az, float *gr, float *gp, float *gy, float *temp, long long *timestamp) {
	write(f_dev, config, 2);
	getSensorRaw(ax, ay, az, gr, gp, gy, temp); //Get the raw data from the sensors

	*ax = (*ax - A_OFF_X) / ACCEL_SENS * STANDARD_GRAVITY; //Remove the offset and divide by the accelerometer sensetivity (use 1000 and round() to round the value to three decimal places)
	*ay = (*ay - A_OFF_Y) / ACCEL_SENS * STANDARD_GRAVITY;
	*az = (*az - A_OFF_Z) / ACCEL_SENS * STANDARD_GRAVITY;

	*temp = *temp / 340.0 + 36.53; //Convert to degrees C (use 1000 and round() to round the value to three decimal places)

	*gr = (*gr - G_OFF_X) / GYRO_SENS * (M_PI / 180.0); // Convert to radians per second
	*gp = (*gp - G_OFF_Y) / GYRO_SENS * (M_PI / 180.0);
	*gy = (*gy - G_OFF_Z) / GYRO_SENS * (M_PI / 180.0);

	*timestamp = chrono::time_point_cast<chrono::nanoseconds>(chrono::system_clock::now()).time_since_epoch().count(); //Get the current time
}

int MPU6050::getAngle(int axis, float *result) {
	if (axis >= 0 && axis <= 2) { //Check that the axis is in the valid range
		*result = _angle[axis]; //Get the result
		return 0;
	}
	else {
		cout << "ERR (MPU6050.cpp:getAngle()): 'axis' must be between 0 and 2 (for roll, pitch or yaw)\n"; //Print error message
		*result = 0; //Set result to zero
		return 1;
	}
}

void MPU6050::_update() { //Main update function - runs continuously
	clock_gettime(CLOCK_REALTIME, &start); //Read current time into start variable

	while (1) { //Loop forever
		
		
		getIMU(&ax, &ay, &az, &gr, &gp, &gy, &temp, &timestamp); //Get the raw data from the sensors

		//X (roll) axis
		_accel_angle[0] = atan2(az, ay) * RAD_T_DEG - 90.0; //Calculate the angle with z and y convert to degrees and subtract 90 degrees to rotate
		_gyro_angle[0] = _angle[0] + gr*dt; //Use roll axis (X axis)

		//Y (pitch) axis
		_accel_angle[1] = atan2(az, ax) * RAD_T_DEG - 90.0; //Calculate the angle with z and x convert to degrees and subtract 90 degrees to rotate
		_gyro_angle[1] = _angle[1] + gp*dt; //Use pitch axis (Y axis)

		//Z (yaw) axis
		if (calc_yaw) {
			_gyro_angle[2] = _angle[2] + gy*dt; //Use yaw axis (Z axis)
		}


		if (_first_run) { //Set the gyroscope angle reference point if this is the first function run
			for (int i = 0; i <= 1; i++) {
				_gyro_angle[i] = _accel_angle[i]; //Start off with angle from accelerometer (absolute angle since gyroscope is relative)
			}
			_gyro_angle[2] = 0; //Set the yaw axis to zero (because the angle cannot be calculated with the accelerometer when vertical)
			_first_run = 0;
		}

		float asum = abs(ax) + abs(ay) + abs(az); //Calculate the sum of the accelerations
		float gsum = abs(gr) + abs(gp) + abs(gy); //Calculate the sum of the gyro readings

		for (int i = 0; i <= 1; i++) { //Loop through roll and pitch axes
			if (abs(_gyro_angle[i] - _accel_angle[i]) > 5) { //Correct for very large drift (or incorrect measurment of gyroscope by longer loop time)
				_gyro_angle[i] = _accel_angle[i];
			}

			//Create result from either complementary filter or directly from gyroscope or accelerometer depending on conditions
			if (asum > 0.1 && asum < 3 && gsum > 0.3) { //Check that th movement is not very high (therefore providing inacurate angles)
				_angle[i] = (1 - TAU)*(_gyro_angle[i]) + (TAU)*(_accel_angle[i]); //Calculate the angle using a complementary filter
			}
			else if (gsum > 0.3) { //Use the gyroscope angle if the acceleration is high
				_angle[i] = _gyro_angle[i];
			}
			else if (gsum <= 0.3) { //Use accelerometer angle if not much movement
				_angle[i] = _accel_angle[i];
			}
		}

		//The yaw axis will not work with the accelerometer angle, so only use gyroscope angle
		if (calc_yaw) { //Only calculate the angle when we want it to prevent large drift
			_angle[2] = _gyro_angle[2];
		}
		else {
			_angle[2] = 0;
			_gyro_angle[2] = 0;
		}

		clock_gettime(CLOCK_REALTIME, &end); //Save time to end clock
		dt = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; //Calculate new dt
		clock_gettime(CLOCK_REALTIME, &start); //Save time to start 
	}
}

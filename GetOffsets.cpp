//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

//Example code

#include <MPU6050.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
	float ax, ay, az, gr, gp, gy; // Variables to store the accel, gyro, and angle values

	sleep(1); // Wait for the MPU6050 to stabilize

    MPU6050 device(0x68);

	device.getOffsets(&ax, &ay, &az, &gr, &gp, &gy);

    std::cout << "Offsets: ax: " << ax << ", ay: " << ay << ", az: " << az << ", gr: " << gr << ", gp: " << gp << ", gy: " << gy << "\n";

	return 0;
}



//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

//Example code

#include <MPU6050.h>
#include <iostream>
#include <chrono>
#include <thread>

MPU6050 device(0x68);

int main() {
	float ax, ay, az, gr, gp, gy; // Variables to store the accel, gyro, and angle values

	sleep(1); // Wait for the MPU6050 to stabilize

	auto start = std::chrono::steady_clock::now();
	auto end = start + std::chrono::seconds(10); // Run for 10 seconds

	double sum = 0.0;
	double count = 1.0;

	while (std::chrono::steady_clock::now() < end) {
		// Get the current accelerometer values

		auto start_interval = std::chrono::steady_clock::now();

		device.getAccel(&ax, &ay, &az);
		std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";

		// Get the current gyroscope values
		device.getGyro(&gr, &gp, &gy);
		std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";

		// usleep(25000); // 0.025sec
		// std::this_thread::sleep_for(std::chrono::milliseconds(25)); // Sleep for 25 milliseconds

		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_interval);
		count++;
		sum += ax;
	}

	std::cout << "Average ay: " << sum / count << "\n";

	return 0;
}



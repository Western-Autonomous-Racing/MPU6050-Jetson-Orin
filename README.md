# MPU6050 Accelerometer and Gyroscope C++ library for Jetson Orin NX.

## Description
This library has been adapted to be used on a Jetson Orin NX. Tested on a Seeed Studio reComputer J4012-Edge AI Device. Based on alex mous's Raspberry Pi library.

### Installation

Dependencies:

```bash 
sudo apt install libi2c-dev i2c-tools libi2c0
```

```bash 
```

### Function Definitions
#### __constructor__ (MPU6050)
- **args:**
    - int8_t addr - the address of the MPU6050 (usually 0x68; can find with command "i2cdetect -y 1" (may need to be installed - run "sudo apt-get install i2c-tools -y")
- **description:**
    - sets up i2c device and starts loop to read the angle

#### getAccelRaw
- **args:**
    - float *x - pointer to the variable where the X axis results should be stored
    - float *y - pointer to the variable where the Y axis results should be stored
    - float *z - pointer to the variable where the Z axis results should be stored
- **description:**
    - gets the raw accelerometer values from the MPU6050 registers

#### getAccel
- **args:**
    - float *x - pointer to the variable where the X axis results should be stored
    - float *y - pointer to the variable where the Y axis results should be stored
    - float *z - pointer to the variable where the Z axis results should be stored
- **description:**
    - gets the accelerometer values (in g), rounded to three decimal places

#### getGyroRaw
- **args:**
    - float *roll - pointer to the variable where the roll (X) axis results should be stored
    - float *pitch - pointer to the variable where the pitch (Y) axis results should be stored
    - float *yaw - pointer to the variable where the yaw (Z) axis results should be stored
- **description:**
    - gets the raw gyroscope values from the MPU6050 registers

#### getGyro
- **args:**
    - float *roll - pointer to the variable where the roll (X) axis results should be stored
    - float *pitch - pointer to the variable where the pitch (Y) axis results should be stored
    - float *yaw - pointer to the variable where the yaw (Z) axis results should be stored
- **description:**
    - gets the gyroscope values (in degrees/second)

#### getOffsets
- **args:**
    - float *ax_off - pointer to the variable where the accelerometer x axis offset will be stored
    - float *ay_off - pointer to the variable where the accelerometer y axis offset will be stored
    - float *az_off - pointer to the variable where the accelerometer z axis offset will be stored
    - float *gr_off - pointer to the variable where the gyroscope roll axis offset will be stored
    - float *gp_off - pointer to the variable where the gyroscope pitch axis offset will be stored
    - float *gy_off - pointer to the variable where the gyroscope yaw axis offset will be stored
- **description:**
    - finds the offsets needed
    - before running, place the module on a completely flat surface (check with a spirit level if possible) and make sure that it stays still while running this function, the results will be stored in the variables ax_off, ay_off, az_off, gr_off, gp_off, gy_off for accel x offset... and gyro roll offset..., take these values and put them into the MPU6050.h file (A_OFF_X, A_OFF_Y, A_OFF_Z, G_OFF_X, G_OFF_Y and G_OFF_Z)

#### getAngle
- **args:**
    - int axis - which axis to use (0 for roll (x), 1 for pitch (Y) and 2 for yaw (Z))
    - float *results - pointer to the variable where the angle will be stored
- **description:**
    - gets the current combined (accelerometer and gyroscope) angle
    - NOTE: the yaw axis will return 0 unless 'calc_yaw' is set to true - See Parameters

### Parameters:
#### calc_yaw
- **type:**
    - bool
- **description:**
    - set this to true when you want to calculate the angle around the yaw axis (remember to change this back to false after taking the yaw readings to prevent gyroscope drift)
    - this is used to prevent drift because only the gyroscope is used to calculate the yaw rotation

Copyright (c) 2019, Alex Mous
Licensed under the Creative Commons Attribution-ShareAlike 4.0 International (CC-BY-4.0)

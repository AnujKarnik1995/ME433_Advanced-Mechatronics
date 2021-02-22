### HW6: Interfacing the IMU unit.<br>

This part deals with adding an IMU unit to the mix.<br>
We've used the LSM6DS33 accelerometer and gyroscope chip made by ST, soldered onto a breakout board by Pololu. It also has a temperature sensor.<br><br>
To approach this problem, we create the 'imu.c' file which has functions to access the correct addresses of registers storing sensor data.<br><br>

The chip communicates over the I2C bus with the PIC.<br>
So, we modify the I2C library with the I2C_read_multiple() function which can read multiple registers from the IMU chip. We need the accelerometer data (x,y,z) and the temperature data.<br>
<br>
Using the OLED library, the raw data from the IMU is displayed on the screen.<br>
<br>
As an intersting challenge, I created a digital spirit level using the IMU and the display.<br>
Taking into account the display resolution and the maximum tilt readings of the IMU, I mapped the tilt readings to the display.<br>
Finally, the display shows a line which reacts based on how much the IMU is tilted.<br><br>
The final output videos can be seen in the portfolio.<br>
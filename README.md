# Advanced Mechatronics
This repository contains code for the homework assignments of Advenced Mechatronics (ME433) taught at Northwestern.

### Author: Anuj Karnik.

### Overview:
We used a PIC32MX270F256B (SPDIP) and added various sensors. <br>

### Hardware, Firmware and other tools:
1. A PIC32MX270F256B microcontroller is SPDIP format.
2. An SSD1306 OLED display.
3. Input/Output Expander MCP23017.
4. DAC module (SPI) MCP4912.
5. IMU unit LSM6D533.
6. WS2812B addressable LEDs (Neopixels).
7. MPLAB IDE with SNAP debugger.
8. n-Scope portable oscilloscope.

### File structure details:
Every sub-directory has its own 'main' function.
├── HW1 --> Basic setup with voltage regulator and getting the LED to blink.<br>
├── HW2 --> Adding the SPI based DAC. Generating a triangle and a sine wave .<br>
├── HW3 --> Adding the I2C based I/O expander.<br>
├── HW4 --> Interfacing the OLED display<br> 
├── HW5 --> Generating the rainbow pattern with Neopixel LEDs.<br>
├── HW6 --> Interfacing the IMU and creating a spirit level.<br>
└── HW8 --> Implementing RTCC based clock.<br><br>

For more information, videos and images, you can visit my [portfolio](https://sites.google.com/u.northwestern.edu/anuj-karnik/projects/advanced-mechatronics).
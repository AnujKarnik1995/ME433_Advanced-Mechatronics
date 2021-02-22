### HW3: Interfacing the I/O Expander.
<br>
This assignment deals with interfacing the MCP23017 I/O expander with the PIC.<br>
<br>
A general purpose library for I2C communication has been created (i2c_master_noint.c).<br>
It has functions to initialize the chip and setup commnication between the devices over the I2C bus.<br>

A button is connected to the expander. Its function is to toggle the state of an LED (connected to another GPIO pin of the expander).<br> 
When it is pressed, a signal is sent to the PIC. The PIC sends out a toggle signal to the expander and the LED is toggled.<br>
This way, we can confirm two-way communiaction between the devices over th I2C bus.<br>
<br>
The results for this can be seen in the portfolio linked in the exterior README.


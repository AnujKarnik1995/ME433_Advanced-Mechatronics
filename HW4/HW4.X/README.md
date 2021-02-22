### HW4: Interfacing the SSD1304 OLED display.<br>
This part deals with creating a I2C based library for the display.<br>
The idea is to create a character buffer which can hold the message, send ot over I2C and refresh the screen to view the text.<br>
The screen supports a refresh rate of 80Hz, though after 75, it starts getting jittery.<br> The code automatically wraps text if the string size is more than the length of the display.It moves to the next line.
<br>
<br>
The font.c file contains code to the alphabets, numbers and symbols. It acts as a wrapper funtion between pixel level LED control and string input from user.<br>
<br>
The results can be seen in the portfolio linked outside.

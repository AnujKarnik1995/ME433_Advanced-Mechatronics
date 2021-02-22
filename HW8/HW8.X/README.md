### HW8: RTCC Functionality. <br>
This assignment dealt with implementing the Real Time Clock and Calendar (RTCC) functionality on the PIC32.<br>
For this, we've used an external oscillator. We need to set the PIC register FNOSCEN to enable the use of the external oscillator.<br>
We manipulate the RTCDATE and RTCTIME registers.<br><br>
Using a function, rtcc_setup(), we feed the current date and time in a 32 bit unsigned int format.<br>
To display the days, I've created an array containing the days of the week.<br>
<br>
The date and time is displayed on the OLED screen. The result can be seen in the portfolio.
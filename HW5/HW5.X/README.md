### HW5: Adding addressable LEDs (Neopixels)<br>

This part deals with interfacing multiple addressable LEDs to a single pin of the PIC32.<br>
The LEDs have a built in controller that takes a single digital signal to set the brightness of RGB LEDs.<br>
The LEDs can also be chained together. If you immediately send one color after another, the data is passed onto the next LED.<br>
<br>
The LEDs are operated on the principle of 'bit-banging'. This method is very CPU intensive<br>
A logic One bit is high for 1.36uS and low for 0.35uS, and a logic Zero is high for 0.35uS and low for 1.36uS, each deviating by no more than 0.15uS.<br>
If there is ever more than 50uS of low, the pass-through starts over (so this is the reset time).
<br>

Once the LEDs are configured and work properly, we use the HSBtoRGB() function to generate a rainbow pattern.<br>
<br>
The output of this can be seen in the portfolio.
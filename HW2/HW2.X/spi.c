// initialize SPI1
void initSPI() {
    ANSELA = 0; //Analog pins off 
    TRISAbits.TRISA0 = 0O; //Configure as output
    LATAbits.LATA0 = 1; //Make as output
    RPA1Rbits.RPA1R = 0b0011; //SDO1
    SDI1Rbits.SDI1R = 0b0001; //SDI1
    
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}

unsigned char spi_io(unsigned char o){
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF){
        ;
    }
    return SPI1BUF;
}
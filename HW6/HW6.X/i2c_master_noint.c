// I2C Master utilities, using polling rather than interrupts
// The functions must be called in the correct order as per the I2C protocol
// I2C pins need pull-up resistors, 2k-10k
#include <stdio.h>
#include "i2c_master_noint.h"
#include "imu.h"
#include "font.h"
#include "ssd1306.h"

/*MY FUNCTIONS*/

void i2c_read_multiple(unsigned char address, unsigned char reg, unsigned char * raw_data, int length){
    
    int k=0, i=0, q=0;
    unsigned char add_read = (address|(0x01));  //converts write address to read address
            
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(reg);            
    i2c_master_restart();
    i2c_master_send(add_read); 
    
    // LOOP TO RUN THROUGH ACK(0) FOR APPROPRIATE NUMBER OF TIMES.
    while(k<(length-1)){
        raw_data[k] = i2c_master_recv(); 
        i2c_master_ack(0);           //so that ack(0) is not sent for the last read
        k++;   
    }
    
    /* USED FOR LOOP AS WELL, STILL SAME RESULTS
    for(k=0; k<length; k++){
        data1[k] = i2c_master_recv(); 
        if((k)!=(length-1)){
        i2c_master_ack(0);           //so that ack(0) is not sent for the last read
        }
    }*/
    raw_data[k] = i2c_master_recv(); //FINAL RECEIVE
    i2c_master_ack(1);               //FINAL ACKNOWLEDGE 1
    i2c_master_stop();
}

void setPin(unsigned char address, unsigned char command, unsigned char value){
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(command);
    i2c_master_send(value);
    i2c_master_stop();
}

unsigned char readPin(unsigned char address, unsigned char command, int ack){
    unsigned char add_read, add_ret;
    add_read = address|(0x01);//converts write address to read address
    i2c_master_start();
    i2c_master_send(address); //tells which register to write to
    i2c_master_send(command); //which pin to deal with
    i2c_master_restart();
    i2c_master_send(add_read); //tells which register to read from 
    add_ret=i2c_master_recv();
    i2c_master_ack(ack); 
    i2c_master_stop();
    return add_ret;   
}


/****/

void i2c_master_setup(void) {
    // using a large BRG to see it on the nScope, make it smaller after verifying that code works
    // look up TPGD in the datasheet
    I2C1BRG = 20; // I2CBRG = [1/(2*Fsck) - TPGD]*Pblck - 2 (TPGD is the Pulse Gobbler Delay)
    I2C1CONbits.ON = 1; // turn on the I2C1 module
}

void i2c_master_start(void) {
    I2C1CONbits.SEN = 1; // send the start bit
    while (I2C1CONbits.SEN) {
        ;
    } // wait for the start bit to be sent
}

void i2c_master_restart(void) {
    I2C1CONbits.RSEN = 1; // send a restart 
    while (I2C1CONbits.RSEN) {
        ;
    } // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
    I2C1TRN = byte; // if an address, bit 0 = 0 for write, 1 for read
    while (I2C1STATbits.TRSTAT) {
        ;
    } // wait for the transmission to finish
    if (I2C1STATbits.ACKSTAT) { // if this is high, slave has not acknowledged
        // ("I2C1 Master: failed to receive ACK\r\n");
        while(1){} // get stuck here if the chip does not ACK back
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C1CONbits.RCEN = 1; // start receiving data
    while (!I2C1STATbits.RBF) {
        ;
    } // wait to receive the data
    return I2C1RCV; // read and return the data
}

void i2c_master_ack(int val) { // sends ACK = 0 (slave should send another byte)
    // or NACK = 1 (no more bytes requested from slave)
    I2C1CONbits.ACKDT = val; // store ACK/NACK in ACKDT
    I2C1CONbits.ACKEN = 1; // send ACKDT
    while (I2C1CONbits.ACKEN) {
        ;
    } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) { // send a STOP:
    I2C1CONbits.PEN = 1; // comm is complete and master relinquishes bus
    while (I2C1CONbits.PEN) {
        ;
    } // wait for STOP to complete
}



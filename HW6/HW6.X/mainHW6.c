#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"i2c_master_noint.h" //I2C header file
#include "ssd1306.h"     // OLED screen
#include "font.h"           //font file for screen
#include <stdio.h>
#include "imu.h"            //IMU include file

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP =OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

int main() {
 
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    TRISBbits.TRISB4 = 1;
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 1;
    // do your TRIS and LAT commands here
    i2c_master_setup(); //INITIALIZING I2C
    ssd1306_setup(); //INITIALIZE THE OLED BOARD   
    imu_setup(); //INITIALIZE THE IMU OVER I2C
    
    __builtin_enable_interrupts();

    //INITIALIZATIONS
    unsigned char m[20];    //ARRAY FOR PRINTING
    signed short mess[7];   //ARRAY FOR STORING 7x16 BIT NUMBERS FROM IMU
    int map2x, map2y;
    while (1){
        
    /**HEARTBEAT LED**/
           
            LATAbits.LATA4 =1;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 =0;
           _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 =1;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 = 0;
    /****/  
            
            imu_read(IMU_OUT_TEMP_L,&mess[0],7);  
            /*
            sprintf(m,"t: %d\n", mess[0]); //PRINTING 7 IMU READINGS AND A COUNTER TO SCREEN
            drawString(1,0,&m[0]);
            sprintf(m,"g: %d %d %d\n", mess[1], mess[2],mess[3]);
            drawString(1,8,&m[0]);
            sprintf(m,"a: %d %d %d\n",mess[4],mess[5],mess[6]);
            drawString(1,16,&m[0]);
            */
            
            
            //*** INCLINOMETER ***//
            //The inclinometer functions like a SPIRIT LEVEL.
            //The inclinometer function uses data from x and y acc.
            //to create a 1:1 mapping to the screen resolution, we divide x by 128 pixels, so for each pixel
            // the counter has to increment/decrement by 260.
            map2x = mess[5]/260;        
            
            //siimlarly for y, dividing max reading by 32 gives us 1092. So for each pixel, the counter has to 
            //move by 1092     
            map2y = -1*(mess[4]/1092);  
            
            //changing the values of 260 & 1092 will change the sensitivity of the incliometer.
            //reducing them makes them more sensitive, while increasing them makes them less sensitive.
            
            inclinometer(map2x, map2y);
            
            
             
    }
}

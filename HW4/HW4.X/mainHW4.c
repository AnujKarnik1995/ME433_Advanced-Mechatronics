#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"i2c_master_noint.h" //I2C header file
#include "ssd1306.h"
#include "font.h"
#include <stdio.h>

/*
This code deals with the SSD1306 OLED display.
The display communicated over I2C so the i2c_master_setup() function is used.
ssd1306_setup() is a custom function that deals with all initializations for the screen.
The drawString() function allows text or numbers to be displayed on screen.
As before, the heartbeat LED (A4) is used to check if the screen has frozen.
*/

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

unsigned char ret_val=0x20, j=0, k=0;
unsigned char u[20]="Text Time Trial\n";
unsigned char m[100];
unsigned char FPS[30]="Just a 1 sec counter:\n";
unsigned char mess[50]="Twinkle twinkle little star..\n";
float q;


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
    
    __builtin_enable_interrupts();

    //INITIALIZATIONS
    drawString(1,1,&u[0]);
    drawString(1,9, &FPS[0]);
    drawString(1,25, &mess[0]); 
    _CP0_SET_COUNT(0);
    while (1){
    /**HEARTBEAT LED**/
        /*
            LATAbits.LATA4 =1;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 =0;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 =1;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 2400000/2){}
            LATAbits.LATA4 = 0;*/
    /****/  
    sprintf(m,"%0.3f\n",q);
    drawString(1,17,&m[0]);
    q=(_CP0_GET_COUNT()/24000000);
    }
}

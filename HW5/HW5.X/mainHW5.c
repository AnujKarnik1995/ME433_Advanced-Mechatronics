#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include"i2c_master_noint.h" //I2C header file
#include "ssd1306.h" //OLED screen header file
#include "font.h" //ASCII font header file
#include "ws2812b.h" //Neo LED Header file

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
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB5 = 1;
    // do your TRIS and LAT commands here
    i2c_master_setup(); //INITIALIZING I2C
    ssd1306_setup(); //INITIALIZE THE OLED BOARD 
    ws2812b_setup(); //INITIALIZE THE LEDS
    __builtin_enable_interrupts();
    
    
    //INITIALIZATIONS
    unsigned char rgb[50]="Array:\n";
    unsigned char m[100];
    float q,r,s;
    int a1 = 0, b2 = 45, c3 = 90, d4 = 140;
    //drawString(1,1,&rgb[0]);
    wsColor col[4];
            
    while (1) {      
    /**HEARTBEAT LED**/
            LATBbits.LATB5 =1;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 3600000){}
            LATBbits.LATB5 =0;
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 3600000){}
            
    /****/
            col[0] = HSBtoRGB(a1,1,0.3);
            col[1] = HSBtoRGB(b2,1,0.3);
            col[2] = HSBtoRGB(c3,1,0.3);
            col[3] = HSBtoRGB(d4,1,0.3);
            ws2812b_setColor(&col[0],4);
            
            a1 = b2;
            b2 = c3;
            c3 = d4;
            d4 = d4 + 40;
            if(d4>360){
                d4 = d4 - 360;
            }
                        
    
        
            
    }
}

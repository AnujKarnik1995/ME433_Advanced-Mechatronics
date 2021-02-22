#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"i2c_master_noint.h" //I2C header file
#include "ssd1306.h"
#include "font.h"
#include <stdio.h>
#include "rtcc.h"

/*
This program uses the rtccTime struct to store data regarding the date and time.
the FSOSCEN register has been set to ON enabling the use of an external oscillator.
The data is displayed onto the OLED screen.
If the PIC loses power, the user needs to reset the date and time.
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
#pragma config FSOSCEN = ON // enable secondary oscillator
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

    //TRISBbits.TRISB4 = 1;
    
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB5 = 1;
    // do your TRIS and LAT commands here
    i2c_master_setup(); //INITIALIZING I2C
    rtcc_setup(0x04030000, 0x20061000);
    
    ssd1306_setup(); //INITIALIZE THE OLED BOARD   
    
    
    __builtin_enable_interrupts();

    //INITIALIZATIONS    
    unsigned char u[11];
    unsigned char m[100];
    unsigned char d[100];
    unsigned char c[10];
    int ctr=0;  //counter for counting half a second
    rtccTime time;
    char day;       

    while (1){
        
    /**HEARTBEAT LED**/
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 4200000){}
            LATBbits.LATB5 =1;                
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 4200000){}   //LED Delay adjusted to half second to make counter implementation easy
            LATBbits.LATB5 =0;
           
    /****/  
            ctr = ctr+1;        //half second counter
            time= readRTCC();       
            sprintf(m,"%d%d:%d%d:%d%d\n",time.hr10,time.hr01,time.min10,time.min01,time.sec10,time.sec01);  //time array
            dayOfTheWeek(3, &day);      //day
            sprintf(u,"%s\n", &day);    //day array
            sprintf(d,"%d%d/%d%d/%d%d\n",time.mn10,time.mn01,time.dy10,time.dy01,time.yr10,time.yr01);      //date array
            drawString(1,19, &d[0]);        
            drawString(1,10,&u[0]);      
            drawString(1,1,&m[0]);
            sprintf(c,"counter = %d\n",ctr); //counter array
            drawString(55,19,&c[0]);            
    }
}

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"i2c_master_noint.h" //I2C header file

/*
This code has two parts. 
A heartbeat LED connected to A4 is blinking at a high frequency. This tells us if the PIC has frozen due to I2C bus errors.
The second part deals with setting up the I2C expander using the setPin and readPin functions.
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

unsigned char addw, addr, comm1, val1, comm2, val2, comm3, val3, comm4, val4;
unsigned char ret_val;
int ack=1;

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
    i2c_master_setup();
    __builtin_enable_interrupts();
   
    //INITIALISATIONS
    addr = 0b01000001; //address for reading
    addw = 0b01000000; //address for writing
    comm1 = 0x00; //accessing IODIRA
    val1 = 0x00; //value to be set to 0x00 for output
    comm2 = 0x01; //accessing IODIRB
    val2 = 0xFF; //value to be set to 0xFF for input
    comm3 = 0x14; //accessing OLATA
    val3 = 0x00; //value to set A pins as low- light OFF
    comm4 = 0x13; //accessing GPIOB
    val4 = 0xFF;//value to set A pins as high- light ON
       
    //INITIALISING GPA7 AS OUTPUT
    setPin(addw, comm1,val1);
    
    //INITIALISING GPB0 AS INPUT
    setPin(addw,comm2,val2);    
    /*
    
    //SANITY CHECK: BLINKING LED FROM B0
    setPin(addw,comm2,val1);
    //paste in while loop
    setPin(addw,0x15,val3);
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 4800000/2){}
           
            setPin(addw,0x15,val4);
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 4800000/2){}
            */
    
    while (1) {
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
            
        ret_val = readPin(addw,comm4,ack);
            //takes write address and ORs it to give read address
            // takes GPB0;
            // takes in acknowledge value as 1
            if((ret_val & 0x01)==(0x01)){ 
    //returns an 8 bit number of which LSB is relevant so AND it with 0x01
                setPin(addw,comm3,val4);
            }
            else{
                setPin(addw,comm3,val3);
            }    
    }

    return(0);
}

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>

/*
This function is used to create a 2Hz sine wave and a 1Hz triangle wave.
Both the waves range from 0V to 3.3V.
The voltage values needed to create the trinagle wave are calculated and stored in a buffer (line 78).
The voltage values needed to create the sine wave are calculated and stored in a buffer (line 104).
The spi_io() function is used to send the buffer data over SPI to the DAC module. 
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

void initSPI();
unsigned char spi_io(unsigned char o);

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
    LATAbits.LATA4 = 0;
    TRISAbits.TRISA0 = 0; //Configure as output
    LATAbits.LATA0 = 1; //Initialize as high
    // do your TRIS and LAT commands here
    initSPI();
    __builtin_enable_interrupts();
 
    unsigned char i = 0, r=0;
    int u = 1,q=149;
    unsigned short vtg12[600]={0};
    unsigned short vtgsin[150]={0};
    unsigned char aorb;
    unsigned short p,s, finsin[150], fintri[600]={0};
    
    /* START OF TRIANGLE WAVE CALCULATIONS*/
    //CREATING ARRAY OF VALUES FOR TRIANGULAR WAVE
    while(u<=150){
        vtg12[u]=(4095*(u))/150;
            while(u==150 && q==149){
                while(u<300 && q>0){
                    vtg12[u]=(4095*(q))/150;
                    q=q-1;
                    u=u+1;
                }
            }
        u++;
    }
    u=1;
    //TO MERGE VALUES TO CREATE FINAL 16BIT NUMBER
    while(u<=300){  
    aorb = 1; //VOUTA
    r = 0b111;
    p= aorb<<15;
    p= p|(r<<12);
    fintri[u]=p|vtg12[u]; //FINTRI is the final array consisting of 16bit number.
    u++; 
    }
    /* END OF TRIANGLE CALCULATIONS*/
    
    u=1, q=149; //RESETTING COUNTERS.
    
    /* START OF SINE WAVE CALCULATIONS */
    while(u<=150){
        vtgsin[u]= (4096/2)+((4096/2)*(sin(2*3.14*(u)/150)));
        u++;
    }
    
    //final loop for sine wave
    u=1;
    while(u<151){
        aorb=0; //VOUTB
        r=0b111;
        s=aorb<<15;
        s=s|(r<<12);
        finsin[u]=s|vtgsin[u];
        u++;
    }
    /* END OF SINE WAVE CALCULATIONS */

    //FINTRI AND FINSIN ARE THE TWO FINAL ARRAYS.
    q=1,u=1;
    while(1){
    _CP0_SET_COUNT(0);
    LATAbits.LATA0 = 0;//insert LAT ON here
    spi_io(finsin[u]>>8);//sine wave
    spi_io(finsin[u]);
    LATAbits.LATA0 = 1;//latoff here
    LATAbits.LATA0 = 0;//insert LAT ON here
    spi_io(fintri[q]>>8);//triangle wave
    spi_io(fintri[q]);//spip1 and spi1 here
    LATAbits.LATA0 = 1;//latoff here
    while(_CP0_GET_COUNT()<80000){ //delay for 300 data points
            ; //delay
    }
    q++;
    u++;
    if(u==150){
        u=1;
    }
    if(q==300){
        q=1;
    }
    }
    
    return (0);   
    }

//}

void initSPI() {
    ANSELA = 0; //Analog pins off 
    TRISAbits.TRISA0 = 0; //Configure as output
    LATAbits.LATA0 = 1; //Initialize as high
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
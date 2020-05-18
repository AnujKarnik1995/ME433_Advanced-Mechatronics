#include "imu.h"
#include "i2c_master_noint.h"
#include <stdio.h>
#include "ssd1306.h"
#include "font.h"

void imu_setup(){
    //check who am I for every loop
    unsigned char who,ant,b,c;
    unsigned char try[100];
    who = readPin(IMU_ADDR, IMU_WHOAMI, 1); // read from IMU_WHOAMI
    if (who != 0b01101001){
        while(1){  //blinks if IMU read fails
        
     }}
    setPin(IMU_ADDR ,IMU_CTRL1_XL, 0b10000010 );// init IMU_CTRL1_XL ; read back to check values 0x82
    setPin(IMU_ADDR, IMU_CTRL2_G, 0b10001000);// init IMU_CTRL2_G 0x88
    setPin(IMU_ADDR, IMU_CTRL3_C, 0b00000100);// init IMU_CTRL3_C  0x04
    }

void imu_read(unsigned char reg, signed short * data, int len){
    int i=0;
    int q=0;
    unsigned char raw_data[2*len]; //raw_data REGISTER TO STORE 14x8 BIT RAW READINGS FROM IMU
    

    i2c_read_multiple(IMU_ADDR, reg, &raw_data[0], (2*len));  // read multiple from the imu, each data takes 2 reads so you need len*2 chars
    
    i=0;
    q=0;
    while(i<(len*2)){
        data[q]=((raw_data[i+1]<<8)|(raw_data[i]));           //CREATING 16BIT NUMBERS BY BIT SHIFTING raw_data HIGH BIT AND ORing WITH LOW BIT
        i=i+2;
        q=q+1;
           
    }
        
    /* CHECKED CONVERTING TO 16 BIT NUMBERS USING FOR LOOP AS WELL.
    for((i=0); (i<(len*2)); (i=i+2)){
        data[q]= ((raw_data[i])|(raw_data[i+1]<<8));          // turn the chars into the shorts   
        q=q+1;
    }*/
    
     /* CHECKED OUTPUT USING SINGLE REGISTER READ. STILL ZEROS.
    unsigned char try[100];
    
    //raw_data[0] = readPin(IMU_ADDR,0x20,1);         //address is TEMP_LOW
    //raw_data[1] = readPin(IMU_ADDR, 0x21,1);        //address is TEMP_HIGH
    //data[0] = ((raw_data[1]<<8)|(raw_data[0]));     
    sprintf(try,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",raw_data[0],raw_data[1], raw_data[2], raw_data[3],raw_data[4],raw_data[5],raw_data[6],raw_data[7],raw_data[8],raw_data[9],raw_data[10],raw_data[11],raw_data[12],raw_data[13]); //,data[2],data[3],data[4],data[5]);
    drawString(1,1,&try[0]);*/
    
}

void inclinometer(int x, int y){
            int cx, cy;
            //to keep aesthetic appeal, added a 4 pixel centre.
            ssd1306_clear();
            ssd1306_drawPixel(65,16,1);
            ssd1306_drawPixel(64,16,1);
            ssd1306_drawPixel(64,17,1);
            ssd1306_drawPixel(65,17,1);
            ssd1306_update();
            ////****////
            
            //X inclinometer
            while(x!=(0)){
                cx = 64 + x;
                ssd1306_drawPixel(cx,16,1);
                ssd1306_drawPixel(cx,17,1);
                if(x<0){x++;}
                else(x--);
            }
            
            //Y inclinometer
            while(y!=(0)){
                cy = 16 + y;
                ssd1306_drawPixel(64,cy,1);
                ssd1306_drawPixel(65,cy,1);
                if(y<0){y++;}
                else(y--);                
            }
            
            ssd1306_update();
}
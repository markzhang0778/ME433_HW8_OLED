#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "font.h"
#include <stdio.h>

#define data_pts 100

//ADDRESS of the IMU: 0b1101000 = 0x68
//ADDRESS of the OLED: 0b01111000

void draw_char(unsigned char x, unsigned char y, unsigned char ascii);
void draw_string(unsigned char x, unsigned char y, unsigned char* str);


int main(void) {
    NU32DIP_Startup();
    i2c_master_setup();
    ssd1306_setup();
    init_mpu6050();

    _CP0_SET_COUNT(0);
    float az = 0;
    //keep track of time between screen update
    float fps = 0;
    char IMU_BUF[14];
    unsigned char display_a[50];
    unsigned char display_fps[50];
    
    while (1) {
        _CP0_SET_COUNT(0);
        
        ssd1306_clear();
        burst_read_mpu6050(IMU_BUF);
        az = conv_zXL(IMU_BUF);
        sprintf(display_a, "z acceleration = %.3f", az);
        sprintf(display_fps, "FPS = %.3f", fps);
        draw_string(2, 2, display_a);       
        draw_string(2, 12, display_fps);
        ssd1306_update();
        //calculate fps
        fps = (float)24000000/_CP0_GET_COUNT();
    }
}

void draw_char(unsigned char x, unsigned char y, unsigned char ascii)
{
    for(unsigned char i = 0; i < 5; i++){
        for(unsigned char j = 0; j < 8; j++){
            if(ASCII[ascii-32][i] & (1 << j)){
                ssd1306_drawPixel(x+i, y+j, 1);
            }   
        }
    }
}

void draw_string(unsigned char x, unsigned char y, unsigned char* string){
    int i = 0;
    char c = string[i];
    while(c != '\0'){
        draw_char(x+i*5, y, c);
        i++;
        c = string[i];
    }
}
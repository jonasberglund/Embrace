#include <stdio.h>
#include <stdlib.h>
#include "iolib.h"

void send_hug();
void send_heartbeat();
void send_buzz();
void test_schedule();
void debug_schedule();
void init_screen():

char black_image_path[] = "/mnt/data/bin/tangible-test.sh /mnt/data/img/default_400x240/monochrome/black.png";
char fanny_hug_image_path[] = "/mnt/data/bin/tangible-test.sh /mnt/data/img/default_400x240/monochrome/fanny_hug.png";
char fanny_heartbeat_image_path[] = "/mnt/data/bin/tangible-test.sh /mnt/data/img/default_400x240/monochrome/fanny_heartbeat.png";
char fanny_buzz_image_path[] = "/mnt/data/bin/tangible-test.sh /mnt/data/img/default_400x240/monochrome/fanny_buzz.png";

int main(void)
{

        iolib_init();

        iolib_setdir(8,30, DIR_OUT); //Pin 30 on P8 expansion header.
        iolib_setdir(8,39, DIR_OUT); //Pin 39 on P8 expansion header.

        init_screen();

        while(1)
        {
         
                debug_schedule();

                //test_schedule();
                //break;
        }

        iolib_free();

        return(0);
}

void init_screen(){
        //Set image on device
        system(fanny_heartbeat_image_path);
        sleep(2);
        system(fanny_heartbeat_image_path);
        sleep(2);
        system(fanny_buzz_image_path);
        sleep(2);
        system(black_image_path);
}

void test_schedule(){
        int del = 60; // 1 min
        
        sleep(del*10);
        send_buzz();

        sleep(del*14);
        send_heartbeat();

        sleep(del*8);
        send_hug();

        sleep(del*14);
        send_heartbeat();

        sleep(del*20);
        send_hug();
}

void debug_schedule(){
        int del = 2; // 2 sek
        
        sleep(del);
        send_hug();

        sleep(del);
        send_heartbeat();

        sleep(del);
        send_buzz();
}

void send_hug(){
        printf("send hug\n"); //Debug

        system(fanny_hug_image_path); //Set image on device

        //Send signal
        pin_high(8,30); 
        sleep(5); 
        pin_low(8,30); 

        system(black_image_path); //Set black image on device
}

void send_heartbeat(){
        printf("send heartbeat\n");

        //Set image on device
        system(fanny_heartbeat_image_path);

        int heartbeat_pattern[] = {1000, 255, 300, 225, 1000, 225, 300, 225}; 
        int repeat = 4;

        int r=0;
        while(r < repeat){

                int i = 0;
                while(i < 8){
                        usleep(heartbeat_pattern[i] * 1000);
                        i++; 
                        pin_high(8,39);

                        usleep(heartbeat_pattern[i] * 1000);
                        i++;
                        pin_low(8,39);
                }
        r++;   
        }

       
        system(black_image_path); //Set black image on device
}

void send_buzz(){
        printf("send buzz\n");

        //Set image on device
        system(fanny_buzz_image_path);

        int delay = 500;
        int count = 3;
        int i = 0;
        while(i < count){
                pin_high(8,39); 
                iolib_delay_ms(delay); //Half a second
                pin_low(8,39);
                iolib_delay_ms(delay); //Half a second 
                i++;
        }

        system(black_image_path); //Set black image on device
}

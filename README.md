#Embrace
The following is the technical documentetain for the project *Embrace*, developed during the Tangbile Interaction course at the Interaction Design master program at Chalmers University of Technology.

##Hardware
Plastic Logic’s Display Evaluation Kit 
Small Display Evaluation Kit PLDEK_BNE-HBZ1.3 

##Connecting to the beagleboard
Install drivers found on [http://beagleboard.org/getting-started](http://beagleboard.org/getting-started) for your operating system.
###Windows
Follow instructions found in document "User-Guide_Small Display Evaluation Kit_PLDEK_BNE-HBZ1.3_700904.pdf" This document can be found in the documentations partition on the SD-card.
###Mac/Linux
When running Windows it was possible to run the the following command whenever to get to the initial “Angstrom” screen. On Mac however, we only found it possible to connect while booting up the hardware. To do so:

1. Plug in both powers supply and usb-cable.
2. Write “sudo screen /dev/tty.usbserial-TIWFV913B 115200” (Could be TIWFV913A instad, depending on your computer) in the terminal window.

After booting the following message will show. You’ll be able to log in with “root”.

![](https://www.dropbox.com/s/klxnf557nrn0uxz/1.png?dl=1)

##Upload your own pictures
Either you connect to the beaglebone via FTP or you could insert the SD-card into your computer. Whatever you choose to do you should be able to navigate to the “/mnt/data/img/” path. Here is where the “slideshow-’” scripts pick their pictures from. You could create your own folder or just replace the images you want to show. (For your own program its only important to remember the path to where you placed your own images). Here, depending on the screen size of the display you connect the slideshow will choose different folders.
![](https://www.dropbox.com/s/8uhqgpcwd8jdp7n/3.png?dl=1)

##Running slideshow
Located in the path 

	“/mnt/data/bin/” 
	
there is a few shell scripts that can be run directly from the console. These are standard scripts that can start/stop the slideshow. You can run them by just typing the name of the file.

![](https://www.dropbox.com/s/wjifsb7hlkzsu1u/2.png?dl=1)
  
##C code
###Including IOLIB
To run our own program and for using the BeagleBones digital and analog inputs/outputs we used the IOLIB and created our own c program.

To get started:

1. Copy iofunc folder found at [https://github.com/RichardPlunkett/jrunner-beaglebone]()

2. Place the folder in 

		cd "home/root"
	
3. Edit the file test_app.c with

		nano
		
4. Compile the program with 

		make
	
5. Run the program with 
		
		./test_app

###Reading analog value
To find the path to analog input file type:
“find / -iname "ain*"

That will return path to the analog input files, in our case:

	“/sys/devices/platform/omap/tsc/ain*”

Try to see if you could get the value from the pin by running, for example:

	“cat /sys/devices/platform/omap/tsc/ain1” 


> [Note that the path "ain1" maps to the pin "AIN0", the path "ain2" maps to the pin "AIN1", etc.](]http://beaglebone.cameon.net/home/reading-the-analog-inputs-adc)



For reading the file in your program, do like this in c.
				
	FILE* f0 = fopen("/sys/devices/platform/omap/tsc/ain1", "r");

The following code example reads and print the analog input every 5ms:

	//Init
	char value_str[4];
	long int value_int = 0;

	//Read the file from the analog input
	FILE* f0 = fopen("/sys/devices/platform/omap/tsc/ain1", "r");

	while(1){

		//Read analog input file
        fseek(f0, 0, 0);
       	fread(&value_str, 3, 3, f0);
        value_int = strtol(value_str,NULL,0);
        printf("Analog value: %li\n", value_int);
        fflush(stdout);
        rewind(f0);
	 	usleep(5000);	//Every 5ms
	 	
	 }

###Sending digital outputs
The following c code sends digital high and low to pins: 

	iolib_init();

	iolib_setdir(8,26, DIR_OUT); //Pin 26 on P8 expansion header.
	iolib_setdir(8,28, DIR_OUT); //Pin 28 on P8 expansion header.

	pin_high(8,26);
	pin_high(8,28);

    sleep(3); // Stay high for 3 seconds

    pin_low(8,26); 
    pin_low(8,28);

###Running shell script from c code
To run a shell script just call system function with the path to the script as a parameter, like:
	
	system("/mnt/data/bin/slideshow-start.sh");

###Making the c code run when booting beagleboard
Follow this tutorial: [http://www.nunoalves.com/open_source/?p=308](http://www.nunoalves.com/open_source/?p=308)

For disabling the “slideshow-start.sh” to run and override our program, we dubilcated the file and saved the original file as  “slideshow-start-org.sh”. The “slideshow-start.sh” is still there but it does not do anything.

As an addition to the tutorial you could also try to run 
		
		systemctl status myScript.service” 
		
and if successful you should see something like this:

![](https://www.dropbox.com/s/b0tbkbjeeslyzn3/4.png?dl=1)

##Errors
If you run in to this errors, as we did a few times. 

![](https://www.dropbox.com/s/k195miogb94wlev/5.png?dl=1)

	make clean
	
will clean all the binaries in the project folder. After done this you can run 

	make 

again, and everything will be fine.

Another solution to the same problem is first running 

	touch * 
	
in the project folder and then do “make”.

##Backup
It’s always a good idea to do a backup. 

We did so by cloning the SD-card containg all information about the operating system following these steps: [http://computers.tutsplus.com/articles/how-to-clone-raspberry-pi-sd-cards-using-the-command-line-in-os-x--mac-59911](http://computers.tutsplus.com/articles/how-to-clone-raspberry-pi-sd-cards-using-the-command-line-in-os-x--mac-59911)


##Code 
The following code was implemented on the BeagleBoard to run our program:


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

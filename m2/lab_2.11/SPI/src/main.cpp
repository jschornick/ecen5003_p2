/*----------------------------------------------------------------------------
LAB EXERCISE 11.1 - SPI interface
SERIAL COMMUNICATION
 ----------------------------------------
 Interface the LCD display using SPI protocol: display four checks (size 10x10
 pixels) at four corners of the LCD.
 
 Input: None
 Output: LCD display
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "NHD_0216HZ.h"

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
	init_spi();
	init_lcd();
    
	print_lcd("Do you even...");
	set_cursor(0,1);
	print_lcd("mbed, bro?");

	clear_lcd();
	set_cursor(0,0);
	print_lcd("Woah...");

	while(1) {
		for(char i = '0'; i<='9'; i++) {
		  set_cursor(10,0);
		  write_data(i);
		  wait_ms(500);
		}
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************

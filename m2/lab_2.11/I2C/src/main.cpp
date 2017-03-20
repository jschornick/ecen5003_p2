/*----------------------------------------------------------------------------
LAB EXERCISE 11.3 - I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
	Access the virtual temperature sensor via I2C interface, print the current temperature
	to the PC via UART
	
	Input: temperature sensor
	Output: PC
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "pindef.h"

//I2C interface
I2C temp_sensor(I2C_SDA, I2C_SCL);
Serial pc(UART_TX, UART_RX);

//I2C address of temperature sensor DS1631
const int temp_addr = 0x90;

enum cmds { START_CONVERT, READ_TEMP };
char cmd[] = {0x51, 0xAA};
char read_temp[2];

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){

  pc.printf("Hello\n");  
	while(1){
		/*
		Write the Start Convert T command to the sensor
		Write the Read Temperature command to the sensor
		Read the 16-bit temperature data
		*/
		
		temp_sensor.write( temp_addr, &cmd[START_CONVERT], 1 );
    temp_sensor.write( temp_addr, &cmd[READ_TEMP], 1 );
    temp_sensor.read( temp_addr, read_temp, 2 );		
		
		//Convert temperature to Celsius
		float temp = (float((read_temp[0] << 8) | read_temp[1]) / 256);
		
    pc.printf("Temp: %0.2f degrees C\n", temp);
		
	}
}

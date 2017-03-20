/*----------------------------------------------------------------------------
LAB EXERCISE 11.4- SPI and I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
 Display the temperature from the virtual DS1631 temperature sensor on the 
      virtual LCD
 
 Input: virtual DS1631 temperature sensor
 Output: virtual LCD display
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "NHD_0216HZ.h"
#include "DS1631.h"
#include "pindef.h"

//Define the LCD and the temperature sensor

NHD_0216HZ lcd(SPI_CS, SPI_MOSI, SPI_SCLK);
DS1631 temp_sensor(I2C_SDA, I2C_SCL, 0x90);

//Define a variable to store temperature measurement
float temp;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
	//Initialise the LCD
	lcd.init_lcd();
	
	while(1){
		/*
		Read the temperature from the DS1631
		Update the LCD with new temperature measurement
		*/
    temp = temp_sensor.read();
    lcd.set_cursor(0,0);
    lcd.printf("Temp: %0.2f", temp);
		
	}
}

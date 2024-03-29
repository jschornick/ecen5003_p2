/*----------------------------------------------------------------------------
LAB EXERCISE 11.2 - UART interface
SERIAL COMMUNICATION
 ----------------------------------------
	Print text to the PC via UART protocol
	
	Input: None
	Output: PC
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "pindef.h"

// Serial tx, rx connected to the PC via an USB cable
Serial device(UART_TX, UART_RX);

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
  /*
  Set the baudrate to 9600 bps
  Print "Hello mbed" to the PC serial monitor
  */

  device.baud(9600);
  device.printf("Hello mbed\n");
  
}

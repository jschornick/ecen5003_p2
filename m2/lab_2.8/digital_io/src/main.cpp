/*----------------------------------------------------------------------------
LAB EXERCISE 8.1 - DIGITAL INPUTS AND OUTPUTS

Let 
  Button 1 turn LED LD2 on, 
  Button 2 turns it off, 
  Button 3 turns on an external LED in you wireless breadboard, and 
  Button 4 turns the external LED off.

 *----------------------------------------------------------------------------*/

#include "mbed.h"

// Input pin assignments
#define BUTTON1_PIN  PA_10  /* D2 */
#define BUTTON2_PIN  PB_3   /* D3 */
#define BUTTON3_PIN  PB_5   /* D4 */
#define BUTTON4_PIN  PB_4   /* D5 */

// Output pin assignments
#define LD2_PIN      PA_5   /* D13 */
#define EXT_LED_PIN  PB_10  /* D6 */

// Button states (all pulled up to 3.3V)
#define BUTTON_DOWN  0
#define BUTTON_UP    1

// LED states (all direct driven)
#define LED_ON   1
#define LED_OFF  0

// Input bus selectors
#define BUTTON1  0x1
#define BUTTON2  0x2
#define BUTTON3  0x4
#define BUTTON4  0x8

// Output bus selectors
#define LED_LD2  0x1
#define LED_EXT  0x2

// Define input bus
BusIn input_bus(BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN);

// Define output bus for the LEDs
BusOut output_bus(LD2_PIN, EXT_LED_PIN);



/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){

  printf("Hello\n");
  int inputs;  // value from input bus
  int outputs; // value for output bus
  
	while(1){

    //Check which button was pressed and light LEDs accordingly
    //printf( "Bus: %u\n", input_bus.read());
    inputs = input_bus.read(); 
    if((inputs & BUTTON1) == BUTTON_DOWN) {
      printf("Button 1\n");
      SET_BIT(outputs, LED_LD2);
    }
    if((inputs & BUTTON2) == BUTTON_DOWN) {
      printf("Button 2\n");
      CLEAR_BIT(outputs, LED_LD2);
    }
    if((inputs & BUTTON3) == BUTTON_DOWN) {
      printf("Button 3\n");
      SET_BIT(outputs, LED_EXT);
    }
    if((inputs & BUTTON4) == BUTTON_DOWN) {
      printf("Button 4\n");
      CLEAR_BIT(outputs, LED_EXT); 
    }
    output_bus = outputs;

	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************

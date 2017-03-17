/*----------------------------------------------------------------------------
LAB EXERCISE 8.2 - INTERRUPT IN/OUT
PROGRAMMING USING MBED API
 ----------------------------------------
 In this exercise you need to use the mbed API functions to:

	1) Define InterruptIn and ISR for the button press
	2) In the interrupt service routines,
      Button 1 turn LED LD2 on,
      Button 2 turns it off,
      Button 3 turns on an external LED in you wireless breadboard, and
      Button 4 turns the external LED off.
	3) Put the processor into sleep mode upon exiting from the ISR
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


// Define outputs
DigitalOut ld2(LD2_PIN);
DigitalOut ext_led(EXT_LED_PIN);

// Define inputs
InterruptIn button1(BUTTON1_PIN);
InterruptIn button2(BUTTON2_PIN);
InterruptIn button3(BUTTON3_PIN);
InterruptIn button4(BUTTON4_PIN);

//Define ISRs for the interrupts
void button1_handler() {
  //printf("Button 1\n");
  ld2 = LED_ON;
}

void button2_handler(){
  //printf("Button 2\n");
  ld2 = LED_OFF;
}

void button3_handler(){
  //printf("Button 3\n");
  ext_led = LED_ON;
}

void button4_handler(){
  //printf("Button 4\n");
  ext_led = LED_OFF;
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){

  printf("Hello\n");

	//initially turn off LEDs
  ld2 = LED_OFF;
  ext_led = LED_OFF;

	//Attach the address of the ISR to the rising edge
  button1.rise(&button1_handler);
  button2.rise(&button2_handler);
  button3.rise(&button3_handler);
  button4.rise(&button4_handler);

	while(1) {
    printf("Going to sleep...\n");
    //__wfi();
    sleep(); // __wfi() alone won't stay asleep, use mbed sleep API instead
	}
}

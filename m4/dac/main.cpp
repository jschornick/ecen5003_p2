/************************
ECEN5003 Project 2
Module 4 : DAC

Write a program either using the mbed compiler, or in the Keil MDK, to 
generate a 1004 Hz tone through the DAC. 

Please note if there is no DAC module present, a software DAC function would
also work and store the samples generated into a .csv or .txt file. The
software DAC function can be based on table lookup, digital oscillator, or 
your own code to create the file.

Bonus Points: take your DAC file and drive a PWM output with RC filter on the
breadboard, and view your tone with an oscilloscope.

**************************/

#include "mbed.h"
#include "math.h"

#define SAMPLE_RATE 10000  /* update the DAC at 10 kHz */
#define SINE_FREQ   1004   /* generate a 1004 Hz sine wave */
#define SINE_TABLE_STEPS 1000   /* resolution of precomputed sine lookup table */

// Use a lower frequency when testing simple low-pass filter hardware
//#define SINE_FREQ   10

#define RECORD_SIZE 3000  /* how many DAC readings to store for text output */

#define DAC_PIN  PB_10  /* D6 */

#define M_PI  3.14159265358979323846

// A simulated DAC (TI DAC8581)
//  16-bit
//  2's compliment 
short int dac = 0;

Ticker dac_updater;

PwmOut dac_out(DAC_PIN);
PwmOut led(LED1);

Serial serial(USBTX, USBRX);

short sine_table[SINE_TABLE_STEPS];
short dac_history[RECORD_SIZE];

unsigned int count = 0;
unsigned int step;   // how quickly to advance phase
unsigned int substep;
unsigned int phase = 0;  // phase within the sine wave [0, SINE_TABLE_STEPS)
unsigned int subpos = 0;  // subphase within sine wave [0, SAMPLE_RATE)

// create a 16-bit sine wave lookup table
void init_sine_table() {
  float step = (2 * M_PI) / SINE_TABLE_STEPS; 
  float x = 0.0;
  for(int i=0; i<SINE_TABLE_STEPS; i++) {
    sine_table[i] = sin(x) * ((1<<15)-1);  // scale [-1.0,1.0] to [-(2^15-1), 2^15-1]
    x += step;
    //printf("Table %03d : %+6d\n", i, sine_table[i]);
  }
}

// Updates the DAC to the correct value for the next sample
void dac_update () {
  phase += step;
  subpos += substep;
  if (subpos >= SAMPLE_RATE) {
    subpos -= SAMPLE_RATE;
    phase += 1;
  }
  phase %= SINE_TABLE_STEPS;
  // If we wanted to be really precise, we'd interpolate using the subposition.
  // Instead, just read the table directly
  dac = sine_table[phase];
  //printf("Sample: %04d, DAC: %+6d\n", count, dac);
  if(count < RECORD_SIZE) {
    dac_history[count] = dac;
  }
  dac_out = ((float) dac + (1<<15))/(1<<16);  // map to duty cycle [0,1]
  led = dac_out;
  count++;
}


int main() {
  
  // increase serial port speed so logging is fast
  serial.baud(57600);
  
  //led.period(1.0/ (SAMPLE_RATE*10));
  //dac_out.period(1.0/ (SAMPLE_RATE*10));
  led.period(0.00002);    // pwm period needs to be small for low pass filter
  dac_out.period(0.00002);

  
  step = (SINE_TABLE_STEPS * SINE_FREQ) / SAMPLE_RATE;
  substep = (SINE_TABLE_STEPS * SINE_FREQ) % SAMPLE_RATE;
  //printf("Step size: %d, Substep size: %d\n", step, substep);
  
  init_sine_table();
  dac_updater.attach(dac_update, 1.0/SAMPLE_RATE);
  // wait for our recorder to fill up with samples
  wait(RECORD_SIZE /SAMPLE_RATE);
  //dac_updater.detach();
  
  for(int i=0; i<RECORD_SIZE; i++) {
    //printf("%0.5f %+6d %04hx\n", i *(1.0/SAMPLE_RATE), dac_history[i], dac_history[i]);    
    printf("%0.5f %04hx\n", i *(1.0/SAMPLE_RATE), dac_history[i]);
  }
  
  while(1) {
  }
}

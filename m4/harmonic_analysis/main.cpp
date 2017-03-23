/*********************************
ECEN5003 Project 2
Module 4 : Harmonic Analysis

Write a program either using the mbed compiler, or in the Keil MDK, to read
a 1004 Hz tone from a file into the ADC buffer and perform a harmonic 
analysis.  Use the samples from your .csv or .txt file from the previous
question for the input to the analyzer. (NOTE: we prefer you do not use arrays
to input values from your csv file to your program. Better to create a lookup
table in the debug tool, or use a serial port transfer program to read the
file data into your target program). 
**********************************/

#include "mbed.h"

#define M_PI 3.14159265358979323846f

#define MAX_SAMPLES 4000

Serial serial(USBTX, USBRX);
DigitalOut led(LED1);

volatile unsigned int count = 0;
volatile char done_reading = 0;
float adc_t[MAX_SAMPLES];
short adc_val[MAX_SAMPLES];

// Read from serial port on interrupt
void reader(void) {
  static char buf[100];
  static unsigned short buf_pos = 0;
  char c;
  float t;
  short val;
  while(serial.readable()) {
    c = serial.getc();
    //serial.putc(c);
    buf[buf_pos++] = c;
    if( (c == '\n') || (c == '\r') ) {
      buf[buf_pos-1] = '\0';
      //printf("Got: '%s'\n", buf);
      sscanf(buf, "%f %hx", &t, &val);
      if( (t>=0) && (count < MAX_SAMPLES) ) {
        adc_t[count] = t;      
        adc_val[count] = val;
        count++;
      } else {
        done_reading = 1;
      }
      //printf("%0.5f %04hx\n", t, val);
      buf_pos = 0;
    }
  }
}

// Perform DFT analysis on ADC samples
int analyze(int n) {
  float max_mag2 = 0.0;
  int max_pos;
  float mag2;
  for (int k = 0; k < (n/2)+1; k++) {  // only need to calculate 1st half of spectrum
		float sumreal = 0;
		float sumimag = 0;
		for (int t = 0; t < n; t++) {
			float angle = 2 * M_PI * t * k / n;
      sumreal +=  adc_val[t] * cos(angle);  // signal is real valued, no imag components in input
			sumimag += -adc_val[t] * sin(angle);  // but resulting dft is complex
		}
    //dft_r[k] = sumreal;
    //dft_i[k] = sumimag;
    mag2 = sumreal * sumreal + sumimag * sumimag;  // squared magnitude
    if(mag2 > max_mag2) {
      max_mag2 = mag2;
      max_pos = k;
    }
    led = !led;
	}
  return max_pos;
}

int main() {
  serial.baud(57600);
  printf("* Reset *\n");
  //printf("Waiting for input file...\n");
  serial.attach(reader, Serial::RxIrq);
  
  while(!done_reading) {
    wait_ms(200);
    led = !led;
  }
  printf("Loaded %d ADC samples\n", count);

  int fund_pos;
  fund_pos = analyze(count);
  float sample_rate = 1.0f/(adc_t[1] - adc_t[0]);
  float freq_step = sample_rate / count;
  //printf( "Fund pos: %d\n", fund_pos);
  printf("Fundamental: %0.2f\n", freq_step * fund_pos);
  led = 0;
}

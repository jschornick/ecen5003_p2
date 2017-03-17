/*----------------------------------------------------------------------------
LAB EXERCISE 9 - Analog input and PWM
 ----------------------------------------
	Use two potentiometers to adjust the volume and pitch of the output sound wave.

 *----------------------------------------------------------------------------*/

#include "mbed.h"

#define SPEAKER_PIN  PB_10  /* D6 */
#define POT1_PIN     PA_0   /* A0 */
#define POT2_PIN     PA_1   /* A1 */

#define MIN_FREQ  320   /* 320Hz, 3125us period */
#define MAX_FREQ  8000  /* 8000Hz, 125us period */
#define MIN_POT  0.11f
#define MAX_POT  1.0f

#define TICKER_SECS  0.00002f  /* 20 us, 50kHz  */
#define ADC_SAMPLES 100

PwmOut speaker(SPEAKER_PIN);
AnalogIn volume(POT1_PIN);
AnalogIn frequency(POT2_PIN);
Serial serial(USBTX, USBRX);
Ticker speakerTick;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

float vol = 0.5;
float freq = 0.5;

float i = 0.0;
float step = 0.0;

void speaker_handler(void) {
  i += step;
  if ( i>1.0f ) {
    i -= 1.0f;
  }
  speaker = i * vol;
}

int main(){

  serial.printf("Hello\n");

  float freq_adc = 0.0;
  float vol_adc = 0.0;

  // Coefficients to linearly scale ADC to frequency
  float freq_scaler = (MAX_FREQ - MIN_FREQ) / (MAX_POT - MIN_POT);
  float freq_offset = MIN_FREQ - freq_scaler * MIN_POT;

  speaker.period_us(5);  // keep period small since we're rapidly chaning duty cycles
  speakerTick.attach(&speaker_handler, TICKER_SECS);  // update the duty cycle every TICKER_SECS

	while(1){
		/*
		Print values of the potentiometers to the PC serial terminal
		Create a saw-tooth sound wave
		Make the period and volume adjustable using the potentiometers
		*/

    // average 100 ADC samples @ ~1khz to get an average frequency update every 100ms
    freq_adc = 0.0;
    vol_adc = 0.0;
    for(int j=0; j<ADC_SAMPLES; j++) {
      freq_adc += frequency.read();
      vol_adc += volume.read();
      wait_us(1000);
    }
    freq_adc /= ADC_SAMPLES;
    vol_adc /= ADC_SAMPLES;

    // Linearly scale volume and frequency based on ADC readings
    vol = (vol_adc - MIN_POT) / (MAX_POT - MIN_POT);
    freq = freq_adc * freq_scaler + freq_offset;

    // recalculate size of sawtooth wave update based on target frequency and update period
    step = freq * TICKER_SECS;

    serial.printf("Vol: %f = %0.1f%%, Freq: %f = %0.1f Hz, Step: %f\n", vol_adc, vol*100, freq_adc, freq, step);

	}
}

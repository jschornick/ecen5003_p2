/*----------------------------------------------------------------------------
LAB EXERCISE 12 - Real-Time Operating System
 ----------------------------------------
	Integrate functions developed in previous modules and run then concurrently
	in the mbed RTOS. The following four threads have to be implemented:
	
	1. Display the temperature on the LCD and to the PC
	2. Adjust the brightness of the RGB LED using a potentiometer
	3. Display an incrementing counter on the LCD and to the PC
	4. Blink an LED
  
  NOTE:  For this lab, the LCD, temp sensor and POT are virtual devices.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "rtos.h"
#include "DS1631.h"
#include "NHD_0216HZ.h"
#include "pindef.h"

/*
Define the mutex
Define the LCD display and the temperature sensor
Define other inputs and outputs
*/

#define POT_PIN      PA_0   /* A0 */
#define EXT_LED_PIN  PB_10  /* D6 */

#define POT_MIN  0.095f
#define POT_MAX  1.0f

//enum led_status { LED_ON, LED_OFF };

AnalogIn brightness_pot(POT_PIN);
PwmOut ext_led(EXT_LED_PIN);

NHD_0216HZ lcd(SPI_CS, SPI_MOSI, SPI_SCLK);
DS1631 temp_sensor(I2C_SDA, I2C_SCL, 0x90);

osMutexId lcd_mutex;
osMutexDef(lcd_mutex);

volatile float brightness = 0.5;
volatile unsigned int count = 0;

//Display temperature on the LCD
void temp_thread(void const *args){
	
  float temp;
  
  while(1) {
    temp = temp_sensor.read();
    osMutexWait(lcd_mutex, osWaitForever);
    lcd.set_cursor(0,1);
    lcd.printf("Temp: %0.2f", temp);
    osMutexRelease(lcd_mutex);
    osDelay(100);
  }
}

//Adjust the brightness of the RGB LED
void adjust_brightness(void const *args){	
  while(1) {
  	brightness = (brightness_pot.read() - POT_MIN) / (POT_MAX - POT_MIN);
    osDelay(100);
  }
}

//Blink an LED
void led1_thread(void const *args) {
  
  while(1) {
  
    ext_led = brightness;
    osDelay(500);
    
    ext_led = 0.0;
    osDelay(500);
	}
  
}

//Display a counter on the LCD
void count_thread(void const *args){

  while(1) { 
    osMutexWait(lcd_mutex, osWaitForever);
    lcd.set_cursor(0,0);
    lcd.printf("Count: %04d", count);
    osMutexRelease(lcd_mutex);
    count++;
    osDelay(1000);
  }

}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

//Serial serial(UART_TX, UART_RX);

// Use RTX-style threading
osThreadDef(led1_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(adjust_brightness, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(count_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(temp_thread, osPriorityNormal, DEFAULT_STACK_SIZE);

int main(){
	/*
	Initialise and clear the LCD display
	Start all threads
	Wait for timer interrupt
	*/
  
  //serial.printf("* Reset *\n");
	lcd.init_lcd();
  lcd.clr_lcd();
  lcd.printf("* Reset *");
    
  //Thread thread1(temp_thread);
  //Thread thread2(adjust_brightness);
  //Thread thread3(led1_thread);
  //Thread thread4(count_thread);
  
  // Create RTX threads
  osThreadCreate(osThread(led1_thread), NULL);
  osThreadCreate(osThread(adjust_brightness), NULL);
  osThreadCreate(osThread(count_thread), NULL);
  osThreadCreate(osThread(temp_thread), NULL); 
  
  while(1) {
    //serial.printf("Count: %04d, Brightness %0.3f\n", count, brightness);
    // Whenever the main thread is scheduled, simply wait for interrupt
    __WFI();
  }
	
}

/*----------------------------------------------------------------------------
 Newhaven NHD0216HZ LCD C/C++ file
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "NHD_0216HZ.h"
#include "pindef.h"

DigitalOut SS(SPI_CS);     //slave select a.k.a. cs or latch for shift reg
SPI spi(SPI_MOSI, NC, SPI_SCLK);

//Initialise SPI
void init_spi(void) {
    SS = 1;
    
    spi.format(8, 3);         //8bit spi mode 2
    spi.frequency(100000);    //100 kHz spi clock
}


// ST7066U instruction register

#define I_CLEAR 0x01

#define I_FUNC       0x20
#define I_FUNC_4BIT  0x00
#define I_FUNC_8BIT  0x10
#define I_FUNC_1LINE 0x00
#define I_FUNC_2LINE 0x08
#define I_FUNC_5X8   0x00
#define I_FUNC_5X11  0x04

#define I_DISP          0x08
#define I_DISP_ON       0x04
#define I_DISP_CURS_ON  0x02
#define I_DISP_CURS_POS 0x01

#define I_ENTRY  0x04
#define I_ENTRY_DEC 0x00
#define I_ENTRY_INC 0x02
#define I_ENTRY_SHIFT 0x01

#define I_SHIFT         0x10
#define I_SHIFT_CURSOR  0x00
#define I_SHIFT_DISPLAY 0x80
#define I_SHIFT_LEFT    0x00
#define I_SHIFT_RIGHT   0x04

// Shift register

// Q_C (P2) = RS
// Q_D (P3) = E
// Q_E (P4) = DB4 = LSB of data nibble
// Q_F (P5) = DB5
// Q_G (P6) = DB6
// Q_H (P7) = DB7 = MSB of data nibble

//Initialise LCD
void init_lcd(void) {
    /*
	Implement the LCD initialisation using the information 
	from the ST7066U LCD driver datasheet (pages 25-26)
	*/
	
  wait_ms(100);  // Wait >40ms after power on
  write_cmd(I_FUNC | I_FUNC_8BIT); // wakeup
  wait_ms(10);  // Wait >37us
  write_cmd(I_FUNC | I_FUNC_8BIT); // wakeup
  wait_ms(10);  // Wait >37us
  write_cmd(I_FUNC | I_FUNC_8BIT); // wakeup
  wait_ms(10);  // Wait >37us
	
  write_cmd(I_FUNC | I_FUNC_4BIT); // enable 4-bit
  wait_ms(10);
  write_cmd(I_FUNC | I_FUNC_4BIT | I_FUNC_2LINE | I_FUNC_5X8); // 4-bit, 2-line
  wait_ms(10);
  
  write_cmd(I_SHIFT | I_SHIFT_CURSOR | I_SHIFT_LEFT);
  write_cmd(I_DISP | I_DISP_ON | I_DISP_CURS_ON | I_DISP_CURS_POS); // display on
  write_cmd(I_ENTRY | I_ENTRY_INC);  // increment on write
  wait_ms(10);  

  
 /*
  wait_ms(100);  // Wait >40ms after power on
  write_cmd(0x30); // wakeup
  wait_ms(10);  //
  write_cmd(0x30); // wakeup
  wait_ms(10);  //
  write_cmd(0x30); // wakeup
  wait_ms(10);  //
  write_cmd(0x20); //4-bit
  write_cmd(0x28);  //4-bit /2 line
  write_cmd(0x10);  // cursor
  write_cmd(0x0f);  // display on / blink
  write_cmd(0x06);  // entry mode
  wait_ms(10);
  */
}

//Write 4bits to the LCD
void write_4bit(int nibble, int mode) {
    SS = 0;
    spi.write(nibble | ENABLE | mode);
    SS = 1;
    wait_us(1);
    SS = 0;
    spi.write(nibble & ~ENABLE);
    SS = 1;
}

//Write a command to the LCD
void write_cmd(int data) {
    int hi_n;
    int lo_n;
    
    hi_n = hi_n = (data & 0xF0);
    lo_n = ((data << 4) &0xF0);
    
    write_4bit(hi_n, COMMAND_MODE);
    write_4bit(lo_n, COMMAND_MODE);
}

//Write data to the LCD
void write_data(char c) {
    int hi_n;
    int lo_n;
    
    hi_n = hi_n = (c & 0xF0);
    lo_n = ((c << 4) &0xF0);
    
    write_4bit(hi_n, DATA_MODE);
    write_4bit(lo_n, DATA_MODE);
}

//Set cursor position
void set_cursor(int column, int row) {
    int addr;
    
    addr = (row * LINE_LENGTH) + column;
    addr |= TOT_LENGTH;
    write_cmd(addr);
}

//Print strings to the LCD
void print_lcd(const char *string) {
    while(*string){
        write_data(*string++);
    }
}

void clear_lcd() {
  write_cmd(I_CLEAR);
  wait_ms(2);  // wait > 1.52ms
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************

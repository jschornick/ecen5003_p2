/*----------------------------------------------------------------------------
 Newhaven NHD0216HZ LCD library
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include <stdarg.h>
#include "NHD_0216HZ.h"

NHD_0216HZ::NHD_0216HZ(PinName SPI_CS, PinName SPI_MOSI, PinName SPI_SCLK) : _SPI_CS(SPI_CS), _SPI_MOSI(SPI_MOSI), _SPI_SCLK(SPI_SCLK){
		_SPI_CS = 0;
}
		
void NHD_0216HZ::shift_out(int data) {
    _SPI_CS = 0;
    for(int i=0; i<8; i++){
        _SPI_MOSI = (data & (0x80 >> i));
        _SPI_SCLK = 0;
        wait_us(1);
        _SPI_SCLK = 1;
    }
    _SPI_MOSI = 0;
    _SPI_CS = 1;
}

void NHD_0216HZ::init_lcd(void) {
    wait_ms(40);
    shift_out(0x30);    //function set 8-bit
    wait_ms(10);
    write_cmd(0x30);    //function set
    wait_ms(10);
    write_cmd(0x30);    //function set
    wait_ms(10);
    write_cmd(0x20);    //function set 4-bit
    wait_ms(10);
    write_cmd(0x28);    //function set 4-bit, 2-line
    wait_ms(10);
    write_cmd(0x0C);    //display ON, no cursor?
    wait_ms(10);
    write_cmd(0x06);    //entry-mode set
    wait_ms(10);
    write_cmd(0x01);    //display clear
    wait_us(1520);
    set_cursor(0,0);
}

void NHD_0216HZ::write_4bit(int data, int mode) {
    int hi_n;
    int lo_n;
    
    hi_n = hi_n = (data & 0xF0);
    lo_n = ((data << 4) &0xF0);

    shift_out(hi_n | ENABLE | mode);
    wait_us(1);
    shift_out(hi_n & ~ENABLE);
    shift_out(lo_n | ENABLE | mode);
    wait_us(1);
    shift_out(lo_n & ~ENABLE);
}

void NHD_0216HZ::write_cmd(int cmd) {
    write_4bit(cmd, COMMAND_MODE);
}

void NHD_0216HZ::write_data(char c) {
    write_4bit(c, DATA_MODE);
}

void NHD_0216HZ::printf(const char *format, ...) {
		char arr[16];
		sprintf(arr, "int %d, float %f", 4, 6.4);
    va_list v;
    char buffer[16];
    va_start(v, format);
    vsprintf(buffer, format, v);
    char *b = buffer;
    for(int i=0; i<16 && *b; i++) {
        write_data(*b++);
    }
}

void NHD_0216HZ::set_cursor(int column, int row) {
    int addr;
    
    addr = (row * LINE_LENGTH) + column;
    addr |= TOT_LENGTH;
    write_cmd(addr);
}

void NHD_0216HZ::clr_lcd(void) {
    write_cmd(0x01);    //display clear
    wait_us(1520);
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************

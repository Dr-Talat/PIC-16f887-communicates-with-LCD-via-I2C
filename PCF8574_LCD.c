#include <16f887.h> 
#fuses hs, put, noprotect
#use delay(clock=20MHz)
#USE   I2C(MASTER,SLOW,SDA=PIN_C4,SCL=PIN_C3)

#include "PCF8574.c" 
void main()
{ 
 i2c_lcd_setup(0x40);
 i2c_lcd_command(0x85); 
 i2c_lcd_data("welcome");
 i2c_lcd_goto_xy(1,2); 
 i2c_lcd_data("I2C interface");
 
 while(true);
}

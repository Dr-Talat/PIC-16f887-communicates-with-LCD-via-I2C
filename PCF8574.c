//!#define pcf8574_address    0x40
//!
//!void send_data()
//!{
//!   i2c_start();
//!   i2c_write(pcf8574_address);
//!   i2c_write(....);
//!   i2c_stop();
//!}

//!#ifndef lcd_rs                   //if not define
//!#define lcd_rs       0b00000001
//!#endif                           //Marks the end of an IF command sequence
//!#ifndef lcd_rw 
//!#define lcd_rw       0b00000010
//!#endif
//!#ifndef lcd_e
//!#define lcd_e        0b00000100
//!#endif

#define lcd_rs       0b00000001
#define lcd_rw       0b00000010
#define lcd_e        0b00000100
#define function_set       0x28
#define clear_display      0x01
#define entry_mode_set     0x06
#define display_control    0x0c

//!========================================================
unsigned int8 pcf8574_address;   // 0x40


void lcd_send_4bit(unsigned int8 x, int1 value)
{
   unsigned int8 out_pcf=x<<4;
   switch (value)
   {
      case 0: //thanh ghi lenh IR
         i2c_write(0x00); delay_us(20);
         i2c_write(out_pcf|lcd_e); delay_us(20);
         i2c_write(out_pcf); delay_ms(2);
         break;
      case 1:  //thanh ghi lenh DR
         i2c_write(lcd_rs); delay_us(20);
         i2c_write(out_pcf|lcd_rs|lcd_e); delay_us(20);
         i2c_write(out_pcf|lcd_rs); delay_ms(2);
         break;
   }  
}


void lcd_send_data(unsigned int8 x, int1 value)
{
   unsigned int8 MSB, LSB;
   MSB=((x>>4)&0x0f);
   LSB=x&0x0f;
   lcd_send_4bit(MSB,value);
   lcd_send_4bit(LSB,value);
}

void lcd_command(unsigned int8 loc)
{
//!   output_low(lcd_rs);
   lcd_send_data(loc,0);
}

void lcd_data(unsigned int8 value)
{
//!   output_high(lcd_rs);
   lcd_send_data(value,1);
}

void lcd_goto_xy(unsigned int8 x, unsigned int8 y)
{
   const unsigned int8 location[]={0x80,0xc0};
   lcd_send_data(location[x]+y,0);
}

//!============================================================================
void i2c_lcd_setup(unsigned int8 pcf8574_add)
{
   pcf8574_address=pcf8574_add;
   i2c_start();
   i2c_write(pcf8574_address);
   i2c_write(0x00);
   delay_ms(15);   // wait for more than 15ms (Vcc=4.5V) and 40ms (Vcc=2.7V)

//!set interface to be 8-bit 3 times and BF cannot be checked before this instruction
   for(int i=1;i<=3;i++)         
   {
      lcd_send_4bit(0x03,0);    
      delay_ms(5);
   }                   
                    

   lcd_send_4bit(0x02,0); //Set interface to be 4 bits and BF can be checked

   lcd_send_data(function_set,0);
   lcd_send_data(display_control,0);
//!   lcd_send_data(clear_display,0);   delay_ms(2);   
   lcd_send_data(entry_mode_set,0);
   i2c_stop();
}


void i2c_lcd_data(unsigned int8 x)
{
   i2c_start();
   i2c_write(pcf8574_address);
   lcd_data(x);
   i2c_stop();
}

void i2c_lcd_goto_xy(unsigned int8 x, unsigned int8 y)
{
   i2c_start();
   i2c_write(pcf8574_address);
   lcd_goto_xy(x,y);
   i2c_stop();
}

void i2c_lcd_command(unsigned int8 x)
{
   i2c_start();
   i2c_write(pcf8574_address);
   lcd_command(x);
   i2c_stop();
}

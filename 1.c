#include<REG_MPC82G516.H>

#define uchar unsigned char
#define uint unsigned int

sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;
#define LCD_DATA P1
sbit LCD_BF = LCD_DATA^7;

uchar sec, min, hr;		//second, minute, hour

bit lcd_busy();									//check if LCD is busy
void lcd_wcmd(uchar cmd);						//function to write command
void lcd_wdat(uchar dat);						//function to write data
void lcd_pos(uchar pos);						//location to show
void lcd_init();								//initial LCD
void print_time(void);							//show time!
void delay(int);
void delay_us(int);

/*******************************main program*************************************/
void main()
{
	lcd_init();
	sec=10; min=10; hr=10;	   //initial start time
	while(1)
		print_time(); //print time
}

void print_time(void)
{

	lcd_pos(0x4a);
	lcd_wdat(':');
	lcd_pos(0x4d);
	lcd_wdat(':');

	lcd_pos(0x48);
	lcd_wdat((hr/10)+48);// to_char
	lcd_wdat((hr%10)+48);

	lcd_pos(0x4b);
	lcd_wdat((min/10)+48);
	lcd_wdat((min%10)+48);

	lcd_pos(0x4e);
	lcd_wdat((sec/10)+48);
	lcd_wdat((sec%10)+48);

}

/*******************************************************************/
/*
/* delay function
/*
/*******************************************************************/

void delay(int ms)
{
	int i;
	while(ms--)
		for(i = 0; i< 1000; i++);
}

void delay_us(int us)
{
	while(us--);
}


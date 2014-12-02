#include<REG_MPC82G516.H>

#define uchar unsigned char
#define uint unsigned int

// specified pins, you can use other port
sbit LCD_RS = P1^5;
sbit LCD_RW = P1^6;
sbit LCD_EN = P1^7;
#define LCD_DATA P2
sbit LCD_BF = LCD_DATA^7;

bit lcd_busy();									//check if LCD is busy
void lcd_wcmd(uchar cmd);						//function to write command
void lcd_wdat(uchar dat);						//function to write data
void lcd_pos(uchar pos);						//location to show
void lcd_cursor(bit on_off);
void lcd_init();								//initial LCD
void delay(int);
void delay_us(int);


/*******************************************************************/
/*
/*check if LCD is busy
/*lcd_busy為1時，忙!等待。lcd-busy為0時, 有空了，可以寫指令或資料。
/*
/*******************************************************************/

bit lcd_busy() {
	bit ret;
	
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	delay_us(10);
	
	ret = LCD_BF;
	
	LCD_EN = 0;
	delay_us(60); // Need 40us
	
	return ret;
}

/*******************************************************************/
/*
/*write command to LCD
/*RS=L，RW=L，E=高脈波，D0-D7=command code。
/*
/*******************************************************************/

void lcd_wcmd(uchar cmd) {
	while(lcd_busy());
	delay_us(10);

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_DATA = cmd;
	LCD_EN = 1;
	delay_us(10);

	LCD_EN = 0;
	if(cmd < 4)
		delay(2); // Need 1.64ms
	else
		delay_us(60); // Need 40us
}

/*******************************************************************/
/*
/*write data to LCD
/*RS=H，RW=L，E=高脈波，D0-D7=data。
/*
/*******************************************************************/

void lcd_wdat(uchar dat) {
	while(lcd_busy());
	delay_us(10);
	
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_DATA = dat;
	LCD_EN = 1;
	delay_us(10);
	
	LCD_EN = 0;
	delay_us(60); // Need 40us
}

/*******************************************************************/
/*
/*	set the location to show
/*
/*******************************************************************/

void lcd_pos(uchar pos) {
	lcd_wcmd(pos|0x80);		//資料指針=80+位址變量
}

/*******************************************************************/
/*
/*	LCD initial
/*
/*******************************************************************/

void lcd_cursor(bit on_off) {
	lcd_wcmd(on_off ? 0x0d : 0x0c);
}

void lcd_init() {
	delay(15);				 //等待LCD電源穩定
	lcd_wcmd(0x38);			 //16*2顯示，5*7點陣，8位資料
	delay(15);				 //等待LCD電源穩定
	lcd_wcmd(0x38);			 //16*2顯示，5*7點陣，8位資料
	delay(15);				 //等待LCD電源穩定
	lcd_wcmd(0x38);			 //16*2顯示，5*7點陣，8位資料
	delay(5);
	
	lcd_wcmd(0x0c);			 //顯示開，關游標
	delay(5);
	lcd_wcmd(0x06);			 //移動游標
	delay(5);
	lcd_wcmd(0x01);			 //清除LCD的顯示內容
	delay(5);

}

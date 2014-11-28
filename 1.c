#include<REG_MPC82G516.H>

#define uchar unsigned char
#define uint unsigned int

// specified pins, you can use other port
sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

uchar sec, min, hr;		//second, minute, hour

bit lcd_busy();									//check if LCD is busy
void lcd_wcmd(uchar cmd);						//function to write command
void lcd_wdat(uchar dat);						//function to write data
void lcd_pos(uchar pos);						//location to show
void lcd_init();								//initial LCD
void print_time(void);							//show time!

/*******************************main program*************************************/
void main()
{
	lcd_init();
	sec=10; min=10; hr=10;     //initial start time
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

/*******************************************************************/
/*
/*check if LCD is busy
/*lcd_busy為1時，忙!等待。lcd-busy為0時, 有空了，可以寫指令或資料。
/*
/*******************************************************************/

bit lcd_busy()
{
	/* your code */
}

/*******************************************************************/
/*
/*write command to LCD
/*RS=L，RW=L，E=高脈波，D0-D7=command code。
/*
/*******************************************************************/

void lcd_wcmd(uchar cmd)
{
   while(lcd_busy());
    /* your code  */
}

/*******************************************************************/
/*
/*write data to LCD
/*RS=H，RW=L，E=高脈波，D0-D7=data。
/*
/*******************************************************************/

void lcd_wdat(uchar dat)
{
   while(lcd_busy());
    /* your code  */
}

/*******************************************************************/
/*
/*  set the location to show
/*
/*******************************************************************/

void lcd_pos(uchar pos)
{
   lcd_wcmd(pos|0x80);      //資料指針=80+位址變量
}

/*******************************************************************/
/*
/*  LCD initial
/*
/*******************************************************************/

void lcd_init()
{
    delay(15);               //等待LCD電源穩定
    lcd_wcmd(0x38);          //16*2顯示，5*7點陣，8位資料
    delay(5);
    lcd_wcmd(0x38);
    delay(5);
    lcd_wcmd(0x38);
    delay(5);
 /*
    lcd_wcmd(0x0c);          //顯示開，關游標
    delay(5);
    lcd_wcmd(0x06);          //移動游標
    delay(5);
    lcd_wcmd(0x01);          //清除LCD的顯示內容
    delay(5);
*/

}

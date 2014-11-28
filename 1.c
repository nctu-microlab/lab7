#include<REG_MPC82G516.H>

#define uchar unsigned char
#define uint unsigned int

uchar sec, min, hr;		//second, minute, hour
bit redisplay_sec, redisplay_min, redisplay_hr, redisplay_colon;

bit lcd_busy();									//check if LCD is busy
void lcd_wcmd(uchar cmd);						//function to write command
void lcd_wdat(uchar dat);						//function to write data
void lcd_pos(uchar pos);						//location to show
void lcd_init();								//initial LCD
void print_time(void);							//show time!
void delay(int);
void delay_us(int);
void timer0_init();

/*******************************main program*************************************/
void main() {
	sec=10; min=10; hr=10;	   //initial start time;
	redisplay_hr = redisplay_min = redisplay_sec = redisplay_colon = 1;
	
	IE = TMOD = 0;
	lcd_init();
	timer0_init();
	
	while(1)
		print_time(); //print time
}

void print_time(void) {
	if(redisplay_colon) {
		lcd_pos(0x4a);
		lcd_wdat(':');
		lcd_pos(0x4d);
		lcd_wdat(':');
		
		redisplay_colon = 0;
	}

	if(redisplay_hr) {
		lcd_pos(0x48);
		lcd_wdat(hr/10 + '0');// to_char
		lcd_wdat(hr%10 + '0');
		
		redisplay_hr = 0;
	}

	if(redisplay_min) {
		lcd_pos(0x4b);
		lcd_wdat(min/10 + '0');
		lcd_wdat(min%10 + '0');
		
		redisplay_min = 0;
	}

	if(redisplay_sec) {
		lcd_pos(0x4e);
		lcd_wdat(sec/10 + '0');
		lcd_wdat(sec%10 + '0');
		
		redisplay_sec = 0;
	}
}

void timer0_init() {
	IE |= 0x82;
	(TMOD &= 0xF0) |= 0x01;
	
	TH0 = (65536-50000) >> 8;
	TL0 = (65536-50000) & 0x00FF;
	TR0 = 1;
}

void timer0_up() interrupt 1 {
	static uchar count = 0;
	
	count++;
	TH0 = (65536-50000) >> 8;
	TL0 = (65536-50000) & 0x00FF;
	
	if(count == 20) {
		count = 0;
		
		redisplay_sec = 1;
		if(++sec >= 60) {
			sec = 0;
			redisplay_min = 1;
			
			if(++min >= 60) {
				min = 0;
				redisplay_hr = 1;
				
				if(++hr >= 24)
					hr = 0;
			}
		}
	}
}

/*******************************************************************/
/*
/* delay function
/*
/*******************************************************************/

void delay(int ms) {
	int i;
	while(ms--)
		for(i = 0; i< 1000; i++);
}

void delay_us(int us) {
	while(us--);
}


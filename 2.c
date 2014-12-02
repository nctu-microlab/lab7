#include<REG_MPC82G516.H>

#define uchar unsigned char
#define uint unsigned int
	
sbit btnA = P3 ^ 1;
sbit btnD = P3 ^ 4;

uchar sec, min, hr;		//second, minute, hour
uchar a_sec, a_min, a_hr;		// Alerm second, minute, hour
bit redisplay_sec, redisplay_min, redisplay_hr, redisplay_colon;

uchar clock_alarm;
#define NORMAL 0
#define SET_CLOCK 1
#define SET_ALARM 2
uchar set_pos;

bit lcd_busy();									//check if LCD is busy
void lcd_wcmd(uchar cmd);						//function to write command
void lcd_wdat(uchar dat);						//function to write data
void lcd_pos(uchar pos);						//location to show
void lcd_cursor(bit on_off);
void lcd_init();								//initial LCD
void print_time(void);							//show time!
void delay(int);
void delay_us(int);
void timer0_init();
void timer2_init();
void btnBC_init();
void check_btnAD();

/*******************************main program*************************************/
void main() {
	sec=10; min=10; hr=10;	   //initial start time;
	a_sec = a_min = a_hr = 0;
	redisplay_hr = redisplay_min = redisplay_sec = redisplay_colon = 1;
	clock_alarm = NORMAL;
	set_pos = 0;
	
	IE = TMOD = 0;
	lcd_init();
	btnBC_init();
	timer0_init();
	timer2_init();
	
	while(1) {
		print_time(); //print time
		check_btnAD();
	}
}

uchar POS[3] = {
	0x48, 0x4b, 0x4e
};

void print_time(void) {
	
	if(redisplay_colon) {
		redisplay_colon = 0;
		
		lcd_pos(0x4a);
		lcd_wdat(':');
		lcd_pos(0x4d);
		lcd_wdat(':');
	}

	if(redisplay_hr) {
		uchar display_hr = clock_alarm == SET_ALARM ? a_hr : hr;
		redisplay_hr = 0;
		
		lcd_pos(0x48);
		lcd_wdat(display_hr/10 + '0');// to_char
		lcd_wdat(display_hr%10 + '0');
	}

	if(redisplay_min) {
		uchar display_min = clock_alarm == SET_ALARM ? a_min : min;
		redisplay_min = 0;
		
		lcd_pos(0x4b);
		lcd_wdat(display_min/10 + '0');
		lcd_wdat(display_min%10 + '0');
	}

	if(redisplay_sec) {
		uchar display_sec = clock_alarm == SET_ALARM ? a_sec : sec;
		redisplay_sec = 0;
		
		lcd_pos(0x4e);
		lcd_wdat(display_sec/10 + '0');
		lcd_wdat(display_sec%10 + '0');
	}
	
	if(clock_alarm) {
		lcd_pos(POS[set_pos / 2] + set_pos % 2);
	}
}

void check_btnAD() {
	static bit clicked = 0;
	if(!clicked) {
		if(!btnA) {
			clicked = 1;
			
			if(clock_alarm == NORMAL) {
				TR0 = 0;
				clock_alarm = SET_CLOCK;
				lcd_cursor(1);
			} else if(clock_alarm == SET_CLOCK) {
				TR0 = 1;
				clock_alarm = NORMAL;
				lcd_cursor(0);
			}
		} else if(!btnD) {
			clicked = 1;
			
			if(clock_alarm == NORMAL) {
				clock_alarm = SET_ALARM;
				lcd_cursor(1);
				redisplay_hr = redisplay_min = redisplay_sec = 1;
			} else if(clock_alarm == SET_ALARM) {
				clock_alarm = NORMAL;
				lcd_cursor(0);
				redisplay_hr = redisplay_min = redisplay_sec = 1;
			}
		}
	} else if(btnA && btnD) {
		clicked = 0;
	}
}

void btnB() interrupt 0 {
	uchar *to_set = 0;
	uchar max;
	
	if(clock_alarm == SET_CLOCK) {
		switch(set_pos / 2) {
			case 0:
				to_set = &hr;
				redisplay_hr = 1;
				max = 24;
				break;
			case 1:
				to_set = &min;
				redisplay_min = 1;
				max = 60;
				break;
			case 2:
				to_set = &sec;
				redisplay_sec = 1;
				max = 60;
				break;
		}
	} else if(clock_alarm == SET_ALARM) {
		switch(set_pos / 2) {
			case 0:
				to_set = &a_hr;
				redisplay_hr = 1;
				max = 24;
				break;
			case 1:
				to_set = &a_min;
				redisplay_min = 1;
				max = 60;
				break;
			case 2:
				to_set = &a_sec;
				redisplay_sec = 1;
				max = 60;
				break;
		}
	}
	
	if(to_set) {
		if(set_pos % 2) {
			if(++(*to_set) >= max)
				*to_set -= max;
		} else {
			if((*to_set += 10) >= max)
				*to_set -= ((max / 10 + 1) * 10);
		}
	}
}

void btnC() interrupt 2 {
	if(clock_alarm != NORMAL) {
		if(set_pos == 5)
			set_pos = 0;
		else
			set_pos++;
	}
}

void btnBC_init() {
	IE |= 0x85;
	IT0 = 1;
	IT1 = 1;
}

void timer0_init() {
	IE |= 0x82;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	TH0 = (65536-50000) >> 8;
	TL0 = (65536-50000) & 0x00FF;
	TR0 = 1;
}

void timer2_init() {
	IE |= 0xA0;
	T2CON = 0x08;
	T2MOD = 0x02;
	
	TH2 = 0;
	TL2 = 0;
	RCAP2H = 63621 & 0xFF00;
	RCAP2L = 63621 >> 8;
}

void timer0_up() interrupt 1 {
	static uchar count = 0;
	
	count++;
	TH0 = (65536-50000) >> 8;
	TL0 = (65536-50000) & 0x00FF;
	
	if(count == 20) {
		count = 0;
		TR2 = 0;
		
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
		
		if(hr == a_hr && min == a_min && sec == a_sec) {
			TR2 = 1;
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


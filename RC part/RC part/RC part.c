#define  F_CPU 8000000UL
#define BAUD 9600
#define U2X_S 2 // U2X    1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)
#define sbi(reg, bit) reg |= (1<<(bit))
#define cbi(reg, bit) reg &= ~(1<<(bit))

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "USART_ch0.h"
#include "USART_ch1.h"
#include "DFplalyer_ch1.h"

void timer0_init(void);
void timer2_init(void);
void Go_Straight (void);
void Turn_Left (void);
void Turn_Right (void);
void Back (void);
void Stop (void);

volatile unsigned int  buf[8],dist[8],start=0,end=0;
volatile unsigned char cnt=0,flag[8]={0,};
volatile char sensor=0; // 초기값 검출안됨(직진)

char ch ;

volatile uint16_t total_count;

ISR(INT0_vect){
	if(EICRA==0x03)start=TCNT1;
	else
	{
		end=TCNT1; buf[0]=end-start; EIMSK=0;
		if(buf[0]<588)sensor|=2; else sensor&=~2; // 가운데
	}
	EICRA^=0x01;
}
//
ISR(INT1_vect){
	if(EICRA==0x0C)start=TCNT1;
	else
	{
		end=TCNT1; buf[1]=end-start; EIMSK=0;
		if(buf[1]<588)sensor|=4; else sensor&=~4; // 왼쪽
		
	}
	EICRA^=0x04;
}
//
ISR(INT4_vect){
	if(EICRB==0x03)start=TCNT1;
	else
	{
		end=TCNT1; buf[4]=end-start; EIMSK=0;
		if(buf[4]<588)sensor|=1; else sensor&=~1; // 오른쪽
	}
	EICRB^=0x01;
}


ISR(TIMER1_COMPA_vect)   //60ms 마다 인터럽트 발동되니 50번 발동되면 3초 60m*50=3
{
	total_count++;
	
	switch(cnt)
	{
		case 0: PORTA|=0x01; _delay_us(10); PORTA&=~0x01; EICRA=0x03; EICRB=0x00; EIFR=0xFF; EIMSK=0x01; break;
		case 1: PORTA|=0x02; _delay_us(10); PORTA&=~0x02; EICRA=0x0C; EICRB=0x00; EIFR=0xFF; EIMSK=0x02; break;
		case 2: PORTA|=0x10; _delay_us(10); PORTA&=~0x10; EICRA=0x00; EICRB=0x03; EIFR=0xFF; EIMSK=0x10; break;
	}
	if(++cnt>2)cnt=0;
}
int main(void)
{
	char val=0;
	DDRE = 0x00;//echo
	DDRA = 0xFF;//trig
	DDRB = 0XFF;
	
	TCCR1B=0x0A; OCR1A=59999; TIMSK=0x10; //8000000/256/(1+ 1840)=16.9744..==60ms

	timer0_init();
	timer2_init();
	USART0_Init ( MYUBRR );
	USART1_Init(MYUBRR);
	dfplayer_init();
	
	total_count = 0;
	
	SREG=0x80;

	while(1)
	{
		SREG&=~0x80; val=sensor; SREG|=0x80;
			
			ch = U0_RX();
			
			if (ch == '0')
			{
				switch(val)
				{
					case 0b000: Go_Straight(); break; //  전진
					
					case 0b100: case 0b010: case 0b110:   // 우회전 왼쪽 장애물
					Back();
					total_count = 0;
					while(total_count != 20); // 딜레이 1.2초
					Turn_Right();
					total_count = 0;
					while(total_count != 20);
					break;
					
					default:  // 좌회전 오른쪽 장애물
					Back();
					total_count = 0;
					while(total_count != 20);
					Turn_Left();
					total_count = 0;
					while(total_count != 20);
				}
			}
			
		/////////////////
		
		if (ch == '8'){Go_Straight();}
		if (ch == '2'){Back();}
		if (ch == '4'){Turn_Left();}
		if (ch == '6'){Turn_Right();}
		if (ch == '5'){Stop(); MP3_send_cmd(0X12,0,3);}
		if (ch == '7'){Stop();}
		
		/////////////////
	}
}
void timer0_init(void){TCCR0 = 0b01101100;}
void timer2_init(void){TCCR2 = 0b01101011;}
	
//OCR 값으로 PWM 조절 (속도) 255까지 가능 
void Turn_Right(void)
{
	OCR0=90;
	sbi(PORTB,2);
	cbi(PORTB,3);

	OCR2=90;
	cbi(PORTB,0);
	sbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Turn_Left(void)
{
	OCR0=90;
	cbi(PORTB,2);
	sbi(PORTB,3);
	
	OCR2=90;
	sbi(PORTB,0);
	cbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Go_Straight(void)
{
	OCR0=90;
	cbi(PORTB,2);
	sbi(PORTB,3);

	OCR2=90;
	cbi(PORTB,0);
	sbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Back(void)
{
	OCR0=90;
	sbi(PORTB,2);
	cbi(PORTB,3);
	
	OCR2=90;
	sbi(PORTB,0);
	cbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}

void Turn_Back_Right(void)
{
	OCR0=110;
	cbi(PORTB,2);
	sbi(PORTB,3);
	
	OCR2=110;
	sbi(PORTB,0);
	cbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}

void Turn_Back_Left(void)
{
	OCR0=110;
	sbi(PORTB,2);
	cbi(PORTB,3);
	
	OCR2=110;
	cbi(PORTB,0);
	sbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}

void Stop(void)
{
	OCR0=0;
	cbi(PORTB,2);
	cbi(PORTB,3);

	OCR2=0;
	cbi(PORTB,0);
	cbi(PORTB,1);
}

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
char ch ;
unsigned char RX_flag=0;
unsigned char rx_1;

ISR(INT0_vect){ if(EICRA==0x03)start=TCNT1; else{ end=TCNT1; buf[0]=end-start; EIMSK=0; flag[0]=1; } EICRA^=0x01; }
ISR(INT1_vect){ if(EICRA==0x0C)start=TCNT1; else{ end=TCNT1; buf[1]=end-start; EIMSK=0; flag[1]=1; } EICRA^=0x04; }
ISR(INT4_vect){ if(EICRB==0x03)start=TCNT1; else{ end=TCNT1; buf[4]=end-start; EIMSK=0; flag[4]=1; } EICRB^=0x01; }
ISR(USART0_RX_vect) {  rx_1=UDR0; RX_flag=1;}

ISR(TIMER1_COMPA_vect)
{
	switch(cnt){
		case 0: PORTA|=0x01; _delay_us(10); PORTA&=~0x01; EICRA=0x03; EICRB=0x00; EIFR=0xFF; EIMSK=0x01; break;
		case 1: PORTA|=0x02; _delay_us(10); PORTA&=~0x02; EICRA=0x0C; EICRB=0x00; EIFR=0xFF; EIMSK=0x02; break;
		case 4: PORTA|=0x19; _delay_us(10); PORTA&=~0x10; EICRA=0x00; EICRB=0x03; EIFR=0xFF; EIMSK=0x10; break;
	}
	if(++cnt>4)cnt=0;
}
int main(void)
{
	

	sbi(UCSR0B, RXCIE); // Enable Rx Interrupt
	
	DDRE = 0x00;//echo    낅젰 쇰줈  ㅼ젙
	DDRA = 0xFF;//trig   異쒕젰 쇰줈  ㅼ젙

	
	TCCR1B=0x0C; OCR1A=1840; TIMSK=0x10; //8000000/256/(1+ 1840)=16.9744..==60ms

	timer0_init();
	timer2_init();
	USART0_Init ( MYUBRR );
	USART1_Init(MYUBRR);
	dfplayer_init();
	
	MP3_send_cmd(0X12,0,1);
	
	SREG=0x80;

	while(1)
	{
		ch = USART0_Receive();
		/////////////////
		if (ch == '0')
		{
			Go_Straight();
			if(flag[0])
			{ flag[0]=0; dist[0]=(int)((float)buf[0]/1.8125);
				if (dist[0]<=5)
				{
					Back();
					_delay_ms(500);
					Stop();
					_delay_ms(100);
					MP3_send_cmd(0X12,0,2); _delay_ms(2000);
					Turn_Right();
					_delay_ms(500);
					
				}
			}
			if(flag[1])
			{ flag[1]=0; dist[1]=(int)((float)buf[1]/1.8125);
				if (dist[1]<=5)
				{
					Back();
					_delay_ms(500);
					Stop(); _delay_ms(100);
					MP3_send_cmd(0X12,0,2); _delay_ms(2000);
					Turn_Right();
					_delay_ms(500);
					

				}
			}
			if(flag[4])
			{ flag[4]=0; dist[4]=(int)((float)buf[4]/1.8125);
				if (dist[4]<=5)
				{
					Back();
					_delay_ms(500);
					Stop();   _delay_ms(100);
					
					MP3_send_cmd(0X12,0,2); _delay_ms(2000);
					Turn_Left();
					_delay_ms(500);
					
				}
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
void Turn_Right(void)
{
	sbi(PORTB,4);
	sbi(PORTB,2);
	cbi(PORTB,3);

	sbi(PORTB,7);
	cbi(PORTB,0);
	sbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Turn_Left(void)
{
	sbi(PORTB,4);
	cbi(PORTB,2);
	sbi(PORTB,3);
	
	sbi(PORTB,7);
	sbi(PORTB,0);
	cbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Go_Straight(void)
{
	sbi(PORTB,4);
	cbi(PORTB,2);
	sbi(PORTB,3);

	sbi(PORTB,7);
	cbi(PORTB,0);
	sbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Back(void)
{
	sbi(PORTB,4);
	sbi(PORTB,2);
	cbi(PORTB,3);
	
	sbi(PORTB,7);
	sbi(PORTB,0);
	cbi(PORTB,1);
	
	MP3_send_cmd(0X12,0,4);
}
void Stop(void)
{
	cbi(PORTB,4);
	cbi(PORTB,2);
	cbi(PORTB,3);

	cbi(PORTB,7);
	cbi(PORTB,0);
	cbi(PORTB,1);
	
	
	
}
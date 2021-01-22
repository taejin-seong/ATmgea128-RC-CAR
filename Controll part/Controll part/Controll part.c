#define F_CPU 8000000UL
#define FS_SEL 16.4
//#define TWI_FERQ 100000

#include <stdio.h>
#include<avr/io.h> 
#include<avr/interrupt.h>
#include<util/delay.h>
#include<math.h>
#include "clcd_d8.h"
#include <stdio.h>


unsigned char twi_read(char addressr);
void twi_write(unsigned char address,unsigned char data);
void USART_Transmit(unsigned char tx_data);
void USART_Transmit_init4(int data);
void get_raw_data();
void calibrate();
void function_1(int sw);
unsigned short read_adc();
void init_adc1();
void init_adc2();




volatile double dt = 0.000;
volatile int temp;
volatile unsigned char a_x_l,a_x_h,a_y_l,a_y_h,a_z_l,a_z_h;
volatile unsigned char g_x_l,g_x_h,g_y_l,g_y_h,g_z_l,g_z_h;
volatile double bas_a_x,bas_a_y,bas_a_z;
volatile double bas_g_x,bas_g_y,bas_g_z;
volatile double a_x,a_y,a_z;
volatile double g_x,g_y,g_z;
volatile double las_angle_gx,las_angle_gy,las_angle_gz;
volatile double angle_ax,angle_ay,angle_az;
volatile double angle_gx,angle_gy,angle_gz;
volatile double roll,pitch,yaw;
volatile double alpha;


int main()
{ 
   DDRB=0x00;
   
   
   //UART
   UCSR0A = 0x00;
   UCSR0B = 0b00011000;
   UCSR0C = 0b00000110;
   UBRR0H = 0;
   UBRR0L = 51;      //9600


   //TWI(I2C)
   TWCR = (1<<TWEN);
   TWBR =  2;//(F_CPU/TWI_FERQ-16)/2;         //100khz

   //TIMER0
   TCCR0 = (1<<CS02)|(1<<CS01);   //256 분주 
   TCNT0 = 256-62;            //125 번 => 0.002s
   TIMSK = (1<<TOIE0);

   //MPU6050 init
   twi_write(0x6B, 0x00);    //sleep 끔
   _delay_ms(10);
   twi_write(0x1A, 0x05);    //DLPF 10Hz
   
   calibrate();
   
   clcd_port_init();
   clcd_init_8bit();
   clcd_command(CMD_DISPLAY_OPTION5);

    SREG = 0x80;
   
   while(1)
   { 
    
    
      get_raw_data();
     

      las_angle_gx = roll;   //최근값 누적
      las_angle_gy = pitch;
      las_angle_gz = yaw;

      temp = (a_x_h<<8) | a_x_l;
      a_x = - temp;
      temp = (a_y_h<<8) | a_y_l;
      a_y = - temp;
      temp = (a_z_h<<8) | a_z_l;
      a_z = temp;
      temp = (g_x_h<<8) | g_x_l;
      g_x = temp;
      temp = (g_y_h<<8) | g_y_l;
      g_y = temp;
      temp = (g_z_h<<8) | g_z_l;
      g_z = temp;

      g_x = (g_x - bas_g_x)/FS_SEL;
      g_y = (g_y - bas_g_y)/FS_SEL;
      g_z = (g_z - bas_g_z)/FS_SEL;
     
      a_x = (a_x -bas_a_x);
      a_y = (a_x -bas_a_y);
     a_z = (a_x -bas_a_z);
      
      angle_ax = atan(-1.000*a_y/sqrt(pow(a_x,2) + pow(a_z,2)))*180/3.141592;
      angle_ay = atan(a_x/sqrt(pow(a_y,2) + pow(a_z,2)))*180/3.141592;

      angle_gx = g_x*dt + las_angle_gx;
      angle_gy = g_y*dt + las_angle_gy;
      angle_gz = g_z*dt + las_angle_gz;

      dt = 0.000;

      alpha = 0.98;
      roll = alpha*angle_gx + (1.000 - alpha)*angle_ax;
      pitch = alpha*angle_gy + (1.000 - alpha)*angle_ay;
      yaw = angle_gz;
     

       function_1(PINB);  //스위치용
   
      
   
       _delay_ms(10);
   
      
   } 

} 

void function_1(int sw)
{
   unsigned short value;

   switch(sw)
   {
      ///////////////1
      case 0x01: //MPU6050용
      {
         
         clcd_position(0,1); clcd_str("MPU6050 START");_delay_ms(1);
         
         
         if (roll <= -20)
         {   
            USART_Transmit('8');
            clcd_clear();
            clcd_position(1,7); clcd_str("GO"); 
         
            
         }
         if (roll >= 20 )
         {   
            USART_Transmit('2');
            clcd_clear();
             clcd_position(1,6); clcd_str("Back"); 
                        
            
         }
         if (pitch <= -20 )
         {
            USART_Transmit('4');
            clcd_clear();
             clcd_position(1,6); clcd_str("LEFT");
         }
         if (pitch >=20 )
         {
            USART_Transmit('6');
            clcd_clear();
           clcd_position(1,6); clcd_str("RIGHT");
         }
         
            
          roll = 0;
          pitch = 0;
            
            break;
               
      }
      ///////////////2
      case 0x02: ///조이스틱용
      {
         
   
   
      clcd_position(0,1); clcd_str("JOYSTICK START"); _delay_ms(1);
                                 
         
      
         init_adc1();
         value = read_adc();
      
         if (value <=5)
         {
            USART_Transmit('2');
            clcd_clear();
            clcd_position(1,6); clcd_str("Back"); 
            
         }
         
         if (value >= 1000)
         {
            USART_Transmit('8');
            clcd_clear();
            clcd_position(1,7); clcd_str("GO");
             
         }
      
         _delay_ms(10);
         
         init_adc2();
         value=read_adc();
         
         if (value <=5)
         {
            USART_Transmit('4');
            clcd_clear();
            clcd_position(1,6); clcd_str("LEFT"); 
            
         }
         
         if (value >= 1000)
         {
            USART_Transmit('6');
            clcd_clear();
            clcd_position(1,6); clcd_str("RIGHT");       
            
         }
      
         
         
         break;
      }
      ///////////////3
      case 0x04: //자율주행용
      {
         
         
         USART_Transmit('0');
         clcd_clear();
       
          clcd_position(0,1); clcd_str("AUTOMODE START"); 
      
   
         
         break;
      }
      
      case 0x0A:
      case 0x09: //mpu6050, 조이스틱 정지용
      {   
         clcd_clear();
         clcd_position(0,6);clcd_str("STOP");
         USART_Transmit('7');
         break;
         
      }
      ///////////////
      
      case 0x03:
      case 0x05:
      case 0x06:
      case 0x07:
      {   
         USART_Transmit('5');
         clcd_clear();
         
         clcd_position(0,4); clcd_str("DONT PUSH"); 
         
            break;
      }
      
      default:
      {
         
             USART_Transmit('5');
            clcd_clear();
          
             clcd_position(0,2); clcd_str("CHOICE MODE");
      }
      
   }
}
ISR(TIMER0_OVF_vect)   //0.002s
{
   dt += 0.002;

   TCNT0 = 256-62;
}

void calibrate()   //초기값 읽기 
{
   int cal = 10;

   for(int i=0; i<cal; i++)   //평균 
   {
      get_raw_data();
   
      temp = (a_x_h<<8) | a_x_l;
      a_x += - temp;
      temp = (a_y_h<<8) | a_y_l;
      a_y += - temp;
      temp = (a_z_h<<8) | a_z_l;
      a_z += temp;
      temp = (g_x_h<<8) | g_x_l;
      g_x += temp;
      temp = (g_y_h<<8) | g_y_l;
      g_y += temp;
      temp = (g_z_h<<8) | g_z_l;
      g_z += temp;

      _delay_ms(10);
   }   
   
   a_x /= cal;
   a_y /= cal;
   a_z /= cal;
   g_x /= cal;
   g_y /= cal;
   g_z /= cal;

   bas_a_x = a_x;   //초기 값으로 저장 
   bas_a_y = a_y;
   bas_a_z = a_z;
   bas_g_x = g_x;
   bas_g_y = g_y;
   bas_g_z = g_z;

}

void get_raw_data()
{
   a_x_h = twi_read(0x3B);      //x축 가속도
   _delay_us(10);
   a_x_l = twi_read(0x3C);
   _delay_us(10);
   a_y_h = twi_read(0x3D);
   _delay_us(10);      //y축 가속도 
   a_y_l = twi_read(0x3E);
   _delay_us(10);      
   a_z_h = twi_read(0x3F);
   _delay_us(10);      //z축 가속도 
   a_z_l = twi_read(0x40);   
   _delay_us(10);   
   g_x_h = twi_read(0x43);   
   _delay_us(10);   //x축 각속도 
   g_x_l = twi_read(0x44);   
      _delay_us(10);
   g_y_h = twi_read(0x45);   
      _delay_us(10);
   g_y_l = twi_read(0x46);   
      _delay_us(10);
   g_z_h = twi_read(0x47);   
   _delay_us(10);
   g_z_l = twi_read(0x48);   
      _delay_us(10);
}
void twi_write(unsigned char address,unsigned char data)
{ 
   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //START

   while(!(TWCR & (1<<TWINT))); //TWINT flag 기다림 
   while((TWSR&0xF8) != 0x08);  //START 상태(08) 기다림  
   
   _delay_us(1);

   TWDR = 0b11010000;          //AD(1101000)+W(0) 
   TWCR = (1<<TWINT)|(1<<TWEN); //전송 

   while(!(TWCR & (1<<TWINT))); 
   while((TWSR&0xF8) != 0x18);  //SLA+W ACK 상태(18) 기다림

   TWDR = address;           //register address
   TWCR = (1<<TWINT)|(1<<TWEN); //전송

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x28);  //Data ACK 상태(28) 기다림 

   TWDR = data;              //data 
   TWCR = (1<<TWINT)|(1<<TWEN); //전송  

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x28);

   TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //STOP
} 
unsigned char twi_read(char address)
{ 
   unsigned char data;

   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //START

   while(!(TWCR & (1<<TWINT))); //TWINT flag 기다림 
   while((TWSR&0xF8) != 0x08);  //START 상태(08) 기다림  

   TWDR = 0b11010000;          //AD(1101000)+W(0) 
   TWCR = (1<<TWINT)|(1<<TWEN); //전송 

   while(!(TWCR & (1<<TWINT))); 
   while((TWSR&0xF8) != 0x18);  //SLA+W ACK 상태(18) 기다림

   TWDR = address;           //register address
   TWCR = (1<<TWINT)|(1<<TWEN); //전송

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x28);  //Data ACK 상태(28) 기다림 

   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //Repeat START

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x10);  //Repeat START 상태(08) 기다림

   TWDR = 0b11010001;          //AD(1101000)+R(1) 
   TWCR = (1<<TWINT)|(1<<TWEN); //전송 

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x40);  //SLA+R ACK 상태(40) 기다림 

   TWCR = (1<<TWINT)|(1<<TWEN); //전송

   while(!(TWCR & (1<<TWINT)));
   while((TWSR&0xF8) != 0x58);  //ACK 상태(58) 기다림 

   data = TWDR; 

   TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //STOP

   return data; 
}
void USART_Transmit(unsigned char tx_data)
{ 
   while(!(UCSR0A & (1<<UDRE0)));
   UDR0 = tx_data; 
}

void USART_Transmit_init4(int data)
{
   if(data < 0)
   {
      data = -data;
      USART_Transmit('-');
   }
   else
      USART_Transmit(' ');

   int temp = 0;
 //  temp = data/10000;
 //  USART_Transmit(temp+48);
 //  temp = (data%10000)/1000;
 //  USART_Transmit(temp+48);
 //  temp = (data%1000)/100;
  // USART_Transmit(temp+48);
   temp = (data%100)/10;   
   USART_Transmit(temp+48);
   temp = data%10; 
   USART_Transmit(temp+48);
}

void init_adc1() //----1번
{
   ADMUX=0X40; //0b01000000 <---- AVCC사용 오른쪽정렬  ADC0번 사용 , 싱글채널 인풋
   ADCSRA=0X87; // 0b10000111 <---- ADC실행 가능, 128분주 사용
}
void init_adc2()
{
   ADMUX =0X41;
   ADCSRA=0X87;
}
unsigned short read_adc()// ------2번
{
   unsigned char adc_low, adc_high;
   unsigned short value;
   ADCSRA |=0x40;  // ADC시작
   while((ADCSRA & 0x10) != 0x10); //반복실행을 한 후 위한것으로 ADC가 끝나면 플래그가 1로 set되어서 반복 종료
   adc_low =ADCL;
   adc_high=ADCH;
   value =(adc_high << 8 ) | adc_low; //short 변수인 value에 adc 하이 로우 값을 넣는다
   return value;
}

/*

Header file for 8 bits Data Bus

*/
typedef unsigned char  INT8;
typedef unsigned int   INT16;


#define EN PC7   // Enable bit of Control Bus
#define RW PC6   // Read/Write bit of Control Bus
#define RS PC5   // Reset bit of Control Bus

#define DATA_BUS                             PORTA // 8 bits of Data Bus
#define CONTROL_BUS                          PORTC // 3 bits of Control Bus
#define DATA_BUS_DIRECTION                   DDRA
#define CONTROL_BUS_DIRECTION                DDRC

#define DATA_BUS_BUSY                        PINA // Busy Check at Data Bus
#define BUSY_BIT                             0x80 // Busy Check Bit --> Bit 7

#define sbi(reg,bit)    reg |= (1<<(bit))	  // Set "bit"th bit of Register "reg"		
#define cbi(reg,bit)    reg &= ~(1<<(bit))


#define CMD_CLEAR_DISPLAY                    0x01 // Clear All Display
#define CMD_RETURN_HOME                      0x02 // Cursor Position -> Return Home

#define CMD_ENTRY_MODE_SET_OPTION1           0x04 //   커서 좌측 이동, 화면이동 없음
#define CMD_ENTRY_MODE_SET_OPTION2           0x05 //   커서 좌측 이동, 화면 이동
#define CMD_ENTRY_MODE_SET_OPTION3           0x06 // * 커서 우측 이동, 화면이동 없음 
#define CMD_ENTRY_MODE_SET_OPTION4           0x07 //   커서 우측 이동, 화면이동 

#define CMD_DISPLAY_OPTION1                  0x08 //   화면 OFF,  커서 OFF,  커서 점멸 OFF 
#define CMD_DISPLAY_OPTION2                  0x09 //   화면 OFF,  커서 OFF,  커서 점멸 ON
#define CMD_DISPLAY_OPTION3                  0x0A //   화면 OFF,  커서 ON ,  커서 점멸 OFF
#define CMD_DISPLAY_OPTION4                  0x0B //   화면 OFF,  커서 ON ,  커서 점멸 ON
#define CMD_DISPLAY_OPTION5                  0x0C //   화면 ON ,  커서 OFF,  커서 점멸 OFF
#define CMD_DISPLAY_OPTION6                  0x0D //   화면 ON ,  커서 OFF,  커서 점멸 ON
#define CMD_DISPLAY_OPTION7                  0x0E //   화면 ON ,  커서 ON ,  커서 점멸 OFF
#define CMD_DISPLAY_OPTION8                  0x0F // * 화면 ON ,  커서 ON ,  커서 점멸 ON

#define CMD_CURSOR_DISPLAY_SHIFT_OPTION1     0x10 //   커서 선택,   커서 좌측 이동
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION2     0x14 //   커서 선택,   커서 우측 이동
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION3     0x18 //   화면 선택,   커서 좌측 이동
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION4     0x1C //   화면 선택,   커서 우측 이동

#define CMD_FUNCTION_SET_OPTION1              0x20 //   4비트,   화면1행,    5x8  Font
#define CMD_FUNCTION_SET_OPTION2              0x24 // * 4비트,   화면1행,    5x11 Font
#define CMD_FUNCTION_SET_OPTION3              0x28 // * 4비트,   화면2행,    5x8  Font
#define CMD_FUNCTION_SET_OPTION4              0x2C //   4비트,   화면2행,    5x11 Font
#define CMD_FUNCTION_SET_OPTION5              0x30 //   8비트,   화면1행,    5x8  Font
#define CMD_FUNCTION_SET_OPTION6              0x34 // * 8비트,   화면1행,    5x11 Font
#define CMD_FUNCTION_SET_OPTION7              0x38 // * 8비트,   화면2행,    5x8  Font
#define CMD_FUNCTION_SET_OPTION8              0x3C //   8비트,   화면2행,    5x11 Font

// function 
void clcd_port_init(void);
void clcd_init_8bit(void);
void clcd_busy_check(void);
void clcd_command(INT8 cmd);
void clcd_data(INT8 data);
void clcd_str(char *string);
void clcd_position(INT8 col, INT8 row);
void clcd_clear(void);


void clcd_init_8bit(void)
{
	clcd_command(CMD_FUNCTION_SET_OPTION7);
	_delay_ms(5);
	clcd_command(CMD_FUNCTION_SET_OPTION7);
	_delay_ms(1);
	clcd_command(CMD_FUNCTION_SET_OPTION7);
	clcd_command(CMD_FUNCTION_SET_OPTION7);
	
	clcd_command(CMD_DISPLAY_OPTION8);
	clcd_command(CMD_CLEAR_DISPLAY);
	clcd_command(CMD_ENTRY_MODE_SET_OPTION3);
}

void clcd_port_init(void) 
{
	CONTROL_BUS_DIRECTION =   0xF0;
	DATA_BUS_DIRECTION=       0x00;
	CONTROL_BUS=              0x00;
	DATA_BUS=                 0x00;
}

void clcd_busy_check(void) 
{
	INT8 busy_flag=0;
	INT16 busy_time=0;
	
	DATA_BUS_DIRECTION = 0x00;

	do 
	{
		cbi(CONTROL_BUS, RS); //RS=0
		sbi(CONTROL_BUS, RW); //RW=1
		sbi(CONTROL_BUS, EN); //EN=1
		_delay_us(10);
		busy_flag=DATA_BUS_BUSY;
		cbi(CONTROL_BUS, EN); //EN=0
		busy_time++;
	} while((busy_flag & BUSY_BIT) && (busy_time<500));

	DATA_BUS_DIRECTION=0xFF;   
}

void clcd_command(INT8 cmd)
{
	clcd_busy_check();

	cbi(CONTROL_BUS, RS);  
	cbi(CONTROL_BUS, RW);  
	sbi(CONTROL_BUS, EN);  
	DATA_BUS=cmd;
	_delay_us(10);
	cbi(CONTROL_BUS, EN);  
}

void clcd_data(INT8 cmd)
{
	clcd_busy_check();

	sbi(CONTROL_BUS, RS);  
	cbi(CONTROL_BUS, RW);  
	sbi(CONTROL_BUS, EN);  
	DATA_BUS=cmd;
	_delay_us(10);
	cbi(CONTROL_BUS, EN);  
}

void clcd_str(char *string)
{
	while(*string != '\0')
	{
		clcd_data(*string);
		string++;
	}
}

void clcd_str_delay(char *string)
{
	while(*string != '\0')
	{
		clcd_data(*string);
		string++;
		_delay_ms(200);
	}
}

void clcd_position(INT8 col, INT8 row)
{
	clcd_command(0x80|(col*0x40+row));
}

void clcd_clear(void)
{
	clcd_command(0x01);
}

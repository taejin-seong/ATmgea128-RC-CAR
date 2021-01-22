/*
 Header file for USART #0 
*/ 
typedef  unsigned char  INT8;
typedef  unsigned int   INT16;

#define sbi(reg, bit)    reg |= (1<<(bit))    
#define cbi(reg, bit)    reg &= ~(1<<(bit))

#define BAUD  9600
#define U2X_S  2     // U2X --> 1 or 2
#define MYUBRR  ((F_CPU*U2X_S)/(16L*BAUD)-1)

void USART1_Init( INT16 ubrr );
void USART1_Transmit( char data );
void USART1_Transmit_String(char *str);
void USART1_Transmit_NewLine(void);
char USART1_Receive( void );


void USART1_Init( INT16 ubrr )
{
	// Set baud rate
	UBRR1H = (INT8)(ubrr>>8);
	UBRR1L = (INT8)ubrr;
	// Enable U2X
	if(U2X_S == 2)
		sbi(UCSR1A, U2X);
	else
		cbi(UCSR1A, U2X);
	// Enable receiver and transmitter
	sbi(UCSR1B, RXEN);
	sbi(UCSR1B, TXEN);

	// Set frame format: 8data, 1stop bit
	cbi(UCSR1C, UMSEL);  // asynch
	cbi(UCSR1C, USBS);   // 1 Stop bit
	cbi(UCSR1C, UPM11);  // No parity
	cbi(UCSR1C, UPM10);
	cbi(UCSR1B, UCSZ12); // 8-bit
	sbi(UCSR1C, UCSZ11);
	sbi(UCSR1C, UCSZ10);

	// Enable interrupt
//	sbi(UCSR0B, RXCIE);  // RX
//	sbi(UCSR0B, TXCIE);  // TX
//	sbi(UCSR0B, UDRIE);  // UDR Empty

//	sei();  // Enable all interrupt
}

void USART1_Transmit( char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSR1A & 0x20 ) )   // (1<<UDRE)
	{
		;
	}
	// Put data into buffer, sends the data
	UDR1 = data;
}

void USART1_Transmit_String(char *str)
{
	while(*str != '\0')
	{
		if(*str == '\n')
		{
			USART1_Transmit_NewLine();
		}
		else
		{
			USART1_Transmit(*str);
		}
		str++;
	}
}

void USART1_Transmit_NewLine(void)
{
	USART1_Transmit(0x0A); // LF
	USART1_Transmit(0x0D); // CR
}


char USART1_Receive( void )
{
	// Wait for data to be received
	while ( !(UCSR1A & 0x80) )  //(1<<RXC)
	{
		;
	}
	// Get and return received data from buffer
	return UDR1;
}

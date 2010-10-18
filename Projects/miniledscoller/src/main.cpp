/*
altparty2010 promo scroller
Copyright (C) 2010 paeae oy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart_serial.h"
/*
defines
*/
#define EEPROM_POS_MSGLEN 0
#define EEPROM_POS_TAG 1
#define EEPROM_POS_MSG 2

#define EEPROM_TAG 0xba

#define PRESCALER 1
#define TIMER_RESOLUTION 10000 //1ms
#define TCNT_VALUE (0xffff-((F_CPU/PRESCALER)/TIMER_RESOLUTION))

#define MODE_BLINK 0
#define MODE_SCROLL 1

#define REFRESH_MS 80



#define COLMASK 0x1f
#define ROWMASK 0x7f

#define COLPORT PORTD
#define COLSTARTBIT 2
#define COLPIN PIND

#define ROWPORT PORTB
#define ROWPIN PINB

#include "chardata.cpp"


/*
default meesage
*/

char ee_msg[] __attribute__((section(".eeprom"))) = "  PAEAE.COM  ALTPARTY 2010  ";

/*
constant strings
*/
const char s_prompt[] PROGMEM = "\r\n? for help> ";
const char s_ok[] PROGMEM = "\r\nok\r\n";
const char s_text[] PROGMEM = "\r\ntext: ";
const char s_help[] PROGMEM = "\r\n? - help\n[string] set new text\r\n\r\n";

void serial_pgm_write(const char* p)
{
	uint8_t b;
	while((b = pgm_read_byte(p)) != 0)
	{
		serial_write(b);
		p++;
	}
}

/* 
EEPROM functions
*/
inline void eeprom_wait()
{
		while(EECR & (1<<EEPE))
		{
		}
}

uint8_t eeprom_read(uint8_t addr)
{
		eeprom_wait();
		
		EEAR = addr;
		EECR |= _BV(EERE);
		
		return EEDR;
}

void eeprom_write(uint8_t addr,uint8_t v)
{
		//first read the eeprom value and if it's the same, don't write. this saves eeprom cycles
		if (eeprom_read(addr) == v)
				return;
				
		//eeprom_wait(); //no need because read waits
		EEAR = addr;
		EEDR = v;
		EECR |= _BV(EEMPE);
		EECR |= _BV(EEPE);
}





/* 
variables
*/
volatile uint8_t msgpos;
volatile uint8_t displaybuf[5];
volatile uint32_t _clock;
volatile uint8_t nextchar;
volatile uint8_t scrollpos;
volatile uint8_t row;
volatile uint32_t __delay;

uint32_t clock()
{
	uint8_t s = SREG;
	cli();
	uint32_t t = _clock;
	SREG = s;
	return t;
}


void setchar(uint8_t b)
{
	for (uint8_t i=0;i<sizeof(s_charlist);i++)
	{
		if (b == pgm_read_byte(&s_charlist[i]))
		{
			nextchar = i;
			break;
		}
	}
	
	
}

ISR(TIMER1_OVF_vect)
{
	_clock++;
	TCNT1 = TCNT_VALUE;
	if (!row && _clock > __delay)
	{
		__delay = _clock+REFRESH_MS;
		for (int x=1;x<5;x++)
			displaybuf[x-1]=displaybuf[x];


		if (scrollpos>4)
		{
			displaybuf[4]=0;
			scrollpos++;
			if (scrollpos > 6)
			{
				scrollpos=0;
				
				uint8_t c = eeprom_read(EEPROM_POS_MSG+msgpos);
				if (c == '\0')
				{
					msgpos=0;
					c = ' ';
				}
				else
					msgpos++;

				setchar(c);
			}
		}
		else
		{
			displaybuf[4] = pgm_read_byte(&s_chardata[nextchar][scrollpos]);
			scrollpos++;
		}
	}

	for (int x=0;x<5;x++)
	{
		//turn rows off
		COLPORT|=COLMASK<<COLSTARTBIT;
	
		ROWPORT = displaybuf[x] & ROWMASK;
		COLPORT&=~(1<<x)<<COLSTARTBIT;
	}
	//turn rows off
	COLPORT|=COLMASK<<COLSTARTBIT;
//	ROWPORT=0;
	
	row=(row+1)%5;
}

void delay(uint32_t ms)
{
	uint32_t t = clock();
	while(clock() < t+ms*(TIMER_RESOLUTION/1000))
	{
		__asm("nop");
	}
}

void setup()
{
	DDRB = ROWMASK;
	DDRD |= COLMASK<<COLSTARTBIT;
	
	serial_init();
	uint8_t i=EEPROM_POS_MSG;
	serial_pgm_write(s_text);
	uint8_t b;
	while((b=eeprom_read(i++)) != 0)
	{
		serial_write(b);
	}
	
	serial_pgm_write(s_prompt);
	
	TCCR1B = _BV(CS00);
	TIMSK |= _BV(TOIE1);
	TCNT1 = TCNT_VALUE;
	
	
	setchar(eeprom_read(EEPROM_POS_MSG));
	msgpos=1;
	sei();
	
}

char cmdbuf[16];
uint8_t bufpos;

inline void clear_buf()
{
	bufpos=0;
}


void read_serial()
{
	while (serial_available())
	{
		uint8_t b = serial_receive();
		serial_write(b);
		if (b == '\n' || b == '\r')
		{
			if (bufpos > 0)
			{
				switch(cmdbuf[0])
				{
					case '?':
						serial_pgm_write(s_help);
						break;
					default:
					{
						cli();
						COLPORT|=COLMASK<<COLSTARTBIT;
						uint8_t i;
						for (i=0;i<bufpos;i++)
							eeprom_write(EEPROM_POS_MSG+i,cmdbuf[i]);

						eeprom_write(EEPROM_POS_MSG+i,'\0');
						serial_pgm_write(s_ok);
						msgpos=0;
						scrollpos=0;
						sei();
					}
				}
			}
			clear_buf();
			serial_pgm_write(s_prompt);
		}
		else
		{
			if (bufpos<sizeof(cmdbuf))
				cmdbuf[bufpos++]=b;
		}
	}
}

void loop()
{
	read_serial();
//	MCUCR |= _BV(SE);
//	__asm("sleep");
}



int main()
{
	setup();
	
	while(true)
	{
		loop();
	}
	
}
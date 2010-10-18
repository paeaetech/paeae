/*
avr uart serial library
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
#include <avr/interrupt.h>
#include <inttypes.h>
#include "uart_serial.h"


#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168__)

#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C

#define UDR UDR0
#define UDRE UDRE0
#define UCSZ1 UCSZ01
#define UCSZ0 UCSZ00
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define U2X U2X0

#define RXEN RXEN0
#define TXEN TXEN0
#define RXCIE RXCIE0

#elif defined(__AVR_ATtiny2313__)
//attiny2313 has correct defines already
#else
#error "unsupported cpu"
#endif

#ifndef F_CPU
#error "F_CPU not defined"
#endif


#define RECV_SIZE UART_SERIAL_BUFFER_SIZE

uint8_t _recv_buf[RECV_SIZE];
uint8_t _recv_read;
uint8_t _recv_write;
uint8_t _recv_len;

ISR(USART_RX_vect)
{
	_recv_buf[_recv_write] = UDR;
	_recv_write = (_recv_write+1) & (RECV_SIZE-1);
	if (_recv_len < RECV_SIZE)
		_recv_len++;
}


uint8_t serial_available()
{
	return _recv_len;
}

uint8_t serial_receive()
{
	while(_recv_len == 0) {}
	
	uint8_t b = _recv_buf[_recv_read];
	_recv_read = (_recv_read+1) & (RECV_SIZE-1);
	_recv_len--;
	return b;
}

inline uint8_t reverse(uint8_t b)
{
	uint8_t t=0;
	for (uint8_t i=0;i<8;i++)
	{
		t |= ((b>>i)&1)<<(7-i);
	}
	return t;
}

void serial_write(uint8_t b)
{
	while ( !( UCSRA & _BV(UDRE)) ) {}
	
	UDR = b;
}

void serial_send(const char* t)
{
	while(*t)
		serial_write(*t++);
}

void serial_send(uint8_t* buf,uint8_t size)
{
	while(size--)
	{
		while ( !( UCSRA & _BV(UDRE)) ) {}

		UDR = *buf++;
	}
}

void serial_init(uint16_t baudrate)
{

#define BAUD 9600
#include <util/setbaud.h>
//	uint16_t ubrr = (uint16_t)((F_CPU/(baudrate*16))-1);
	
//	UBRRH = ubrr >> 8;
//	UBRRL = ubrr & 0xff;
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	
	UCSRA = _BV(U2X);
	UCSRB = _BV(RXEN)|_BV(TXEN) | _BV(RXCIE);
	UCSRC = _BV(UCSZ1) | _BV(UCSZ0);
	
}

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
#ifndef __UART_SERIAL_H__
#define __UART_SERIAL_H__

#define UART_SERIAL_BUFFER_SIZE 8


uint8_t serial_available();
uint8_t serial_receive();
void serial_write(uint8_t b);
void serial_send(const char* t);
void serial_send(uint8_t* buf,uint8_t size);
void serial_init(uint16_t baudrate=9600);


#endif /* __UART_SERIAL_H__ */

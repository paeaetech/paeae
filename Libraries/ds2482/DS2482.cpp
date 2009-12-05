/*
  DS2482 library for Arduino
  Copyright (C) 2009 Paeae Technologies

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

	crc code is from OneWire library


Updates:
	dec 5th, 2009: included a search function fix by Don T
	see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1242137666
*/
#include "WConstants.h"

#include "DS2482.h"
#include "Wire.h"


#define PTR_STATUS 0xf0
#define PTR_READ 0xe1
#define PTR_CONFIG 0xc3


DS2482::DS2482(uint8_t addr)
{
	mAddress = 0x18 | addr;
	
}

//-------helpers
void DS2482::begin()
{
	Wire.beginTransmission(mAddress);
}

void DS2482::end()
{
	Wire.endTransmission();
}

void DS2482::setReadPtr(uint8_t readPtr)
{
	begin();
	Wire.send(0xe1);
	Wire.send(readPtr);
	end();
}

uint8_t DS2482::readByte()
{
	Wire.requestFrom(mAddress,(uint8_t)1);
	return Wire.receive();
}

uint8_t DS2482::wireReadStatus(bool setPtr)
{
	if (setPtr)
		setReadPtr(PTR_STATUS);
	
	return readByte();
}

uint8_t DS2482::busyWait(bool setReadPtr)
{
	uint8_t status;
	int loopCount = 1000;
	while((status = wireReadStatus(setReadPtr)) & DS2482_STATUS_BUSY)
	{
		if (--loopCount <= 0)
		{
			mTimeout = 1;
			break;
		}
		delayMicroseconds(20);
	}
	return status;
}

//----------interface
void DS2482::reset()
{
	mTimeout = 0;
	begin();
	Wire.send(0xf0);
	end();
}

bool DS2482::configure(uint8_t config)
{
	busyWait(true);
	begin();
	Wire.send(0xd2);
	Wire.send(config | (~config)<<4);
	end();

	return readByte() == config;
}

bool DS2482::selectChannel(uint8_t channel)
{	
	uint8_t ch, ch_read;

	switch (channel)
	{
		case 0:
		default:  
			ch = 0xf0; 
			ch_read = 0xb8; 
			break;
		case 1: 
			ch = 0xe1; 
			ch_read = 0xb1; 
			break;
		case 2: 
			ch = 0xd2; 
			ch_read = 0xaa; 
			break;
		case 3: 
			ch = 0xc3; 
			ch_read = 0xa3; 
			break;
		case 4: 
			ch = 0xb4; 
			ch_read = 0x9c; 
			break;
		case 5: 
			ch = 0xa5; 
			ch_read = 0x95; 
			break;
		case 6: 
			ch = 0x96; 
			ch_read = 0x8e; 
			break;
		case 7: 
			ch = 0x87; 
			ch_read = 0x87; 
			break;
	};

	busyWait(true);
	begin();
	Wire.send(0xc3);
	Wire.send(ch);
	end();
	busyWait();
	
	uint8_t check = readByte();
	
	return check == ch_read;
}



bool DS2482::wireReset()
{
	busyWait(true);
	begin();
	Wire.send(0xb4);
	end();
	
	uint8_t status = busyWait();
	
	return status & DS2482_STATUS_PPD ? true : false;
}


void DS2482::wireWriteByte(uint8_t b)
{
	busyWait(true);
	begin();
	Wire.send(0xa5);
	Wire.send(b);
	end();
}

uint8_t DS2482::wireReadByte()
{
	busyWait(true);
	begin();
	Wire.send(0x96);
	end();
	busyWait();
	setReadPtr(PTR_READ);
	busyWait();
	return readByte();
}

void DS2482::wireWriteBit(uint8_t bit)
{
	busyWait(true);
	begin();
	Wire.send(0x87);
	Wire.send(bit ? 0x80 : 0);
	end();
}

uint8_t DS2482::wireReadBit()
{
	wireWriteBit(1);
	uint8_t status = busyWait(true);
	return status & DS2482_STATUS_SBR ? 1 : 0;
}

void DS2482::wireSkip()
{
	wireWriteByte(0xcc);
}

void DS2482::wireSelect(uint8_t rom[8])
{
	wireWriteByte(0x55);
	for (int i=0;i<8;i++)
		wireWriteByte(rom[i]);
}


#if ONEWIRE_SEARCH
void DS2482::wireResetSearch()
{
	searchExhausted = 0;
	// Initialize to negative value
	searchLastDisrepancy = -1;
	
	for(uint8_t i = 0; i<8; i++) 
		searchAddress[i] = 0;
}

uint8_t DS2482::wireSearch(uint8_t *newAddr)
{
	uint8_t i;
	uint8_t direction;
	uint8_t last_zero=0;
	
	if (searchExhausted) 
		return 0;
	
	if (!wireReset()) 
		return 0;

	busyWait(true);
	wireWriteByte(0xf0);
	
	for(i=0;i<64;i++) 
	{
		int romByte = i/8;
		int romBit = 1<<(i&7);
		
		if (i < searchLastDisrepancy)
			direction = searchAddress[romByte] & romBit;
		else
			direction = i == searchLastDisrepancy;
		
		busyWait();
		begin();
		Wire.send(0x78);
		Wire.send(direction ? 0x80 : 0);
		end();
		uint8_t status = busyWait();
		
		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;
		
		if (id && comp_id)
			return 0;
		else
		{ 
			if (!id && !comp_id && !direction)
				last_zero = i;
		}
		
		if (direction)
			searchAddress[romByte] |= romBit;
		else
			searchAddress[romByte] &= (uint8_t)~romBit;
	}

	searchLastDisrepancy = last_zero;

	if (last_zero == 0) 
		searchExhausted = 1;
	
	for (i=0;i<8;i++) 
		newAddr[i] = searchAddress[i];
	
	return 1;  
}
#endif

#if ONEWIRE_CRC
// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
//

uint8_t DS2482::crc8( uint8_t *addr, uint8_t len)
{
	uint8_t crc=0;
	
	for (uint8_t i=0; i<len;i++) 
	{
		uint8_t inbyte = addr[i];
		for (uint8_t j=0;j<8;j++) 
		{
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) 
				crc ^= 0x8C;
			
			inbyte >>= 1;
		}
	}
	return crc;
}

#endif
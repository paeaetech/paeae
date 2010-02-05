/*
  HTC595 library for Arduino
  Copyright (C) 2009-2010 Paeae Technologies
 
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
*/
#include <WProgram.h>
#include "htc595.h"


#define SHCP_TIME 2
#define STCP_TIME 2

HTC595::HTC595(uint8_t outpin,uint8_t sh_cp,uint8_t st_cp)
: mOut(outpin),
  mShcp(sh_cp),
  mStcp(st_cp)
{
	pinMode(mOut,OUTPUT);
	pinMode(mShcp,OUTPUT);
	pinMode(mStcp,OUTPUT);
	digitalWrite(mOut,LOW);
	digitalWrite(mShcp,LOW);
	digitalWrite(mStcp,LOW);
}


void HTC595::update(uint8_t* data)
{
	for (int i=HTC_NUM_CASCADES-1;i>=0;i--)
	{
		uint8_t b = data[i];
		
		for (int bi=0;bi<8;bi++)
		{
			uint8_t bit = (b >> bi) & 0x1;

			writeBit(bit);
		}
	}
	pulseStcp();
	
}


void HTC595::writeBit(uint8_t b)
{
	digitalWrite(mOut,b == 0 ? LOW : HIGH);
	
	pulseShcp();
	digitalWrite(mOut,LOW);
	
}

void HTC595::pulseShcp()
{
	digitalWrite(mShcp,HIGH);
//	delayMicroseconds(SHCP_TIME);
	digitalWrite(mShcp,LOW);
}

void HTC595::pulseStcp()
{
	digitalWrite(mStcp,HIGH);
//	delayMicroseconds(STCP_TIME);
	digitalWrite(mStcp,LOW);
}

void HTC595::setStcp(bool b)
{
	digitalWrite(mStcp,b ? HIGH : LOW);
}


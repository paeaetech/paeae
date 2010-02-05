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
#ifndef __HTC595_H__
#define __HTC595_H__


#include <inttypes.h>

#define SH_CP 7
#define ST_CP 6
#define HTC_OUT 5

#ifndef HTC_NUM_CASCADES
#define HTC_NUM_CASCADES 1
#endif

class HTC595
{
public:
	HTC595(uint8_t outpin=HTC_OUT,uint8_t sh_cp=SH_CP, uint8_t st_cp=ST_CP);

	void update(uint8_t* data);

protected:
	uint8_t mOut;
	uint8_t mShcp;
	uint8_t mStcp;

	void writeBit(uint8_t b);
	void pulseShcp();
	void pulseStcp();
	void setStcp(bool b);
	
};


#endif /* __HTC595_H__ */

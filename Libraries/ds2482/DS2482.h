/*
  DS2482 library for Arduino
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

#ifndef __DS2482_H__
#define __DS2482_H__

#include <inttypes.h>

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif


#define DS2482_CONFIG_APU (1<<0)
#define DS2482_CONFIG_PPM (1<<1)
#define DS2482_CONFIG_SPU (1<<2)
#define DS2484_CONFIG_WS  (1<<3)

#define DS2482_STATUS_BUSY 	(1<<0)
#define DS2482_STATUS_PPD 	(1<<1)
#define DS2482_STATUS_SD	(1<<2)
#define DS2482_STATUS_LL	(1<<3)
#define DS2482_STATUS_RST	(1<<4)
#define DS2482_STATUS_SBR	(1<<5)
#define DS2482_STATUS_TSB	(1<<6)
#define DS2482_STATUS_DIR	(1<<7)

class DS2482
{
public:
	//Address is 0-3
	DS2482(uint8_t address);
	
	bool configure(uint8_t config);
	void reset();
	
	//DS2482-800 only
	bool selectChannel(uint8_t channel);
	
	bool wireReset(); // return true if presence pulse is detected
	uint8_t wireReadStatus(bool setPtr=false);
	
	void wireWriteByte(uint8_t b);
	uint8_t wireReadByte();
	
	void wireWriteBit(uint8_t bit);
	uint8_t wireReadBit();
    // Issue a 1-Wire rom select command, you do the reset first.
    void wireSelect( uint8_t rom[8]);
	// Issue skip rom
	void wireSkip();
	
	uint8_t hasTimeout() { return mTimeout; }
#if ONEWIRE_SEARCH
    // Clear the search state so that if will start from the beginning again.
    void wireResetSearch();

    // Look for the next device. Returns 1 if a new address has been
    // returned. A zero might mean that the bus is shorted, there are
    // no devices, or you have already retrieved all of them.  It
    // might be a good idea to check the CRC to make sure you didn't
    // get garbage.  The order is deterministic. You will always get
    // the same devices in the same order.
    // The parameter 'cmd' can be used for devices supporting Conditional
    // Search 0xEC (e.g. DS2408)
	uint8_t wireSearch(uint8_t *newAddr, uint8_t cmd = 0xf0);
#endif
#if ONEWIRE_CRC
    // Compute a Dallas Semiconductor 8 bit CRC, these are used in the
    // ROM and scratchpad registers.
    static uint8_t crc8( uint8_t *addr, uint8_t len);

	// Compute a Dallas Semiconductor 16 bit CRC, these are used in the
	// scratchpad registers of some devices (e.g. DS2408).
	static uint16_t crc16( const uint8_t *data, uint8_t len);
#endif

private:
	
	uint8_t mAddress;
	uint8_t mTimeout;
	uint8_t readByte();
	void setReadPtr(uint8_t readPtr);
	
	uint8_t busyWait(bool setReadPtr=false); //blocks until
	void begin();
	void end();

#if ONEWIRE_CRC
	// Initializes the CRC16-table.
	static void initCrc16();
#endif
	
#if ONEWIRE_SEARCH
	uint8_t searchAddress[8];
	uint8_t searchLastDisrepancy;
	uint8_t searchExhausted;
#endif
	
};



#endif
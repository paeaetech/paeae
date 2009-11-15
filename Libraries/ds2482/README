
Arduino library for controlling DS2482-100 and DS2482-800 1-wire masters.

Uses Wire library to communicate with DS2482 so be sure to call Wire.begin() before using the library.

Quick example modeled after Onewire example:

#include 
#include

DS2482 ds(0);

void setup() 
{ 
	Wire.begin(); 
	ds.reset(); 
	//configure DS2482 to use active pull-up instead of pull-up resistor 
	//configure returns 0 if it cannot find DS2482 connected 
	//if (!ds.configure(DS2482_CONFIG_APU)) 
	//{ 
	//	 Serial.print("DS2482 not found\n"); 
	//} 
}

void loop() 
{ 
	byte addr[8];

	if ( !ds.wireSearch(addr)) 
	{ 
		//Serial.print("No more addresses.\n"); 
		ds.wireResetSearch(); 
		return; 
	}

}


#ifndef __HTC595_H__
#define __HTC595_H__


#include <inttypes.h>

#define SH_CP 7
#define ST_CP 6
#define HTC_OUT 5

#define HTC_NUM_CASCADES 4

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

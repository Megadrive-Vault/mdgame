#include "mpsg.h"

void psg_vol(unsigned char chan, unsigned char vol)
{
	volatile unsigned char *psg = (unsigned char *)0xC00011;
	*psg = 0x90 | ((chan & 0x03) << 5) | (vol & 0x0F);
}

void psg_pitch(unsigned char chan, unsigned int pitch)
{
	volatile unsigned char *psg = (unsigned char *)0xC00011;
	*psg = 0x80 | ((chan & 0x03) << 5) | (pitch & 0x0F);
	*psg = (pitch >> 4) & 0x3F;
}

void psg_tone(unsigned char chan, unsigned char vol, unsigned int pitch)
{	
	psg_pitch(chan,pitch);
	psg_vol(chan,vol);
}

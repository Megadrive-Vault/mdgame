#include <genesis.h>
#include "mpsg.h"
#include "mpad.h"
#include "mvdp.h"

const u16 octs[] =  {0,1,1, 1, 1, 1, 1,1,1,1,1,1 ,0,0, 1,1 ,0,0 ,0 ,1,1 ,0 ,0,0 ,0,0 ,0 ,0 ,0 ,0 ,0 ,0 ,13};
const u16 notes[] = {7,0,12,12,12,12,2,4,5,4,0,12,7,12,0,12,7,10,12,0,12,10,9,12,7,12,12,12,12,12,12,12,13};

const u16 oct2[] = {1,2,2,3,3,2,2,3,3,2,2,3,3,2,2,3,3,1,1,2,2,1,1,2,2,1,2,1,2,1,2,1,2,1,};
const u16 not2[] = {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,10,10,7,7,9,9,10,10,11,11};
int main(void)
{
	VDP_drawText("1   2", 3, 1);
	VDP_drawText("U",5,3);
	VDP_drawText("D",5,4);
	VDP_drawText("L",5,5);
	VDP_drawText("R",5,6);
	VDP_drawText("A",5,7);
	VDP_drawText("B",5,8);
	VDP_drawText("C",5,9);
	VDP_drawText("S",5,10);
	
	unsigned char i = 0;
	unsigned char kmax = 10;
	unsigned char k = kmax;
	unsigned char amp = 0;
	unsigned char amp2 = 0;
	int ucnt = 0;
	int dcnt = 0;
	
	int note = 0;
	
	while(1)
	{
		k--;
		if (k == 0)
		{
			note++;
			k = kmax;
		amp2 = 0;
		}
		amp++;
		amp2 = amp2 + 1;
		if (amp == 16)
		{
			amp = 15;
		}
		if (amp2 == 16)
		{
			amp2 = 15;
		}
		if (notes[note] == 13)
		{
			note = 0;
		}
		else if (notes[note] != 12)
		{
			psg_note(0,notes[note],octs[note]+2);
			amp = 0;
		}
		psg_note(1,not2[note],oct2[note]-1);
		psg_vol(0,amp);
		psg_vol(1,amp2);
		
		unsigned char padinfo = pad_read(PLAYER_1);
		unsigned char padinfo2 = pad_read(PLAYER_2);
		for (i = 8; i != 0; i--)
		{
			if ((padinfo >> (8-i)) & 0x01)
			{
				VDP_drawText(" ",3,4+7-i);
			}
			else
			{
				VDP_drawText("X",3,4+7-i);
			}
			if ((padinfo2 >> (8-i)) & 0x01)
			{
				VDP_drawText(" ",7,4+7-i);
			}
			else
			{
				VDP_drawText("X",7,4+7-i);
			}
		}
		
		if (padinfo & 0x01)
		{
			ucnt++;
		}
		else
		{
			ucnt = 0;
		}
		if (ucnt == 1)
		{
			kmax++;
		}
		if (padinfo & 0x02)
		{
			dcnt++;
		}
		else
		{
			dcnt = 0;
		}
		if (dcnt == 1)
		{
			kmax--;
		}
		
		
		VDP_updateSprites();
		VDP_waitVSync();
	}
	return (0);
}

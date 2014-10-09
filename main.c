#include <genesis.h>
#include "mpsg.h"
#include "mpad.h"
#include "mvdp.h"

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
	u16 pitch = 0;
	
	while(1)
	{
		pitch = pitch + 1;
		psg_tone(0,(pitch<<1)&0xF,0xFFFF - pitch);
		psg_tone(1,(pitch>>1)&0x0F,pitch + 128);
		psg_tone(2,(pitch / 3)&0x0F,pitch + 384);
		
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
		
		VDP_setSprite(0, pitch%320,((pitch / 320) % 224),
		SPRITE_SIZE(1,1),TILE_ATTR_FULL(PAL0,1,0,0,1),0);
		
		VDP_updateSprites();
		VDP_waitVSync();
		i++;
		if (i == 60)
		{
			i = 0;
		}
		if (i > 30)
		{
			
		}
	}
	return (0);
}

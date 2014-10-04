#include <genesis.h>


void psg_tone(u8 chan, u8 vol, u16 pitch)
{
	u8 *psg = (u8 *)0xC00011;
	*psg = 0x90 | ((chan & 0x03) << 5) | (vol & 0x0F);
	asm("nop");
	*psg = 0x80 | ((chan & 0x03) << 5) | (pitch & 0x0F);
	asm("nop");
	*psg = pitch >> 4 & 0x3F;
}

int main(void)
{
        VDP_drawText("Manual controller reads...", 1, 1);
		VDP_drawText("Up",5,3);
		VDP_drawText("Down",5,4);
		VDP_drawText("Left",5,5);
		VDP_drawText("Right",5,6);
		VDP_drawText("A",5,7);
		VDP_drawText("B",5,8);
		VDP_drawText("C",5,9);
		VDP_drawText("Start",5,10);

		u8 *ctrlr;
		ctrlr = (u8 *)0x00A10003;
		
		int k = 0;
		u16 pitch = 0;
		
		psg_tone(0,8,1400);
		
        while(1)
        {
		
			//psg_tone(1,7,pitch + 128);
			pitch = pitch + 1;
			psg_tone(3,0,0xFFFF - pitch / 2);
			psg_tone(0,(pitch<<1)&0xF,0xFFFF - pitch);
			psg_tone(1,(pitch>>1)&0x0F,pitch + 128);
			psg_tone(2,(pitch>>2)&0x0F,pitch + 384);
			// Set bit 7 to read 0SA00DU
			
			
			// Clr bit 7 to read 1CBRLDU
			*ctrlr = *ctrlr | (0x40);
			asm("nop");
			
			// Paul-quality test code
			
		
			/*
			for (k = 0; k < 8; k++)
			{
				if ((*ctrlr >> k) & 0x01)
				{
					VDP_drawText("1",16,3+k);
				}
				else
				{
					VDP_drawText("0",16,3+k);
				}
			}
			*/
			
			if (*ctrlr & 0x01) // U
			{
				VDP_drawText("U",3,3);
			}
			else
			{
				VDP_drawText(" ",3,3);
			}
			if (*ctrlr & 0x02) 
			{
				VDP_drawText("D",3,4);
			}
			else
			{
				VDP_drawText(" ",3,4);
			}
			if (*ctrlr & 0x04)
			{
				VDP_drawText("L",3,5);
			}
			else
			{
				VDP_drawText(" ",3,5);
			}
			if (*ctrlr & 0x08)
			{
				VDP_drawText("R",3,6);
			}
			else
			{
				VDP_drawText(" ",3,6);
			}
			
			if (*ctrlr & 0x10)
			{
				VDP_drawText("B",3,8);
			}
			else
			{
				VDP_drawText(" ",3,8);
			}
			if (*ctrlr & 0x20)
			{
				VDP_drawText("C",3,9);
			}
			else
			{
				VDP_drawText(" ",3,9);
			}
			
			
			
			*ctrlr = *ctrlr & ~(0x40);
			for (k = 0; k < 8; k++)
			{
			asm("nop");
			}
			/*
			for (k = 0; k < 8; k++)
			{
				if ((*ctrlr >> k) & 0x01)
				{
					VDP_drawText("1",18,3+k);
				}
				else
				{
					VDP_drawText("0",18,3+k);
				}
			}
			*/
			if (*ctrlr & 0x10)
			{
				VDP_drawText("A",3,7);
			}
			else
			{
				VDP_drawText(" ",3,7);
			}
			if (*ctrlr & 0x20)
			{
				VDP_drawText("S",3,10);
			}
			else
			{
				VDP_drawText(" ",3,10);
			}
			
                //read input
                //move sprite
                //update score
                //draw current screen (logo, start screen, settings, game, gameover, credits...)
			VDP_waitVSync();
        }
        return (0);
}

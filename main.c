#include <genesis.h>

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
		ctrlr = 0x00A10003;
		
		int i = 0;
		int k = 0;
		
        while(1)
        {
			// Set bit 7 to read 0SA00DU
			
			
			// Clr bit 7 to read 1CBRLDU
			*ctrlr = *ctrlr | (0x40);
			
			// Paul-quality test code
			
			
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
				if ((*ctrlr >> k) & 0x01)
				{
					VDP_drawText("1",18,3+k);
				}
				else
				{
					VDP_drawText("0",18,3+k);
				}
			}
			
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

#include "gamedata.h"

u8 map_collision(u16 x, u16 y)
{
	x = (x >> 3);
	y = (y >> 3);
	return map[y][x];
}

// For now, use the SGDK text function to render a "level"...
void ghetto_map_render()
{
	for (u8 y = 0; y < 28; y++)
	{
		for (u8 x = 0; x < 40; x++)
		{
			if (map[y][x] == MAP_SOLID)
			{
				VDP_drawText("#",x,y);
			}
		}
	}
}



void bg_dma_tiles(void)
{

	u16 pal1[] = {
		0x0000, 0x0240, 0x0282, 0x02E4,
		0x0248, 0x026A, 0x048C, 0x0EEE,
		0x0200, 0x0620, 0x0A60, 0x0EC2,
		0x0002, 0x0048, 0x02AE, 0x0000
	};
	
	u16 pal2[] = {
		0x0ECE, 0x04E0, 0x0240, 0x0882,
		0x0ECE, 0x04EE, 0x024C, 0x088E,
		0x0ECE, 0x04E0, 0x024E, 0x088E,
		0x0ECE, 0x04E0, 0x024E, 0x088E
	};
	
	// Write palettes
	VDP_doCRamDMA(pal1,0x0000,16);
	VDP_doCRamDMA(pal2,0x0020,16);

	u8 num_tiles = BG_NUM_TILES;
	u16 size = (16 * num_tiles);
	u16 *src = bg_tiles;
	VDP_doVRamDMA(src,BG_BASE_VRAM_INDEX,size);
}

void plot_map(void)
{
	VDP_clearPlan(VDP_PLAN_A,1);
	VDP_clearPlan(VDP_PLAN_B,1);
	
	for (u8 y = 0; y < 28; y++)
	{
		for (u8 x = 0; x < 40; x++)
		{
			VDP_setTileMapXY(VDP_PLAN_A,TILE_ATTR_FULL(fgmap[y][x]>>7,1,0,0,(BG_BASE_VRAM_INDEX/32) + (fgmap[y][x] & 0x7F)),x,y);
			VDP_setTileMapXY(VDP_PLAN_B,TILE_ATTR_FULL(bgmap[y][x]>>7,0,0,0,(BG_BASE_VRAM_INDEX/32) + (bgmap[y][x] & 0x7F)),x,y);
		}
	}
}


void load_maps(void)
{
	memcpy(&map, &default_map, sizeof(u8[32][40]));
	memcpy(&fgmap, &map_foreground, sizeof(u8[32][40]));
	memcpy(&bgmap, &map_background, sizeof(u8[32][40]));
}

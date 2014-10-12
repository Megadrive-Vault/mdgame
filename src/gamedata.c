#include "gamedata.h"

u8 map_collision(u16 x, u16 y)
{
	x = (x >> 3);
	y = (y >> 3);
	return map[y][x];
}

// For now, use the SGDK text function to render a "level"...

// Yah load das plahn
void load_font(u16 plan, u8 pal)
{
	if (plan != VDP_PLAN_A && plan != VDP_PLAN_B)
	{
		return;
	}
	
	// Font palette
	u16 fpal[] = {
		0x0000, 0x0240, 0x0282, 0x02E4,
		0x0204, 0x0468, 0x06AE, 0x0EEE,
		0x0600, 0x0A20, 0x0C60, 0x0EC2,
		0x0004, 0x004A, 0x028E, 0x0000
	};
	VDP_doCRamDMA(fpal,pal << 5,16);
	
	// Tile datum
	u8 num_tiles = 0x7F; // 128 characters in ASCII
	u16 size = (16 * num_tiles);
	VDP_doVRamDMA(font_tiles,(plan == VDP_PLAN_A ? FG_BASE_VRAM_INDEX : BG_BASE_VRAM_INDEX),size);
}

void bg_dma_tiles(void)
{

	u16 pal1[] = {
		0x0000, 0x0240, 0x0282, 0x02E4,
		0x0204, 0x0468, 0x06AE, 0x0EEE,
		0x0600, 0x0A20, 0x0C60, 0x0EC2,
		0x0004, 0x004A, 0x028E, 0x0000
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
	num_tiles = FG_NUM_TILES;
	size = (16 * num_tiles);
	src = fg_tiles;
	VDP_doVRamDMA(src,FG_BASE_VRAM_INDEX,size);
}

void plot_map(void)
{	
	VDP_clearPlan(VDP_PLAN_A,0);
	VDP_clearPlan(VDP_PLAN_B,0);
	
	for (u8 map_y = 0; map_y < 28; map_y++)
	{
		for (u8 map_x = 0; map_x < 40; map_x++)
		{
			VDP_setTileMapXY(VDP_PLAN_A,TILE_ATTR_FULL(fgmap[map_y][map_x]>>7,1,0,0,(FG_BASE_VRAM_INDEX/32) + (fgmap[map_y][map_x] & 0x7F)),map_x,map_y);
			VDP_setTileMapXY(VDP_PLAN_B,TILE_ATTR_FULL(bgmap[map_y][map_x]>>7,0,0,0,(BG_BASE_VRAM_INDEX/32) + (bgmap[map_y][map_x] & 0x7F)),map_x,map_y);
		}
	}
}


void load_maps(void)
{
	memcpy(&map, &default_map, sizeof(u8[32][40]));
	memcpy(&fgmap, &map_foreground, sizeof(u8[32][40]));
	memcpy(&bgmap, &map_background, sizeof(u8[32][40]));
}

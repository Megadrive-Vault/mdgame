#include "gamedata.h"

u8 map_collision(u16 x, u16 y)
{
	x = (x >> 3) % 40;
	y = (y >> 3) % 28;
	return collision_map[y][x];
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
		0x0000, 0x0240, 0x0482, 0x08E4,
		0x0204, 0x0468, 0x06AE, 0x0EEE,
		0x0600, 0x0A20, 0x0C60, 0x0EC2,
		0x0004, 0x004A, 0x028E, 0x0000
	};
	VDP_doCRamDMA(fpal,pal ? 0x0000 : 0x0020,16);
	
	// Tile datum
	u8 num_tiles = 0x7F; // 128 characters in ASCII
	u16 size = (16 * num_tiles);
	VDP_doVRamDMA(font_tiles,(plan == VDP_PLAN_A ? FG_BASE_VRAM_INDEX : BG_BASE_VRAM_INDEX),size);
}

void bg_dma_tiles()
{
	u16 pal1[] = {
		0x00C8, 0x0020, 0x0040, 0x0060,
		0x0204, 0x0468, 0x06AE, 0x0EEE,
		0x0600, 0x0A20, 0x0C60, 0x0EC2,
		0x0004, 0x004A, 0x028E, 0x0000
	};
	
	u16 pal2[] = {
		0x02A6, 0x0260, 0x0280, 0x02A0,
		0x02C0, 0x0264, 0x02A6, 0x02CA,
		0x0ECE, 0x04E0, 0x024E, 0x088E,
		0x0ECE, 0x04E0, 0x024E, 0x088E
	};
	

	u16 pal3[] = {
		0x0200, 0x0400, 0x0822, 0x0C88,
		0x0444, 0x0888, 0x0CCC, 0x0EEE,
		0x0404, 0x0808, 0x0C0C, 0x0E8E,
		0x0004, 0x006A, 0x02CE, 0x0000
	};
	
	u16 pal4[] = {
		0x0200, 0x0240, 0x0460, 0x0680,
		0x08A0, 0x0220, 0x0440, 0x0400,
		0x0ECE, 0x04E0, 0x024E, 0x088E,
		0x0ECE, 0x04E0, 0x024E, 0x088E
	};
	u8 num_tiles = BG_NUM_TILES;
	u16 size = (16 * num_tiles);
	u16 *src = bg_tiles;
	VDP_doVRamDMA(src,BG_BASE_VRAM_INDEX,size);
	num_tiles = FG_NUM_TILES;
	size = (16 * num_tiles);
	src = fg_tiles;
	VDP_doVRamDMA(src,FG_BASE_VRAM_INDEX,size);
	
	// Write palettes
	if (mode == 1)
	{
		VDP_doCRamDMA(pal1,0x0000,16);
		VDP_doCRamDMA(pal2,0x0020,16);
	}
	else
	{
		VDP_doCRamDMA(pal3,0x0000,16);
		VDP_doCRamDMA(pal4,0x0020,16);
	}
}
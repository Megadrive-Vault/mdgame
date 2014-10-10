#include "gamedata.h"

void read_pads(gamedata *data)
{
	data->pad1 = pad_read(0);
	data->pad2 = pad_read(1);
}

// For now, use the SGDK text function to render a "level"...
void ghetto_map_render()
{
	u8 y, x;
	for (y = 0; y < 28; y++)
	{
		for (x = 0; x < 40; x++)
		{
			if (map[y][x] == MAP_SOLID)
			{
				VDP_drawText("#",x,y);
			}
		}
	}
}
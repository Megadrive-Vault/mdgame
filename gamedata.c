#include "gamedata.h"

u8 map_collision(u16 x, u16 y)
{

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
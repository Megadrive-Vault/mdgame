#include <genesis.h>
#include "mpsg.h"
#include "mpad.h"
#include "gamedata.h"
#include "player.h"
#include "globals.h"

void gameloop(void)
{
	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 40; x++)
		{
			map[y][x] = 0;
	
			if (y == 27)
			{
				map[y][x] = 1;
			}
			
			if ((y == 18 || y == 19)&& x > 12 && x < 20)
			{
				map[y][x] = 1;
			}
			if (y == 24 && x > 5 && x < 10)
			{
				map[y][x] = 1;
			}
			
			if (y > 12 && x < 3)
			{
				map[y][x] = 1;
			}
			if (y > 12 && x > 37)
			{
				map[y][x] = 1;
			}
		}
	}
	
	int i = 0;
	
	player_init(&p1);
	player_init(&p2);
	
	p2.palette = 2;
	p2.sprite_num = 1;
	p2.tile_index = 3;
	ghetto_map_render();	

	while (1)
	{
		player_take_inputs(&p1,pad_read(0));
		player_take_inputs(&p2,pad_read(1));
		player_move(&p1);
		player_move(&p2);
		player_draw(&p1,0);
		player_draw(&p2,1);
		VDP_waitVSync();
	}
}

int main(void)
{
	VDP_init();
	gameloop();
	while(1)
	{
		VDP_waitVSync();
	}
	return (0);	
}

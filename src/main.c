#include <genesis.h>
#include "mpsg.h"
#include "mpad.h"
#include "gamedata.h"
#include "player.h"
#include "globals.h"
#include "echo.h"

// Resources
#include "sprite_tiles.h"
#include "music.h"
#include "instrument.h"

const void* const instrument_table[] =
  {
	instrument__square,
	instrument__saw,
	instrument__piano,
	0
  };

void gameloop(void)
{
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 40; x++)
		{
			
			map[y][x] = 0;
			if (y > 12 && (x > 37 || x < 3))
			{ 
				map[y][x] = 1;
			}
			if (y > 19 && x > 37)
			{
				map[y][x] = 0;
			}
	
			if (y == 27 && x > 8)
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
			if (y == 31)
			{
				map[y][x] = 1;
			}
		}
	}
	
	VDP_drawText("wot u think ur doin on my lawn",8,25);
	VDP_drawText("u get of my fekin lawn u kids",4,26);
	
	VDP_drawText("WELCOME TO SUPER DMA LAND",9,1);
	
	player_init(&p1);
	player_init(&p2);
	
	p1.palette = 2;
	p2.palette = 3;
	p2.sprite_num = 1;
	p2.player_num = 1;
	p1.tile_offset = 1;
	p2.tile_offset = 1;
	ghetto_map_render();
	
	// Load the player tiles
	VDP_doVRamDMA(sprite_tiles,0x0000,16*256);
	
	u8 nothing[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	VDP_doVRamDMA(&nothing,0x0000,32);
	
	int i = 0;
	// Initialize the sound engine
	echo_init(instrument_table);
	echo_play_bgm(&music__example_track);
	while (1)
	{
		i++;
		if (i % 6 == 0)
		{
			p1.tile_offset++;
			p2.tile_offset++;
			if (p1.tile_offset == 10)
			{
				p1.tile_offset = 2;
				p2.tile_offset = 2;
			}
		}
		p1.sprite_num = i % 2;
		p2.sprite_num = (i + 1) % 2;
		player_take_inputs(&p1,pad_read(0));
		player_take_inputs(&p2,pad_read(1));
		player_move(&p1);
		player_move(&p2);
		// Shows CPU usage, heh
		VDP_waitVSync();
		player_draw(&p1);
		player_draw(&p2);
		player_dma_pal(&p1);
		player_dma_pal(&p2);
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

#include <genesis.h>
#include "mpsg.h"
#include "mpad.h"
#include "gamedata.h"
#include "player.h"
#include "globals.h"
#include "echo.h"
#include "enemy.h"

// Resources
#include "music.h"
#include "instrument.h"
#include "map.h"

const void* const instrument_table[] =
{
	instrument__chord,
	instrument__bass,
	instrument__lead,
	0
};


void gameloop(void)
{
	load_maps();
	plot_map();
	
	player_init(&p1);
	player_init(&p2);
	
	p1.palette = 2;
	p2.palette = 3;
	p1.other = &p2;
	p2.other = &p1;
	p2.sprite_num = 1;
	p2.player_num = 1;
	p2.x = 512;
	
	int i = 0;
	// Initialize the sound engine
	echo_init(instrument_table);
	echo_play_bgm(&music__bgm);

	enemy e;
	enemy_spawn(&e);
	
	player *player_a = &p1;
	player *player_b = &p2;
	
	bg_dma_tiles();
	load_font(VDP_PLAN_B,0);
	while (1)
	{
		i++;
		player_a = (i & 0x01) ? &p1 : &p2;
		player_b = (i & 0x01) ? &p2 : &p1;
		p1.sprite_num = i % 2;
		p2.sprite_num = (i + 1) % 2;
		player_take_inputs(player_a,pad_read((i & 0x01) ? 0 : 1));
		player_take_inputs(player_b,pad_read((i & 0x01) ? 1 : 0));
		player_move(player_a);
		player_move(player_b);
		enemy_update(&e);
		player_collide(player_a);
		player_collide(player_b);
		player_slap(player_a);
		player_slap(player_b);
		player_animate(player_a);
		player_animate(player_b);
		VDP_waitVSync();
		player_dma_tiles(player_a);
		player_dma_tiles(player_b);
		player_dma_pal(player_a);
		player_dma_pal(player_b);
		enemy_dma_tiles(&e);
		player_draw(player_a);
		player_draw(player_b);
		enemy_draw(&e);
		// Enemy update section
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

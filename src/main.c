#include "includes.h"

// Static function prototypes //
static void intro(void);
static void title_screen(void);
static void menu(void);
static void game(void);
static void scoreboard(void);

// Static variables //
static game_map_set default_map_set =
{
	&default_collision_map,
	&default_foreground_map,
	&default_background_map
};

const u8 const *default_instrument_set[] =
{
	(u8*)instrument_chord,
	(u8*)instrument_bass,
	(u8*)instrument_lead,
	0
};

static void init()
{
	// Initialize the VDP: reset registers, clear VRAM, set default palettes.
	VDP_init();
	
	// Initialize the sound engine
	echo_init(default_instrument_set);
	echo_play_bgm(&music__bgm); // TODO: Possibly adjust this convention just like instrument.
}

static void intro(void)
{
	// TODO: Create intro.
	title_screen();
}

static void title_screen(void)
{
	// TODO: Create title screen.
	menu();
}

static void menu(void)
{
	
	// TODO: Create menu. 
	game();
}

static void game(void)
{
	// TODO: Clean up game function.
	map_init(&default_map_set);
	
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

	enemy e;
	enemy_spawn(&e);
	
	player *player_a = &p1;
	player *player_b = &p2;
	
	bg_dma_tiles();
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

static void scoreboard(void)
{
	// TODO: Create score board.
}

// Suggested states: intro, title, menu, game, scoreboard

int main(void)
{
	// Generic initialization
	init();
	// Start at the intro.
	intro();
	return 0;
}

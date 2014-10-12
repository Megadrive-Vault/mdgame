#include "includes.h"

#define STATE_INTRO 0
#define STATE_TITLE 1
#define STATE_MENU 2
#define STATE_GAME 3
#define STATE_POST 4

u8 state;
// Static function prototypes //
static void main_loop(void);
static void intro(void);
static void title_screen(void);
static u8 menu(void);
static void game(void);
static void scoreboard(void);

static void main_loop(void)
{
	u8 result = 0;
	while (1)
	{
		switch (state)
		{
			case STATE_INTRO:	
				intro();
				state = STATE_TITLE;
				break;
			case STATE_TITLE:
				title_screen();
				state = STATE_MENU;
				break;
			case STATE_MENU:
				result = menu();
				if (result == 0)
				{
					state = STATE_INTRO;
				}
				else
				{
					state = STATE_GAME;
				}
				break;
			case STATE_GAME:
				game();
				state = STATE_POST;
				break;
			case STATE_POST:
				scoreboard();
				state = STATE_INTRO;
				break;
		}
	}
}	

// Static variables //
static game_map_set default_map_set =
{
	&default_collision_map,
	&default_foreground_map,
	&default_background_map
};

static game_map_set results_map_set =
{
	&default_collision_map,
	&results_foreground_map,
	&results_background_map
	
};

const u8 const *default_instrument_set[] =
{
	(u8*)instrument_chord,
	(u8*)instrument_bass,
	(u8*)instrument_lead,
	(u8*)instrument_bell,
	0
};

static void init()
{
	state = 0;
	// Initialize the VDP: reset registers, clear VRAM, set default palettes.
	VDP_init();
}

static void intro(void)
{
	// TODO: Create intro.
}

static void title_screen(void)
{
	// TODO: Create title screen.
}

static u8 menu(void)
{
	return 1;
}

static void game(void)
{
	// TODO: Clean up game function.
	VDP_clearPlan(VDP_PLAN_A,0);
	VDP_clearPlan(VDP_PLAN_B,0);
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
	
	u8 framecnt = 60;
	
	u16 timer = 60 * 120;

	enemy_t e;
	e.state = 0;

	player *player_a = &p1;
	player *player_b = &p2;

	
	bg_dma_tiles();
	timer = 0;
	u16 spawn_cnt = 0;
	
	// Initialize the sound engine
	echo_init(default_instrument_set);
	echo_play_bgm(&music__bgm); // TODO: Possibly adjust this convention just like instrument.
	
	while (timer < (60 * 60 * 2))
	{
		spawn_cnt++;
		if (spawn_cnt == ENEMY_SPAWN_TIME)
		{
			if ( e.state != 1 && e.state != 2)
			{
				enemy_spawn(&e, 2, (player_a->button_count + player_b->button_count));
			}
			spawn_cnt = 0;
		}
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
		enemy_check_players(&e, player_a);
		enemy_check_players(&e, player_b);
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
		timer++;
		// Enemy update section
	}
	
	e.x = -128;
	enemy_draw(&e);
	timer = 0;
	
	VDP_clearPlan(VDP_PLAN_A,0);
	VDP_clearPlan(VDP_PLAN_B,0);
	
	load_font(VDP_PLAN_B,0);
	
	while (timer < 60)
	{
		timer++;
		VDP_waitVSync();
	}
	VDP_waitVSync();
	timer = 0;
	map_init(&results_map_set);
	p1.x = 24 * 16;
	p2.x = 24 * 16;
	if (p1.total_slaps >= p2.total_slaps)
	{
		p1.y = 13 * 16;
		p2.y = 17 * 16;
	}
	else
	{
		p2.y = 13 * 16;
		p1.y = 17 * 16;
	}
	player_draw(player_a);
	player_draw(player_b);
	u8 pads[2];
	pads[0] = 0xFF;
	pads[1] = 0xFF;
	echo_stop_bgm();
	while ((pads[0] & KEY_START) && (pads[1] & KEY_START))
	{
		pads[0] = pad_read(0);
		pads[1] = pad_read(1);
	}
	
	// The game is over
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
	main_loop();
	
	while (1)
	{
		VDP_waitVSync();
	}
	return 0;
}

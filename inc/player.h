#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>
#include "mpad.h"
#include "gamedata.h"
#include "mpsg.h"
#include "globals.h"

#include "player_sprites.h"

#define PLAYER_NUM_SPRITES 27

#define PLAYER_PAD0 0
#define PLAYER_PAD1 1

#define PLAYER_GROUNDED 1
#define PLAYER_AIRBORN 0

// Time constants for how long actions take before happening
#define PLAYER_SLAPTIME 14
#define PLAYER_SLAP_COOLDOWNTIME 12
#define PLAYER_SLAP_THRESHHOLD PLAYER_SLAP_COOLDOWNTIME - 4
#define PLAYER_DASHTIME 22



// Physics "resolution"
#define PLAYER_RESOLUTION 	1
// Physics constants
#define PLAYER_MAX_DX 		4
#define PLAYER_MAX_DY 		15
#define PLAYER_ACCEL 		1
#define PLAYER_DECEL 		1
#define PLAYER_JUMPSTR		9
#define PLAYER_HIGRAV 		1
#define PLAYER_LOWGRAV 		1

#define PLAYER_DASH_THRUST_X 8
#define PLAYER_DASH_THRUST_Y 7

#define PLAYER_DECEL_WAIT 	2
#define PLAYER_ACCEL_WAIT 	0
#define PLAYER_HIGRAV_WAIT  1
#define PLAYER_LOWGRAV_WAIT	2

#define PLAYER_TILE_WIDTH 	4
#define PLAYER_TILE_HEIGHT 	4

// Animation cell offsets
#define PLAYER_ANIM_OFF_STAND 0
#define PLAYER_ANIM_OFF_WALK 3
#define PLAYER_ANIM_OFF_RUN 9
#define PLAYER_ANIM_OFF_JUMP 17
#define PLAYER_ANIM_OFF_FALL 20
#define PLAYER_ANIM_OFF_PRESLAP 24
#define PLAYER_ANIM_OFF_POSTSLAP 25

#define PLAYER_ANIM_STAND 9
#define PLAYER_ANIM_WALK 1
#define PLAYER_ANIM_RUN 2
#define PLAYER_ANIM_JUMP 3
#define PLAYER_ANIM_FALL 4
#define PLAYER_ANIM_PRESLAP 5
#define PLAYER_ANIM_POSTSLAP 6
#define PLAYER_ANIM_DASH 7
#define PLAYER_ANIM_HURT 8

// Hitbox information
#define PLAYER_X1 -6
#define PLAYER_X2 6
#define PLAYER_Y1 0
#define PLAYER_Y2 16
#define PLAYER_Y3 8 // midpoint

// Added to width and height to give extra slap tolerance
#define PLAYER_SLAP_W 4
#define PLAYER_SLAP_H 6

// Horizontal wrapping boundaries
#define PLAYER_MINX 128 + PLAYER_X1
#define PLAYER_MAXX 128 + PLAYER_X2

typedef struct player player;
struct player 
{
	player *other;

	// Palettes; last 8 colors used by other sprites (enemies, etc)
	u16 normal_pal1[16]; // P1
	u16 normal_pal2[16]; // P2
	u16 light_pal1[16];
	u16 light_pal2[16];
	u16 white_pal[16];
	u8 hitstun; // Player is knocked around whiel hurt, can't press buttons
	u8 flash; // Invincibility after damage?
	u8 hitfreeze; 
	
	u8 current_anim;
	u8 anim_cnt_stand;
	u8 anim_cnt_run;
	u8 anim_cnt_jump;
	u8 anim_cnt_fall;


	u8 input_id; // Who controls this sprite - 0 for P1, 1 for P2
	u8 vis; // Is the player to be rendered?
	u8 priority; // Above or below playfield backdrop graphics
	u8 osc; // Just counts up all the time. May be used to resolve contention
	
	u16 tile_index; // Base VRAM address for graphics
	u16 tile_offset; // Which animation cell to use (4x4 chunks)
	u8 player_num; 
	u16 sprite_num; // Which sprite number to render with - oscillates every frame. 
	                // Also used to resolve contention
	u8 palette; // Usually corresponds with player number
	
	// Physics variables:
 	u16 x; 
	u16 y;
	int dx;
	int dy;
	u8 grounded; // Non-zero = not in the air
	u8 direction; // 0 is facing right, 1 is facing left
	
	// Used to fake higher precision for physics
	u8 higrav_wait; 
	u8 lowgrav_wait;
	u8 decel_wait;
	u8 accel_wait;
	
	u8 pad_data; // Last gamepad readout
	u8 jump_key; // 0 - not pushed; 1 - just pushed; 2 - held
	
	// Decrementing counters (activated when the value == 1, decrementing)
	// Time is processed in frames (1/60s in NTSC, 1/50s in PAL)
	u8 slapcnt; // Press A to start the countdown - early release cancels
	u8 slapcooldown; // Helplessness after an attack before player can be active
	u8 dashcooldown; // Cooldown from a dash
	u8 slapok;
	u8 dashok; 
};

player p1;
player p2;

void player_dma_tiles(void);
void player_init(player *pl);
void player_dma_pal(player *pl);
void player_take_inputs(player *pl, u8 pad_data); // Affect physics variables based on input
void player_move(player *pl); // Run movement routine based on physics info
void player_draw(player *pl);
void player_animate(player *pl);
void player_collide(player *pl);

// Support functions for the above
void player_counters(player *pl);
void player_calc_animation(player *pl);
void player_dash_vectors(player *pl);
void player_ground(player *pl);
void player_gravity(player *pl);
void player_vis(player *pl);
void player_slap(player *pl);
void player_dash(player *pl);
u8 player_pos_dy(player *pl);
u8 player_neg_dy(player *pl);
u8 player_pos_dx(player *pl);
u8 player_neg_dx(player *pl);

#endif

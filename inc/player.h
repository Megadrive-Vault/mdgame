#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>
#include "mpad.h"
#include "gamedata.h"
#include "mpsg.h"
#include "globals.h"

#define PLAYER_PAD0 0
#define PLAYER_PAD1 1

#define PLAYER_GROUNDED 1
#define PLAYER_AIRBORN 0

// Time constants for how long actions take before happening
#define PLAYER_SLAPTIME 35
#define PLAYER_DASHTIME 17



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
#define PLAYER_HIGRAV_WAIT  0
#define PLAYER_LOWGRAV_WAIT	2

#define PLAYER_TILE_WIDTH 	2
#define PLAYER_TILE_HEIGHT 	3

/*
Player collision points relative to (center,bottom) in (x,y)
Twelve collision points total, 3 per direction.

	Top points:
	(x+x1,y+y1-1) , (x,y+y1-1) , (x+x2, y+y1-1)
	
	Bottom points:
	(x+x1,y+y2+1) , (x,y+y2+1), (x+x2,y+y2+1)
	
	Left points:
	(x+x1-1,y+y1) , (x+x1-1,y) , (x+x1-1, y+y2)
	
	Right points:
	(x+x2+1,y+y1) , (x+x2+1,y) , (x+x2+1, y+y2)
	
*/

// Hitbox information
#define PLAYER_X1 -6
#define PLAYER_X2 6
#define PLAYER_Y1 0
#define PLAYER_Y3 8
#define PLAYER_Y2 16


// Horizontal wrapping boundaries
#define PLAYER_MINX 128 + PLAYER_X1
#define PLAYER_MAXX 128 + PLAYER_X2

typedef struct player player;
struct player 
{
	u16 sprite_num;
	u16 tile_index;
	u8 palette;
	u8 direction; // 0 is facing right, 1 is facing left
	
	// Physics variables:
	// Position is processed in a virtual 2560x1792 space then >> 3 (div by 8)
	// for rendering and collisions. This allows the player to move at minimum
	// 0.125 pixels per frame for more granularity without floating point
 	u16 x; 
	u16 y;
	int dx;
	int dy;
	u8 grounded; // Non-zero = not in the air
	u8 jump_key; // 0 - not pushed; 1 - just pushed; 2 - held
	
	u8 higrav_wait;
	u8 lowgrav_wait;
	u8 decel_wait;
	u8 accel_wait;
	
	u8 priority;
	
	u8 input_id; // Who controls this sprite - 0 for P1, 1 for P2
	
	// Decrementing counters (activated when the value == 1, decrementing)
	// Time is processed in frames (1/60s in NTSC, 1/50s in PAL)
	u8 slapcnt; // Press A to start the countdown - early release cancels
	
	u8 vis;
	
	u8 slapcooldown; // Helplessness after an attack before player can be active
	u8 dashcooldown; // Cooldown from a dash
	u8 slapok;
	u8 dashok; 
};

player p1;
player p2;

void player_init(player *pl);
void player_take_inputs(player *pl, u8 pad_data); // Affect physics variables based on input
void player_move(player *pl); // Run movement routine based on physics info
void player_draw(player *pl);

// Support functions for the above
void player_dash_vectors(player *pl, u8 pad_data);
void player_ground(player *pl);
void player_gravity(player *pl);
void player_vis(player *pl);
void player_dash(player *pl);
u8 player_pos_dy(player *pl);
u8 player_neg_dy(player *pl);
u8 player_pos_dx(player *pl);
u8 player_neg_dx(player *pl);

#endif

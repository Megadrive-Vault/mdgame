#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>
#include "ids.h"
#include "mvdp.h"
#include "globals.h"

#define PLAYER_PAD0 0
#define PLAYER_PAD1 1

// Time constants for how long actions take before happening
#define PLAYER_SLAPTIME 35
#define PLAYER_GROUNDED 1
#define PLAYER_AIRBORN 0
#define PLAYER_HIGRAV 5
#define PLAYER_LOWGRAV 2

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
#define PLAYER_X1 -6
#define PLAYER_X2 6
#define PLAYER_Y1 -20
#define PLAYER_Y2 0

// Horizontal wrapping boundaries
#define PLAYER_MINX 128 + PLAYER_X1
#define PLAYER_MAXX 128 + PLAYER_X2

typedef struct player player;
struct player 
{
	u16 sprite_num;
	
	// Physics variables:
	// Position is processed in a virtual 2560x1792 space then >> 3 (div by 8)
	// for rendering and collisions. This allows the player to move at minimum
	// 0.125 pixels per frame for more granularity without floating point
 	u16 x; 
	u16 y;
	char dx;
	char dy;
	u8 grounded; // Non-zero = not in the air
	
	int **level_data; // Pointer to the level data
	
	u8 input_id; // Who controls this sprite - 0 for P1, 1 for P2
	
	// Decrementing counters (activated when the value == 1, decrementing)
	// Time is processed in frames (1/60s in NTSC, 1/50s in PAL)
	u8 slapcnt; // Press A to start the countdown - early release cancels
	u8 slapcooldown; // Helplessness after an attack before player can be active
	u8 dashcooldown; // Cooldown from a dash
};


player p1;
player p2;

void player_take_inputs(player *pl); // Affect physics variables based on input
void player_move(player *pl); // Run movement routine based on physics info
void player_draw(player *pl, int sprite_num);

#endif

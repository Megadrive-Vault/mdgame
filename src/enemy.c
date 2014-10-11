#include "enemy.h"
#include "player.h"
#include "curves.h"

typedef struct point { u16 x; u16 y; } point;

static _direction = 1;

void enemy_spawn(enemy *e)
{
	// TODO: Implement some pseudo-random initial (x, y) selector
	e->x = 150;
	e->y = 100;
	// Direction alternates between spawned enemies
	e->direction = (_direction = !_direction);
	e->phase = 0;
}

void enemy_update(enemy *e)
{
	u16 x_offset = curve_a[e->phase].x;
	u16 y_offset = curve_a[e->phase].y;
	// Update X
	if(e->direction)
	{
		// Flying toward the left
		e->x -= x_offset;
	} else {
		// Flying toward the right
		e->x += x_offset;
	}
	// Update Y
	if(e->flip_curve)
	{
		// Flying toward the left
		e->y -= y_offset;
	} else {
		// Flying toward the right
		e->y += y_offset;
	}	
	
	// Update phase index
	if(e->phase >= curve_a_len - 1)
	{
		// Reset to the beginning of the phase sequence.
		e->phase = 0;
		// Alternate curve direction
		e->flip_curve = !e->flip_curve;
	} else {
		// Proceed to the next phase in the phase sequence.
		e->phase += 1;
	}
	enemy_draw(e);
}

void enemy_draw(enemy *e)
{

	u8 palette = 0;
	u8 priority = 1;
	u8 tile_index = 0;
	u8 tile_offset = 0;
	
	VDP_setSpriteDirect(0,
						(e->x >> PLAYER_RESOLUTION) + PLAYER_X1 - 9 + (16 - (4*PLAYER_TILE_WIDTH)),
						(e->y >> PLAYER_RESOLUTION) - ((8*PLAYER_TILE_HEIGHT) - PLAYER_Y2),
						SPRITE_SIZE(PLAYER_TILE_WIDTH, PLAYER_TILE_HEIGHT),
						TILE_ATTR_FULL(palette,priority,0,e->direction,tile_index + (tile_offset * (PLAYER_TILE_WIDTH*PLAYER_TILE_HEIGHT))),
						1);
}

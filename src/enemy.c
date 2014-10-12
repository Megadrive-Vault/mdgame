#include "enemy.h"
#include "curves.h"

//typedef struct point { u16 x; u16 y; } point;

static _direction = 1;

void enemy_spawn(enemy *e)
{
	// TODO: Implement some pseudo-random initial (x, y) selector
	e->x = 150;
	e->y = 100;
	// Direction alternates between spawned enemies
	e->direction = (_direction = !_direction);
	e->phase = 0;
	e->anim_frame = 1;
	e->anim_cnt = 0;
	e->state = 0;
	e->type = 0;
	e->sprite_num = 2;
	e->enemy_num = 1;
	e->curve = curve_a;
	e->curve_len = curve_a_len;
}

void enemy_update(enemy *e)
{
	u16 x_offset = e->curve[e->phase].x;
	u16 y_offset = e->curve[e->phase].y;
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
	if(e->phase >= e->curve_len - 1)
	{
		// Reset to the beginning of the phase sequence.
		e->phase = 0;
		// Alternate curve direction
		e->flip_curve = !e->flip_curve;
		if(e->x > 150 && e->x < 300) {
			e->curve = curve_a;
			e->curve_len = curve_a_len;
		} else {
			e->curve = curve_b;
			e->curve_len = curve_b_len;
		}
		if(e->x > (320 << PLAYER_RESOLUTION)) {
			e->direction = 1;
		} else if (e->x < -40) {
			e->direction = 0;
		}
	} else {
		// Proceed to the next phase in the phase sequence.
		e->phase += 1;
	};
}

void enemy_dma_tiles(enemy *e)
{
	// Load the player tiles
	u8 num_tiles = ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT;
	u16 size = (16 * num_tiles);
	u16 *src = enemy_sprites + (32 * ((ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT) * (8*e->type) + (ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT) * (e->anim_frame)));
	VDP_doVRamDMA(src,ENEMY_BASE_VRAM_INDEX + (2 * size * e->enemy_num),size);
}

void enemy_draw(enemy *e)
{
	int tile_addr = (ENEMY_BASE_VRAM_INDEX / 32)+((ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT) * e->enemy_num);
	VDP_setSpriteDirect(e->sprite_num,
		(e->x >> PLAYER_RESOLUTION) + (16 - (4*ENEMY_TILE_WIDTH)),
		(e->y >> PLAYER_RESOLUTION) - ((8*ENEMY_TILE_HEIGHT)),
		SPRITE_SIZE(ENEMY_TILE_WIDTH, ENEMY_TILE_HEIGHT),
		TILE_ATTR_FULL((e->type & 0x01) + 2,0,0,e->direction,tile_addr),
		e->sprite_num +1);
}

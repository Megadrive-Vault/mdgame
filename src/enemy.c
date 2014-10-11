#include "enemy.h"
#include "player.h"

// For now, there is only one movement phase sequence.
/* static u8 _phases[] = */
/* { */
/* 	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 40, 40, 40, 40, 40, 40, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 */
/* }; */

typedef struct point { u16 x; u16 y; } point;

static point _phases[] = 
{
	{0, 0}, {2, 2}, {3, 3}, {5, 5}, {7, 6}, {8, 8}, {10, 9}, {12, 10}, {13, 12}, {15, 13}, {17, 14}, {18, 15}, {20, 16}, {22, 17}, {23, 18}, {25, 19}, {27, 20}, {28, 20}, {30, 21}, {32, 22}, {33, 22}, {35, 23}, {37, 23}, {38, 24}, {40, 24}, {42, 24}, {43, 25}, {45, 25}, {47, 25}, {48, 25}, {50, 25}, {52, 25}, {53, 25}, {55, 25}, {57, 25}, {58, 24}, {60, 24}, {62, 24}, {63, 23}, {65, 23}, {67, 22}, {68, 22}, {70, 21}, {72, 20}, {73, 20}, {75, 19}, {77, 18}, {78, 17}, {80, 16}, {82, 15}, {83, 14}, {85, 13}, {87, 12}, {88, 10}, {90, 9}, {92, 8}, {93, 6}, {95, 5}, {97, 3}, {98, 2}, {100, 0}, 
};

static _direction = 1;

void enemy_spawn(enemy *e)
{
	// TODO: Implement some pseudo-random initial (x, y) selector
	e->orig_x = (e->x = 150);
	e->orig_y = (e->y = 100);
	// Direction alternates between spawned enemies
	e->direction = (_direction = !_direction);
	e->phase = 0;
	e->anim_frame = 1;
	e->anim_cnt = 0;
	e->state = 0;
	e->type = 0;
	e->sprite_num = 2;
	e->enemy_num = 1;
}

void enemy_update(enemy *e)
{
	u16 x_offset = _phases[e->phase].x;
	u16 y_offset = _phases[e->phase].y;
	// Update X
	if(e->direction)
	{
		// Flying toward the left
		e->x = e->orig_x - x_offset;
	} else {
		// Flying toward the right
		e->x = e->orig_x + x_offset;
	}
	// Update Y
	e->y = e->orig_y + y_offset;
	
	// Update phase index
	if(e->phase >= sizeof(_phases) / sizeof(u8)) {
		// Reset to the beginning of the phase sequence.
		e->phase = 0;
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

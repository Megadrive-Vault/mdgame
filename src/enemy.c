#include "enemy.h"
#include "curves.h"


void enemy_check_players(enemy_t *e, player *pl)
{
	if (e->state != 0)
	{
		if ((e->x >> PLAYER_RESOLUTION) + 12 < (pl->x >> PLAYER_RESOLUTION) + PLAYER_X1)
		{
		
		}
		else if ((e->x >> PLAYER_RESOLUTION) - 12 > (pl->x >> PLAYER_RESOLUTION) + PLAYER_X2)
		{
		
		}
		else if ((e->y >> PLAYER_RESOLUTION) + 12< (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y1)
		{
		
		}
		else if ((e->y >> PLAYER_RESOLUTION) - 12 > (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2)
		{
		
		}
		else
		{
			if (e->state == 1 && pl->slapcooldown > PLAYER_SLAP_THRESHHOLD)
			{
				enemy_get_hit(e,pl);
			}
			else if (e->state == 2 && pl->flash == 0)
			{
				enemy_hit_player(e,pl);
			}
		}
	}
}


void enemy_get_hit(enemy_t *e, player *pl)
{
	e->state = 2;
	e->dx = (pl->direction ? -2 : 2) + (pl->dx / 2);
	if ((pl->direction == 0 && !(pl->pad_data & KEY_RIGHT)) || 
		(pl->direction == 1 && !(pl->pad_data & KEY_LEFT)))
	{
		e->dx += (pl->direction ? -4 : 4);
	}
	int magx = pl->dx;
	if (magx < 0)
	{
		magx = magx * -1;
	}
	if (!(pl->pad_data & KEY_UP))
	{
		e->dy = -12;
	}
	else if (!(pl->pad_data & KEY_DOWN))
	{
		e->dy += 9;
	}
	else
	{
		e->dy = -5;
	}
	pl->dy = pl->dy >> 1;
	pl->flash = 16;
}

void enemy_hit_player(enemy_t *e, player *pl)
{
	pl->other->total_slaps+= 3;
	pl->slapcooldown = 0;
	pl->slapcnt = 0;
	pl->other->hitfreeze = 3;
	pl->hitfreeze = 3;
	e->direction = (e->dx > 0) ? 0 : 1;
	pl->dx = (e->direction ? -5 : 5) + (pl->dx / 2);
	
	if (pl->grounded)
	{
		pl->dy = -2;
	}
	else
	{
		pl->dy = -9;
	}
	pl->hitstun = 40;
	
	pl->flash = 30;
}

void enemy_spawn(enemy_t *e, u8 num, u8 seed)
{
	// TODO: Implement some pseudo-random initial (x, y) selector
	switch ((seed / 3) % 4)
	{
		case 0:
			e->x = 0;
			e->direction = 0;
			e->y = 96 << PLAYER_RESOLUTION;
			break;
		case 1:
			e->x = 0;
			e->direction = 0;
			e->y = 64 << PLAYER_RESOLUTION;
			break;
		case 2:
			e->x = 320 << PLAYER_RESOLUTION;
			e->direction = 1;
			e->y = 96 << PLAYER_RESOLUTION;
			break;
		case 3:
			e->x = 320 << PLAYER_RESOLUTION;
			e->direction = 1;
			e->y = 64 << PLAYER_RESOLUTION;
			break;
	}
	e->dx = 0;
	e->dy = 0;
	// Direction alternates between spawned enemies
	e->phase = 0;
	e->anim_frame = 0;
	e->anim_cnt = 0;
	e->state = 1;
	e->type = seed % 3;
	e->sprite_num = num;
	e->enemy_num = 1;
	e->curve = (seed >> 1 & 0x01) ? curve_a : curve_b;
	e->curve_len = curve_a_len;
	e->fly_count = 0;
	e->flip_curve = 0;
}

void enemy_update(enemy_t *e)
{
	if (e->state == 1)
	{
		u16 x_offset = e->curve[e->phase].x;
		u16 y_offset = e->curve[e->phase].y;
		// Update X
		if(e->direction)
		{
			// Flying toward the left
			e->x -= x_offset;
		} 
		else 
		{
			// Flying toward the right
			e->x += x_offset;
		}
		// Update Y
		if(e->flip_curve)
		{
			// Flying toward the left
			e->y -= y_offset;
		} 
		else 
		{
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
			if (e->x > 150 && e->x < 300) 
			{
				e->curve = curve_a;
				e->curve_len = curve_a_len;
			} 
			else 
			{
				e->curve = curve_b;
				e->curve_len = curve_b_len;
			}
			if (e->direction == 0 && e->x > (320 << PLAYER_RESOLUTION)) 
			{
				e->direction = 1;
				e->fly_count++;
			} 
			else if (e->direction == 1 && e->x < -40) 
			{
				e->direction = 0;
				e->fly_count++;
			}
		} 
		else 
		{
			// Proceed to the next phase in the phase sequence.
			e->phase += 1;
		}
	}
	else if (e->state == 2)
	{
		e->x = e->x + e->dx;
		e->y = e->y + e->dy / 4;
		e->dy = e->dy + PLAYER_LOWGRAV;
		if (e->y > 256 << PLAYER_RESOLUTION)
		{
			e->state = 0;
		}	
		if (e->y < -64)
		{
			e->state = 0;
		}	
		if (e->x > (320 << PLAYER_RESOLUTION)) 
		{
			e->x = 0;
		} 
		else if (e->x < 0) 
		{
			e->x = 320 << PLAYER_RESOLUTION;
		}
	}
	
	if (e->fly_count > ENEMY_FLY_COUNT_MAX)
	{
		e->state = 0;
	}
	
	// Animate
	if (e->state == 1)
	{
		e->anim_cnt++;
		if (e->anim_cnt == 3)
		{
			e->anim_cnt = 0;
			e->anim_frame++;
		}
		if (e->anim_frame == 6)
		{
			e->anim_frame = 0;
		}
	}
	else if (e->state == 2)
	{
		e->anim_cnt++;
		e->anim_frame = 6 + ((e->anim_cnt >> 2) & 0x01);
	}
	else
	{
		e->x = -64;
		e->y = -64;
		e->phase = 0;
	}
}

void enemy_dma_tiles(enemy_t *e)
{
	// Load the player tiles
	u8 num_tiles = ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT;
	u16 size = (16 * num_tiles);
	u16 *src = enemy_sprites + (32 * (((ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT) * ((8 * e->type) + e->anim_frame))));
	VDP_doVRamDMA(src,ENEMY_BASE_VRAM_INDEX + (2 * size * e->enemy_num),size);
}

void enemy_draw(enemy_t *e)
{
	if (e->state == 0)
	{
		VDP_setSpriteDirect(e->sprite_num,
			-128,
			-128,
			SPRITE_SIZE(1,1),
			TILE_ATTR_FULL(0,0,0,0,0),
			e->sprite_num +1);
	}
	else
	{
		int tile_addr = (ENEMY_BASE_VRAM_INDEX / 32)+((ENEMY_TILE_WIDTH * ENEMY_TILE_HEIGHT) * e->enemy_num);
		VDP_setSpriteDirect(e->sprite_num,
			(e->x >> PLAYER_RESOLUTION) + (16 - (4*ENEMY_TILE_WIDTH)),
			(e->y >> PLAYER_RESOLUTION) - ((8*ENEMY_TILE_HEIGHT)),
			SPRITE_SIZE(ENEMY_TILE_WIDTH, ENEMY_TILE_HEIGHT),
			TILE_ATTR_FULL((e->type & 0x01) + 2,1,0,e->direction,tile_addr),
			e->sprite_num +1);
	}
}

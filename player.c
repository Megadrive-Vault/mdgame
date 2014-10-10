#include "player.h"

void player_init(player *pl)
{
	pl->sprite_num = 0;
	pl->palette = 0;
	pl->tile_index = 0;
	pl->tile_index = 0;
	
	pl->x = 0;
	pl->y = 0;
	pl->dx = 0;
	pl->dy = 0;
	pl->grounded = 0;
	pl->input_id = 0;
	
	pl->jump_key = 0;
	
	pl->slapcnt = 0;
	pl->slapcooldown = 0;
	pl->dashcooldown = 0;
}

void player_take_inputs(player *pl, u8 pad_data)
{
	// Jump negative edge detection
	if (!(pad_data & KEY_B))
	{
		if (pl->jump_key == 0)
		{
			pl->jump_key = 1;
		}
		else if (pl->jump_key == 1)
		{
			pl->jump_key = 2;
		}
	}
	else
	{
		pl->jump_key = 0;
	}
	
	// Horizontal ddx
	if (!(pad_data & KEY_RIGHT))
	{
		if (pl->dx < PLAYER_MAX_DX)
		{
			pl->dx += PLAYER_ACCEL;
		}
		if (pl->dx > PLAYER_MAX_DX)
		{
			pl->dx = PLAYER_MAX_DX;
		}
	}
	else if (!(pad_data & KEY_LEFT))
	{
		if (pl->dx > PLAYER_MAX_DX * -1)
		{
			pl->dx -= PLAYER_ACCEL;
		}
		if (pl->dx < PLAYER_MAX_DX * -1)
		{
			pl->dx = PLAYER_MAX_DX * -1;
		}
	}
	else
	{
		if (pl->dx > PLAYER_DECEL)
		{
			pl->dx -= PLAYER_DECEL;
		}
		if (pl->dx < PLAYER_DECEL * -1)
		{
			pl->dx += PLAYER_DECEL;
		}
		if (pl->dx >= PLAYER_DECEL * -1 && pl->dx <= PLAYER_DECEL)
		{
			pl->dx = 0;
		}
	}
	
	// Jump from the ground
	if (pl->grounded && pl->jump_key == 1)
	{
		if (pl->jump_key == 1)
		{
			pl->dy = PLAYER_JUMPSTR * -1;
		}
	}
}

void player_move(player *pl)
{
	// Terminate a fall when landing
	if (pl->grounded && pl->dy > 0)
	{
		pl->dy = 0;
	}
	
	// Variable strength gravity
	if (!pl->grounded)
	{
		if (pl->jump_key)
		{
			pl->dy += PLAYER_LOWGRAV;
		}
		else
		{
			pl->dy += PLAYER_HIGRAV;
		}
	}
	pl->x = pl->x + pl->dx;
	pl->y = pl->y + pl->dy;
	
	// Actually do the movement now
	
}

void player_draw(player *pl, int sprite_num)
{
	VDP_setSpriteDirect(pl->sprite_num,
		pl->x >> 3,
		pl->y >> 3,
		SPRITE_SIZE(3,4),
		TILE_ATTR_FULL(pl->palette,1,0,pl->direction,pl->tile_index),
		pl->sprite_num +1);
}
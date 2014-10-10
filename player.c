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
}

void player_move(player *pl)
{
	u8 coltype = 0;
	// Actually do the movement now
	
	coltype = player_pos_dx(pl);
	if (coltype == MAP_SOLID)
	{
		pl->dx = 0;
	}
	coltype = player_neg_dx(pl);
	if (coltype == MAP_SOLID)
	{
		pl->dx = 0;
	}
	
	coltype = player_pos_dy(pl);
	if (coltype == MAP_SOLID)
	{
		pl->dy = 0;
	}
	// Handle downwards collision
	coltype = player_neg_dy(pl);
	// Handle upwards collision
	if (coltype == MAP_SOLID)
	{
		pl->dy = pl->dy * -1;
	}
	// Terminate a fall when landing
	if (pl->grounded && pl->dy > 0)
	{
		pl->dy = 0;
	}
	player_ground(pl);
	player_gravity(pl);
	
	// Jump from the ground
	if (pl->grounded && pl->jump_key == 1)
	{
		if (pl->jump_key == 1)
		{
			pl->dy = PLAYER_JUMPSTR * -1;
		}
	}
} 

void player_draw(player *pl)
{
	VDP_setSpriteDirect(pl->sprite_num,
		pl->x >> 3,
		pl->y >> 3,
		SPRITE_SIZE(PLAYER_TILE_WIDTH,PLAYER_TILE_HEIGHT),
		TILE_ATTR_FULL(pl->palette,1,0,pl->direction,pl->tile_index),
		pl->sprite_num +1);
}



// PHYSICS SUPPORT FUNCTIONS
void player_ground(player *pl)
{

	// Find out if we are grounded...
	if (map_collision((pl->x >> 3) + PLAYER_X1,(pl->y >> 3) + PLAYER_Y2 + 1) ||
		map_collision((pl->x >> 3) + PLAYER_X2,(pl->y >> 3) + PLAYER_Y2 + 1) ||
		map_collision((pl->x >> 3),(pl->y >> 3) + PLAYER_Y2 + 1))
	{
		pl->grounded = 1;
	}
	else
	{
		pl->grounded = 0;
	}
}

void player_gravity(player *pl)
{
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
}

u8 player_pos_dy(player *pl)
{
	// Collision type if it should come up
	u8 coltype = 0;
	// Bottom points
	if (pl->dy > 0)
	{
		// Increment step by step, checking for a collision
		for (int l = pl->dy; l != 0; l--)
		{
			u16 checkx1 = (pl->x >> 3) + PLAYER_X1;
			u16 checkx2 = (pl->x >> 3) + PLAYER_X2;
			u16 checkx3 = (pl->x >> 3);
			u16 checky = (pl->y >> 3) + PLAYER_Y2 + 1;
			coltype = map_collision(checkx1, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx2, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx3, checky);
			if (coltype) { break; };
			pl->y = pl->y + 1;
		}
	}
	return coltype;
}

u8 player_neg_dy(player *pl)
{
	// Collision type if it should come up
	u8 coltype = 0;
	// Bottom points
	if (pl->dy < 0)
	{
		// Increment step by step, checking for a collision
		for (int l = pl->dy * -1; l != 0; l--)
		{
			u16 checkx1 = (pl->x >> 3) + PLAYER_X1;
			u16 checkx2 = (pl->x >> 3) + PLAYER_X2;
			u16 checkx3 = (pl->x >> 3);
			u16 checky = (pl->y >> 3) + PLAYER_Y1 - 1;
			coltype = map_collision(checkx1, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx2, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx3, checky);
			if (coltype) { break; };
			pl->y = pl->y - 1;
		}
	}
	return coltype;
}

u8 player_pos_dx(player *pl)
{
	// Collision type if it should come up
	u8 coltype = 0;
	// Bottom points
	if (pl->dx > 0)
	{
		// Increment step by step, checking for a collision
		for (int l = pl->dx; l != 0; l--)
		{
			u16 checkx = (pl->x >> 3) + PLAYER_X2 + 1;
			u16 checky1 = (pl->y >> 3) + PLAYER_Y1;
			u16 checky2 = (pl->y >> 3) + PLAYER_Y2;
			u16 checky3 = (pl->y >> 3) + PLAYER_Y3;
			u16 checky4 = (pl->y >> 3) + PLAYER_Y4;
			coltype = map_collision(checkx, checky1);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky2);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky3);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky4);
			if (coltype) { break; };
			pl->x = pl->x + 1;
		}
	}
	return coltype;
}

u8 player_neg_dx(player *pl)
{
	// Collision type if it should come up
	u8 coltype = 0;
	// Bottom points
	if (pl->dx < 0)
	{
		// Increment step by step, checking for a collision
		for (int l = pl->dx * -1; l != 0; l--)
		{
			u16 checkx = (pl->x >> 3) + PLAYER_X1 - 1;
			u16 checky1 = (pl->y >> 3) + PLAYER_Y1;
			u16 checky2 = (pl->y >> 3) + PLAYER_Y2;
			u16 checky3 = (pl->y >> 3) + PLAYER_Y3;
			u16 checky4 = (pl->y >> 3) + PLAYER_Y4;
			coltype = map_collision(checkx, checky1);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky2);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky3);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky4);
			if (coltype) { break; };
			pl->x = pl->x - 1;
		}
	}
	return coltype;
}

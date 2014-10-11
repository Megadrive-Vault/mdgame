#include "player.h"

void player_init(player *pl)
{
	pl->sprite_num = 0;
	pl->palette = 0;
	pl->tile_index = 0;
	pl->tile_offset = 0;
	
	pl->x = 32;
	pl->y = 32;
	pl->dx = 0;
	pl->dy = 0;
	pl->grounded = 0;
	pl->input_id = 0;
	
	pl->lowgrav_wait = 0;
	pl->higrav_wait = 0;
	pl->accel_wait = 0;
	pl->decel_wait = 0;
	
	pl->priority = 1;
	
	pl->vis = 1;
	
	pl->jump_key = 0;
	
	pl->slapcnt = 0;
	pl->slapcooldown = 0;
	pl->dashcooldown = 0;
	
	pl->dashok = 1;
	pl->slapok = 1;
}

void player_take_inputs(player *pl, u8 pad_data)
{
	// Priority
	pl->priority = (!(pad_data & KEY_A))?1:0;
	
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
	if (!(pad_data & KEY_RIGHT) && pl->dashcooldown == 0)
	{
		pl->direction = 0;
		if (pl->dx <= PLAYER_MAX_DX)
		{
			if (PLAYER_ACCEL_WAIT)
			{
				if (pl->accel_wait == 0)
				{
					pl->dx += PLAYER_ACCEL;
					pl->accel_wait = PLAYER_ACCEL_WAIT;
				}
				else
				{
					pl->accel_wait--;
				}
			}
			else
			{
				pl->dx += PLAYER_ACCEL;
			}
		}
		else
		{
			pl->dx-= PLAYER_ACCEL<<1;
		}
	}
	else if (!(pad_data & KEY_LEFT) && pl->dashcooldown == 0)
	{
		pl->direction = 1;
		if (pl->dx >= PLAYER_MAX_DX * -1)
		{
			if (PLAYER_ACCEL_WAIT)
			{
				if (pl->accel_wait == 0)
				{
					pl->dx -= PLAYER_ACCEL;
					pl->accel_wait = PLAYER_ACCEL_WAIT;
				}
				else
				{
					pl->accel_wait--;
				}
			}
			else
			{
				pl->dx -= PLAYER_ACCEL;
			}
		}
		else
		{
			pl->dx+= PLAYER_ACCEL<<1;
		}
	}
	else
	{
		if (pl->dx >= PLAYER_DECEL)
		{
			if (PLAYER_DECEL_WAIT)
			{
				if (pl->decel_wait == 0)
				{
					pl->dx -= PLAYER_DECEL;
					pl->decel_wait = PLAYER_DECEL_WAIT;
				}
				else
				{
					pl->decel_wait--;
				}
			}
			else
			{
				pl->dx -= PLAYER_DECEL;
			}
		}
		if (pl->dx <= PLAYER_DECEL * -1)
		{
			if (PLAYER_DECEL_WAIT)
			{
				if (pl->decel_wait == 0)
				{
					pl->dx += PLAYER_DECEL;
					pl->decel_wait = PLAYER_DECEL_WAIT;
				}
				else
				{
					pl->decel_wait--;
				}
			}
			else
			{
				pl->dx += PLAYER_DECEL;
			}
		}
		if (pl->dx > PLAYER_DECEL * -1 && pl->dx < PLAYER_DECEL)
		{
			pl->dx = 0;
		}
	}
	
	// Limits
	if (pl->dy > PLAYER_MAX_DY)
	{
		pl->dy = PLAYER_MAX_DY;
	}
	if (pl->dy < PLAYER_MAX_DY * -1)
	{
		pl->dy = PLAYER_MAX_DY * -1;
	}
	
	// Dash inputs
	if (!(pad_data & KEY_C))
	{
		if (pl->dashcooldown == 0 && pl->dashok)
		{
			pl->dashcooldown = PLAYER_DASHTIME;
			player_dash_vectors(pl, pad_data);
		}
	}
}

void player_move(player *pl)
{
	player_dash(pl);
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
	
	coltype = player_neg_dy(pl);
	if (coltype == MAP_SOLID)
	{
		pl->dy = (pl->dy >> 1) * -1;
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
	player_vis(pl);
	if (pl->vis & 1)
	{
		VDP_setSpriteDirect(pl->sprite_num,
			(pl->x >> PLAYER_RESOLUTION) + PLAYER_X1 - 9 + (16 - (4*PLAYER_TILE_WIDTH)),
			(pl->y >> PLAYER_RESOLUTION) - ((8*PLAYER_TILE_HEIGHT) - PLAYER_Y2),
			SPRITE_SIZE(PLAYER_TILE_WIDTH,PLAYER_TILE_HEIGHT),
			TILE_ATTR_FULL(pl->palette,pl->priority,0,pl->direction,pl->tile_index + (pl->tile_offset * (PLAYER_TILE_WIDTH*PLAYER_TILE_HEIGHT))),
			pl->sprite_num +1);
	}
	else // Don't render
	{
		VDP_setSpriteDirect(pl->sprite_num,
			-128,
			-128,
			SPRITE_SIZE(1,1),
			TILE_ATTR_FULL(0,0,0,0,0),
			pl->sprite_num +1);
	}
}



// PHYSICS SUPPORT FUNCTIONS


void player_dash_vectors(player *pl, u8 pad_data)
{
	// X
	if (!(pad_data & KEY_RIGHT))
	{
		pl->dx = PLAYER_DASH_THRUST_X;
	}
	else if (!(pad_data & KEY_LEFT))
	{
		pl->dx = 1 + PLAYER_DASH_THRUST_X * -1;
	}
	else if (pl->grounded)
	{
		pl->dx = (pl->direction) ? (PLAYER_DASH_THRUST_X * -1) : PLAYER_DASH_THRUST_X;
	}
	if (!(pad_data & KEY_DOWN))
	{
		pl->dy = PLAYER_DASH_THRUST_Y;
	}
	else if (!(pad_data & KEY_UP))
	{
		pl->dy = 1 + PLAYER_DASH_THRUST_Y * -1;
	}
	else
	{
		pl->dy = 0;
	}
	// Y
}

void player_ground(player *pl)
{
	// Find out if we are grounded...
	if (map_collision((pl->x >> PLAYER_RESOLUTION) + PLAYER_X1,(pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2 + 1) ||
		map_collision((pl->x >> PLAYER_RESOLUTION) + PLAYER_X2,(pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2 + 1) ||
		map_collision((pl->x >> PLAYER_RESOLUTION),(pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2 + 1))
	{
		// If a landing just occured, cancel dash cooldown (wavedashing, heh)
		if (pl->dashcooldown != 0 && pl->grounded == 0)
		{
			pl->dashcooldown = 0;
		}
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
		if (pl->jump_key || pl->dashcooldown != 0)
		{
			if (PLAYER_LOWGRAV_WAIT)
			{
				if (pl->lowgrav_wait == 0)
				{
					pl->dy += PLAYER_LOWGRAV;
					pl->lowgrav_wait = PLAYER_LOWGRAV_WAIT;
				}
				else
				{
					pl->lowgrav_wait--;
				}
			}
			else
			{
				pl->dy += PLAYER_LOWGRAV;
			}
		}
		else
		{
			if (PLAYER_HIGRAV_WAIT)
			{
				if (pl->higrav_wait == 0)
				{
					pl->dy += PLAYER_HIGRAV;
					pl->higrav_wait = PLAYER_HIGRAV_WAIT;
				}
				else
				{
					pl->higrav_wait--;
				}
			}
			else
			{
				pl->dy += PLAYER_HIGRAV;
			}
		}
		
	}
}

void player_vis(player *pl)
{
	if (pl->dashcooldown == 0)
	{
		pl->vis = 1;
	}
	else
	{
		pl->vis++;
	}
}

void player_dash(player *pl)
{
	if (pl->dashcooldown != 0)
	{
		pl->dashok = 0;
		pl->dashcooldown--;
	}
	if (pl->grounded)
	{
		pl->dashok = 1;
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
			u16 checkx1 = (pl->x >> PLAYER_RESOLUTION) + PLAYER_X1;
			u16 checkx2 = (pl->x >> PLAYER_RESOLUTION) + PLAYER_X2;
			u16 checkx3 = (pl->x >> PLAYER_RESOLUTION);
			u16 checky = (pl->y >>  PLAYER_RESOLUTION) + PLAYER_Y2 + 1;
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
			u16 checkx1 = (pl->x >> PLAYER_RESOLUTION) + PLAYER_X1;
			u16 checkx2 = (pl->x >> PLAYER_RESOLUTION) + PLAYER_X2;
			u16 checkx3 = (pl->x >> PLAYER_RESOLUTION);
			u16 checky = (pl->y >>  PLAYER_RESOLUTION) + PLAYER_Y1 - 1;
			coltype = map_collision(checkx1, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx2, checky);
			if (coltype) { break; };
			coltype = map_collision(checkx3, checky);
			if (coltype) { break; };
			if (pl->y != 0)
			{
				pl->y = pl->y - 1;
			}
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
			u16 checkx = ((pl->x >> PLAYER_RESOLUTION) + PLAYER_X2 + 1) % 320;
			u16 checky1 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y1;
			u16 checky2 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2;
			u16 checky3 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y3;
			coltype = map_collision(checkx, checky1);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky2);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky3);
			if (coltype) { break; };
			if (pl->x == (320 << PLAYER_RESOLUTION) - 1)
			{
				pl->x = -1;
			}
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
			u16 checkx = ((pl->x >>  PLAYER_RESOLUTION) + PLAYER_X1 - 1);
			if (checkx > 320) // Trust me on this one, it'll fix the left.
			{
				checkx += 320;
			}
			u16 checky1 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y1;
			u16 checky2 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y2;
			u16 checky3 = (pl->y >> PLAYER_RESOLUTION) + PLAYER_Y3;
			coltype = map_collision(checkx, checky1);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky2);
			if (coltype) { break; };
			coltype = map_collision(checkx, checky3);
			if (coltype) { break; };
			if (pl->x == 0)
			{
				pl->x = (320 << PLAYER_RESOLUTION) - 1;
			}
			pl->x = pl->x - 1;
		}
	}
	return coltype;
}

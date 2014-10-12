#include "player.h"

void player_dma_tiles(player *pl)
{
	// Load the player tiles
	u8 num_tiles = PLAYER_TILE_WIDTH * PLAYER_TILE_HEIGHT;
	u16 size = (16 * num_tiles);
	u16 *src_addr = player_sprites + (32*(PLAYER_TILE_WIDTH * PLAYER_TILE_HEIGHT)*pl->tile_offset);
	VDP_doVRamDMA(src_addr,pl->tile_index + (32*(PLAYER_TILE_WIDTH * PLAYER_TILE_HEIGHT) * pl->player_num),size);
}

void player_calc_animation(player *pl)
{
	if (pl->hitstun == 0 && pl->slapcnt == 0 && pl->slapcooldown == 0)
	{
		if (pl->dashcooldown != 0)
		{
			pl->current_anim = PLAYER_ANIM_DASH;
		}
		else
		{
			if (pl->grounded)
			{
				if (pl->dx != 0)
				{
					pl->current_anim = PLAYER_ANIM_RUN;
				}
				else
				{
					pl->current_anim = PLAYER_ANIM_STAND;
				}
			}
			else
			{
				if (pl->dy <= 0)
				{
					pl->current_anim = PLAYER_ANIM_JUMP;
				}
				else
				{
					pl->current_anim = PLAYER_ANIM_FALL;
				}
			}
		}
	}
	else if (pl->hitstun != 0)
	{
		pl->current_anim = PLAYER_ANIM_HURT;
	}
	else if (pl->slapcnt != 0)
	{
		pl->current_anim = PLAYER_ANIM_PRESLAP;
	}
	else if (pl->slapcooldown != 0)
	{
		pl->current_anim = PLAYER_ANIM_POSTSLAP;
	}
}

void player_animate(player *pl)
{
	player_calc_animation(pl);
	if (pl->hitfreeze != 0)
	{
		return;
	}
	if (pl->flash != 0)
	{
		pl->flash--;
	}
	int xmag = pl->dx;
	switch (pl->current_anim)
	{
		case PLAYER_ANIM_STAND:
			pl->anim_cnt_stand++;
			pl->anim_cnt_run = 0;
			if (pl->anim_cnt_stand == 80)
			{
				pl->anim_cnt_stand = 0;
			}
			pl->tile_offset = PLAYER_ANIM_OFF_STAND + 1;
			if (pl->anim_cnt_stand < 30)
			{
				pl->tile_offset -= 1;
			}
			if (pl->anim_cnt_stand > 40 && pl->anim_cnt_stand < 70)
			{
				pl->tile_offset += 1;
			}
			break;
		case PLAYER_ANIM_RUN:
			pl->anim_cnt_run++;
			if (pl->anim_cnt_run == 48)
			{
				pl->anim_cnt_run = 0;
			}
			pl->tile_offset = PLAYER_ANIM_OFF_RUN + (pl->anim_cnt_run / 6);
			break;
		case PLAYER_ANIM_JUMP:
			pl->anim_cnt_jump++;
			pl->anim_cnt_fall = 0;
			if (pl->anim_cnt_jump == 12)
			{
				pl->anim_cnt_jump = 0;
			}
			if (!pl->slapok)
			{
				pl->anim_cnt_fall = 4;
			}
			pl->tile_offset = PLAYER_ANIM_OFF_JUMP + (pl->anim_cnt_jump / 4);
			break;
		case PLAYER_ANIM_FALL:
			pl->anim_cnt_fall++;
			if (pl->anim_cnt_fall == 16)
			{
				pl->anim_cnt_fall = 4;
			}
			pl->tile_offset = PLAYER_ANIM_OFF_FALL + (pl->anim_cnt_fall / 4);
			break;
		case PLAYER_ANIM_DASH:
			pl->tile_offset = PLAYER_ANIM_OFF_RUN + 2;
			break;
		case PLAYER_ANIM_PRESLAP:
			pl->tile_offset = PLAYER_ANIM_OFF_PRESLAP;
			break;
		case PLAYER_ANIM_POSTSLAP:
			pl->tile_offset = PLAYER_ANIM_OFF_POSTSLAP + 1;
			if (pl->slapcooldown > PLAYER_SLAP_THRESHHOLD)
			{
				pl->tile_offset--; 
			}
			break;
		case PLAYER_ANIM_HURT: 
			if (xmag < 0)
			{
				pl->direction = 1;
				xmag = xmag * -1;
			}
			else
			{
				pl->direction = 0;
			}
			if (pl->dy < 0)
			{
				if (pl->dy * -1 > xmag + 3)
				{
					pl->tile_offset = PLAYER_ANIM_OFF_HURT_UP;
				}
				else
				{
					pl->tile_offset = PLAYER_ANIM_OFF_HURT_SIDE;
				}
			}
			else if (pl->dy > 0)
			{
				if (pl->dy > xmag + 3)
				{
					pl->tile_offset = PLAYER_ANIM_OFF_HURT_DOWN;
				}
				else
				{
					pl->tile_offset = PLAYER_ANIM_OFF_HURT_SIDE;
				}
			}
			else
			{
				pl->tile_offset = PLAYER_ANIM_OFF_HURT_SIDE;
			}
			break;
	}
}

void player_init(player *pl)
{
	pl->other = NULL;
	pl->slaps = 0;
	pl->sprite_num = 0;
	pl->palette = 0;
	pl->tile_index = 16 * 32;
	pl->tile_offset = 0;
	pl->player_num = 0;
	
	pl->anim_cnt_stand = 0;
	pl->anim_cnt_run = 0;
	pl->anim_cnt_jump = 0;
	pl->anim_cnt_fall = 0;
	
	pl->current_anim = 0;
	
	pl->pad_data = 0xFF;
	
	
	pl->osc = 0;
	pl->hitstun = 0;
	pl->flash = 0;
	pl->hitfreeze = 0;
	
	pl->x = 96;
	pl->y = 32;
	pl->dx = 0;
	pl->dy = 0;
	pl->grounded = 0;
	pl->input_id = 0;
	
	pl->lowgrav_wait = 0;
	pl->higrav_wait = 0;
	pl->accel_wait = 0;
	pl->decel_wait = 0;
	
	pl->priority = 0;
	
	pl->vis = 1;
	
	pl->jump_key = 0;
	
	pl->slapcnt = 0;
	pl->slapcooldown = 0;
	pl->dashcooldown = 0;
	
	pl->dashok = 1;
	pl->slapok = 1;
	
	const u16 pal1[] = {
		0x0000, 0x0420, 0x0820, 0x0222,
		0x0CCC, 0x0EEE, 0x029C, 0x06CE,
		0x0000, 0x0820, 0x04AE, 0x0EEE,
		0x0000, 0x0820, 0x04AE, 0x0EEE
	};
	const u16 pal2[] = {
		0x0000, 0x0204, 0x0208, 0x006E,
		0x0ACC, 0x0EEE, 0x04AE, 0x06CE,
		0x0000, 0x0026, 0x04AE, 0x0EEE,
		0x0000, 0x0026, 0x04AE, 0x0EEE
	};
	
	for (int i = 0; i < 16; i++)
	{
		if (i != 0)
		{
			pl->white_pal[i] = 0x0EEE;
		}
		else
		{
			pl->white_pal[i] = 0x0000;
		}	
		pl->normal_pal1[i] = pal1[i];
		pl->normal_pal2[i] = pal2[i];
		if (i < 8)
		{
			pl->light_pal1[i] = (pal1[i] << 1) | (pal1[i]);
			pl->light_pal2[i] = (pal2[i] << 1) | (pal2[i]);
		}
		else
		{
			pl->light_pal1[i] = pal1[i];
			pl->light_pal2[i] = pal2[i];
		}
	}
}

void player_dma_pal(player *pl)
{
	u16 **pal_addr = NULL;
	if ((pl->flash >> 1) & 0x01)
	{
		pal_addr = &(pl->white_pal);
	}
	else if (pl->dashcooldown == 0)
	{
		pal_addr = (pl->player_num == 0) ? &(pl->normal_pal1) : &(pl->normal_pal2);
	}
	else
	{
		if (pl->osc & 0x01)
		{
			pal_addr = (pl->player_num == 0) ? &(pl->light_pal1) : &(pl->light_pal2);
		}
		else
		{
			pal_addr = (pl->player_num == 0) ? &(pl->normal_pal1) : &(pl->normal_pal2);
		}
	}
	
	VDP_doCRamDMA(pal_addr,(pl->player_num << 5) + 0x0040,16);
}

void player_take_inputs(player *pl, u8 pad_data)
{
	pl->pad_data = pad_data;
	if (pl->hitfreeze != 0) { return; }
	
	if (pl->hitstun != 0)
	{
		// Might do more here later
		return;
	}
	
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
	
	// Horizontal acceleration
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
	// Deceleration
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
}

void player_counters(player *pl)
{
	pl->osc++;
	if (pl->hitfreeze != 0) 
	{ 
		pl->hitfreeze--;
	}
	if (pl->hitstun != 0)
	{
		pl->hitstun--;
	}
}

void player_move(player *pl)
{
	player_counters(pl);
	if (pl->hitfreeze != 0) 
	{ 
		return; 
	}
	player_dash(pl);
	u8 coltype = 0;
	
	// Vectors limits
	
	if (pl->dy > PLAYER_MAX_DY)
	{
		pl->dy = PLAYER_MAX_DY;
	}
	if (pl->dy < PLAYER_MAX_DY * -1)
	{
		pl->dy = PLAYER_MAX_DY * -1;
	}
	// Actually do the movement now
	coltype = player_pos_dx(pl);
	if (coltype == MAP_SOLID)
	{
		if (pl->hitstun == 0)
		{
			pl->dx = 0;
		}
		else
		{
			pl->dx = pl->dx * -1;
		}
	}
	
	coltype = player_neg_dx(pl);
	if (coltype == MAP_SOLID)
	{
		if (pl->hitstun == 0)
		{
			pl->dx = 0;
		}
		else
		{
			pl->dx = pl->dx * -1;
		}
	}
	
	coltype = player_pos_dy(pl);
	if (coltype == MAP_SOLID)
	{
		if (pl->hitstun == 0)
		{
			pl->dy = 0;
		}
		else
		{
			pl->dy = pl->dy * -1;
		}
	}
	
	coltype = player_neg_dy(pl);
	if (coltype == MAP_SOLID)
	{
		if (pl->hitstun == 0)
		{
			pl->dy = 0;
		}
		else
		{
			pl->dy = pl->dy * -1;
		}
	}
	else if (coltype == MAP_CIELING)
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

void player_update_sprite(player *pl)
{
	
}

void player_draw(player *pl)
{
	player_vis(pl);
	if (pl->vis & 1)
	{
		VDP_setSpriteDirect(pl->sprite_num,
			(pl->x >> PLAYER_RESOLUTION) + PLAYER_X1 - 9 + (16 - (4*PLAYER_TILE_WIDTH)),
			(pl->y >> PLAYER_RESOLUTION) - ((8*PLAYER_TILE_HEIGHT) - PLAYER_Y2) + 1,
			SPRITE_SIZE(PLAYER_TILE_WIDTH,PLAYER_TILE_HEIGHT),
			TILE_ATTR_FULL(pl->palette,pl->priority,0,pl->direction,(pl->tile_index / 32)+(PLAYER_TILE_WIDTH * PLAYER_TILE_HEIGHT * pl->player_num)),
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


void player_collide(player *pl)
{
	if ((pl->x >> 1) + PLAYER_X2 + PLAYER_SLAP_W < (pl->other->x >> 1) + PLAYER_X1 - PLAYER_SLAP_W)
	{
	
	}
	else if ((pl->x >> 1) + PLAYER_X1 - PLAYER_SLAP_W > (pl->other->x >> 1) + PLAYER_X2 + PLAYER_SLAP_W)
	{
	
	}
	else if ((pl->y >> 1) + PLAYER_Y1 - PLAYER_SLAP_H > (pl->other->y >> 1) + PLAYER_Y2)
	{
	
	}
	else if ((pl->y >> 1) + PLAYER_Y2 < (pl->other->y >> 1) + PLAYER_Y1 - PLAYER_SLAP_H)
	{
	
	}
	else
	{
		if (pl->slapcooldown > PLAYER_SLAP_THRESHHOLD)
		{
			pl->slaps++;
			pl->other->slapcooldown = 0;
			pl->other->slapcnt = 0;
			pl->other->hitfreeze = 6;
			pl->hitfreeze = 6;
			pl->other->dx = (pl->direction ? -2 : 2) + (pl->dx / 2);
			if ((pl->direction == 0 && !(pl->pad_data & KEY_RIGHT)) || 
				(pl->direction == 1 && !(pl->pad_data & KEY_LEFT)))
			{
				pl->other->dx += (pl->direction ? -4 : 4);
			}
			
			int magx = pl->dx;
			if (magx < 0)
			{
				magx = magx * -1;
			}
			
			if (pl->grounded)
			{
				pl->other->dy = -14;
			}
			else
			{
				pl->other->dy = -1 * (pl->y - pl->other->y);
				if (!(pl->pad_data & KEY_UP))
				{
					pl->other->dy -= 4;
				}
				if (!(pl->pad_data & KEY_DOWN))
				{
					pl->other->dy += 5;
				}
				pl->dy = pl->dy >> 1;
			}
			pl->other->hitstun = 35 + magx;
			
			pl->other->flash = 20;
		}
	}
}

// PHYSICS SUPPORT FUNCTIONS


void player_dash_vectors(player *pl)
{
	if (!(pl->pad_data & KEY_RIGHT))
	{
		pl->dx = PLAYER_DASH_THRUST_X;
	}
	else if (!(pl->pad_data & KEY_LEFT))
	{
		pl->dx = PLAYER_DASH_THRUST_X * -1;
	}
	else if (pl->grounded)
	{
		pl->dx = (pl->direction) ? (PLAYER_DASH_THRUST_X * -1) : PLAYER_DASH_THRUST_X;
	}
	if (!(pl->pad_data & KEY_DOWN))
	{
		pl->dy = PLAYER_DASH_THRUST_Y;
	}
	else if (!(pl->pad_data & KEY_UP))
	{
		pl->dy = 1 + PLAYER_DASH_THRUST_Y * -1;
	}
	else
	{
		pl->dy = 0;
	}
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
	pl->vis = 1;
}

void player_dash(player *pl)
{
	
	// Dash inputs
	if (!(pl->pad_data & KEY_C))
	{
		if (pl->dashcooldown == 0 && pl->dashok && pl->hitstun == 0)
		{
			pl->dashcooldown = PLAYER_DASHTIME;
			player_dash_vectors(pl);
		}
	}
	if (pl->dashcooldown != 0 )
	{
		pl->dashok = 0;
		pl->dashcooldown--;
	}
	if (pl->grounded && (pl->pad_data & KEY_C))
	{
		pl->dashok = 1;
	}
}

void check_bounds(u16 *check, u16 limit)
{
	if (*check > limit)
	{
		*check = limit;
	}
}

void player_slap(player *pl)
{
	// No slapping is occurring
	if (pl->slapcooldown == 0 && pl->slapcnt == 0)
	{
		if (!(pl->pad_data & KEY_A) && pl->slapok)
		{
			pl->slapcnt = PLAYER_SLAPTIME;
			pl->slapok = 0;
		}
		else if ((pl->pad_data & KEY_A))
		{
			pl->slapok = 1;
		}
	}
	// Getting ready to slap
	else if (pl->slapcnt > 0)
	{
		// Actually do the slap
		if (pl->slapcnt == 1)
		{
			pl->slapcooldown = PLAYER_SLAP_COOLDOWNTIME;
		}
		pl->slapcnt--;
	}
	else if (pl->slapcooldown > 0)
	{
		pl->slapcooldown--;
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
			check_bounds(&checkx2,320);
			u16 checkx3 = (pl->x >> PLAYER_RESOLUTION);
			check_bounds(&checkx3,320);
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
			check_bounds(&checkx1,320);
			u16 checkx2 = (pl->x >> PLAYER_RESOLUTION) + PLAYER_X2;
			u16 checkx3 = (pl->x >> PLAYER_RESOLUTION);
			check_bounds(&checkx3,320);
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

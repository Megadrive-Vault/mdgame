#include "map.h"

// Static function prototypes //
static void map_plot(void);
static void map_load_default(game_map_set *map);

// Static variables //
// NONE

// API Functions //
void map_init(game_map_set *map)
{
	// Load the map
	map_load_default(map);
	// Plot the foreground and background onto the screen
	map_plot();
}

// Internal Functions //
static void map_load_default(game_map_set *map)
{
	memcpy(&collision_map,  map->collision,  sizeof(u8[32][40]));
	memcpy(&foreground_map, map->foreground, sizeof(u8[32][40]));
	memcpy(&background_map, map->background, sizeof(u8[32][40]));
}

static void map_plot(void)
{
	VDP_clearPlan(VDP_PLAN_A,1);
	VDP_clearPlan(VDP_PLAN_B,1);
	
	for (u8 y = 0; y < 28; y++)
	{
		for (u8 x = 0; x < 40; x++)
		{
			VDP_setTileMapXY(VDP_PLAN_A,TILE_ATTR_FULL(foreground_map[y][x]>>7,1,0,0,(FG_BASE_VRAM_INDEX/32) + (foreground_map[y][x] & 0x7F)),x,y);
			VDP_setTileMapXY(VDP_PLAN_B,TILE_ATTR_FULL(background_map[y][x]>>7,0,0,0,(BG_BASE_VRAM_INDEX/32) + (background_map[y][x] & 0x7F)),x,y);
		}
	}
}

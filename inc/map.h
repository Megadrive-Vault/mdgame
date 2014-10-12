#ifndef MAP_H
#define MAP_H
#include "includes.h"

// Types and structs //
typedef u8 game_map[32][40];
typedef struct game_map_set
{
	game_map *collision;
	game_map *foreground;
	game_map *background;
} game_map_set;

// Global variables //
game_map collision_map;
game_map foreground_map;
game_map background_map;

// Function prototypes //
void map_init(game_map_set*);

#endif

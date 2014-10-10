#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <genesis.h>

#include "player.h"
#include "mpad.h"

#define MAP_EMPTY 0
#define MAP_SOLID 1

// Struct to hold various properties about the game state

typedef struct gamedata gamedata;
struct gamedata
{
	player *p1;
	player *p2; 
	u8 pad1;
	u8 pad2;
};

u8 map[28][40]; 

void read_pads(gamedata *data);
void ghetto_map_render();

#endif
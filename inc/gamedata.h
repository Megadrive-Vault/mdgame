#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <genesis.h>

#include "player.h"
#include "mpad.h"

#define MAP_EMPTY 0
#define MAP_SOLID 1

// Struct to hold various properties about the game state

u8 map[28][40]; 

u8 map_collision(u16 x, u16 y);
void ghetto_map_render();

#endif

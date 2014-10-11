#ifndef ENEMY_H
#define ENEMY_H

#include <genesis.h>
#include "mpad.h"
#include "gamedata.h"
#include "mpsg.h"
#include "globals.h"

// TODO: Create enemy sprites
// #include "enemy_sprites.h"

typedef struct enemy
{
	u16 x;
	u16 y;

	u8 direction; // 0 = Rightward, 1 = Leftward
	u8 phase;
	u8 flip_curve;
} enemy;

void enemy_spawn(enemy*);
void enemy_update(enemy*);
void enemy_draw(enemy*);

#endif

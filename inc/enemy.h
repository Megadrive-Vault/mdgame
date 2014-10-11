#ifndef ENEMY_H
#define ENEMY_H

#include <genesis.h>
#include "mpad.h"
#include "gamedata.h"
#include "mpsg.h"
#include "globals.h"

#include "enemy_sprites.h"

#define ENEMY_TILE_WIDTH 2
#define ENEMY_TILE_HEIGHT 2

// Index is tile number * 32
#define ENEMY_BASE_VRAM_INDEX (64) * 32

// TODO: Create enemy sprites
// #include "enemy_sprites.h"

typedef struct enemy
{
u16 orig_x;
u16 orig_y;

u16 x;
u16 y;

u8 direction; // 0 = Rightward, 1 = Leftward
u8 phase;

u8 state; // 0 = flying about, 1 = after being hit
u8 type; // Which "bird", and also which reaction to being hit. TBD
u8 sprite_num;

u8 enemy_num;
u8 anim_cnt;
u8 anim_frame;
} enemy;

void enemy_animate(enemy *);
void enemy_spawn(enemy *);
void enemy_update(enemy *);
void enemy_draw(enemy *);

#endif

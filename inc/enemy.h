#ifndef ENEMY_H
#define ENEMY_H
#include "includes.h"

#define ENEMY_TILE_WIDTH 2
#define ENEMY_TILE_HEIGHT 2
#define ENEMY_FLY_COUNT_MAX 3

// Index is tile number * 32
#define ENEMY_BASE_VRAM_INDEX (64) * 32
#define ENEMY_SPAWN_TIME 60 * 8

// TODO: Create enemy sprites
// #include "enemy_sprites.h"

typedef struct enemy_t enemy_t;
struct enemy_t
{
	// point_diff *curve;
	int16_t x;
	int16_t y;
	
	int dx;
	int dy;

	u8 direction; // 0 = Rightward, 1 = Leftward
	u8 phase;
	u8 flip_curve;
	struct point_diff *curve;
	u16 curve_len;

	u8 state; // 0 = inactive, 1 = flying about, 2 = after being hit
	u8 type; // Which "bird", and also which reaction to being hit. TBD
	u8 sprite_num;

	u8 enemy_num;
	u8 anim_cnt;
	u8 anim_frame;
	
	u8 fly_count;
};

void enemy_animate(enemy_t *e);
void enemy_spawn(enemy_t *e, u8 num, u8 seed);
void enemy_update(enemy_t *e);
void enemy_draw(enemy_t *e);

//void enemy_check_player(enemy_t *e, player *pl);
//void enemy_hit_player(enemy_t *e, player *pl);
//void enemy_get_hit(enemy_t *e, player *pl);
#endif

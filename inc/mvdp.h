#ifndef MVDP_H
#define MVDP_H

#include "genesis.h"
/* Megadrive VDP functions */

#define VDP_DATAPORT 0xC00000
#define VDP_CTRL 0xC00004 // When reading, this is the status register
#define VDP_STATUS VDP_CTRL // When writing, this is the control port
#define VDP_COUNTERS 0xC00008 // H and V counters (read)

// VDP Register names
#define VDP_REG_MODE1 0
#define VDP_REG_MODE2 1

// Bases of tables for graphics
#define VDP_REG_NAMEA 2
#define VDP_REG_NAMEW 3
#define VDP_REG_NAMEB 4
#define VDP_REG_SPRITES 5
#define VDP_REG_BGCOLOR 7
#define VDP_REG_HINT 10
#define VDP_REG_MODE3 11
#define VDP_REG_MODE4 12
#define VDP_REG_HSCROLL_BASE 13
#define VDP_REG_AUTOINC 15

// Bitfield values used for sprite routines
#define VDP_SPRITE_HFLIP 0x00001000
#define VDP_SPRITE_VFLIP 0x00010000
#define VDP_SPRITE_PRIORITY 0x10000000
#define VDP_SPRITE_W_1 0x00000000
#define VDP_SPRITE_W_2 0x00000100
#define VDP_SPRITE_W_3 0x00001000
#define VDP_SPRITE_W_4 0x00001100
#define VDP_SPRITE_H_1 0x00000000
#define VDP_SPRITE_H_2 0x00000001
#define VDP_SPRITE_H_3 0x00000010
#define VDP_SPRITE_H_4 0x00000011
                           
// This doc is pretty clear - YaVDP
// http://devster.monkeeh.com/sega/yavdp.html

// Set up the VDP. For now, fairly hard-coded to this game.
// TODO: Take arguments for more use cases
void vdp_init(void);

void vdp_write_register(u16 regnum, u16 data); 
void vdp_set_sprite_base(u16 base);

void vdp_read_vram(u16 addr);
void vdp_write_vram(u16 addr, u16 data);

// Sprite functions
/*

Sprite access:

Data present in VDP VRAM is big endian.

Base sprite address = (Sprite base) + (8 bytes * n)

Index + 0  :   ------yy yyyyyyyy
Index + 2  :   ----hhvv
Index + 3  :   -lllllll
Index + 4  :   pccvhnnn nnnnnnnn
Index + 6  :   ------xx xxxxxxxx

y = Vertical coordinate of sprite
h = Horizontal size in cells (00b=1 cell, 11b=4 cells)
v = Vertical size in cells (00b=1 cell, 11b=4 cells)
l = Link field
p = Priority
c = Color palette
v = Vertical flip
h = Horizontal flip
n = Sprite pattern start index
x = Horizontal coordinate of sprite


*/
void vdp_sprite_set_x(u16 n, u16 x);
void vdp_sprite_set_y(u16 n, u16 y);
void vdp_sprite_set_size(u16 n, u16 w, u16 h); // Size is in tiles
void vdp_sprite_set_link(u16 n, u16 link);
void vdp_set_sprite_pal(u16 n, u16 pal);
void vdp_set_sprite_priority(u16 n, u16 p);
void vdp_set_sprite_flip(u16 n, u16 flip);

#endif

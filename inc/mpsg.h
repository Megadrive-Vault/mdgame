#ifndef MPSG_H
#define MPSG_H
#include <genesis.h>

// Base note frequencies - tuned to 4th octave
#define PSG_BASE_C 0x1AC
#define PSG_BASE_Db 0x194
#define PSG_BASE_D 0x17D
#define PSG_BASE_Eb 0x168
#define PSG_BASE_E 0x153
#define PSG_BASE_F 0x140
#define PSG_BASE_Gb 0x12E
#define PSG_BASE_G 0x11D
#define PSG_BASE_Ab 0x10D
#define PSG_BASE_A 0x0FE
#define PSG_BASE_Bb 0x0F0
#define PSG_BASE_B 0x0E2

#define PSG_NOTE_C  0
#define PSG_NOTE_Db 1
#define PSG_NOTE_D  2
#define PSG_NOTE_Eb 3
#define PSG_NOTE_E  4
#define PSG_NOTE_F  5
#define PSG_NOTE_Gb 6
#define PSG_NOTE_G  7
#define PSG_NOTE_Ab 8
#define PSG_NOTE_A  9
#define PSG_NOTE_Bb 10
#define PSG_NOTE_B  11

// Megadrive PSG functions
void psg_vol(unsigned char chan, unsigned char vol);
void psg_pitch(unsigned char chan, unsigned int pitch);
void psg_tone(unsigned char chan, unsigned char vol, unsigned int pitch);
void psg_note(unsigned char chan, unsigned char note, unsigned char octave);

#endif

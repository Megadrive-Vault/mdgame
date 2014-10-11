#include "mpsg.h"

void psg_vol(unsigned char chan, unsigned char vol)
{
	volatile unsigned char *psg = (unsigned char *)0xC00011;
	*psg = 0x90 | ((chan & 0x03) << 5) | (vol & 0x0F);
}

void psg_pitch(unsigned char chan, unsigned int pitch)
{
	volatile unsigned char *psg = (unsigned char *)0xC00011;
	*psg = 0x80 | ((chan & 0x03) << 5) | (pitch & 0x0F);
	*psg = (pitch >> 4) & 0x3F;
}

void psg_tone(unsigned char chan, unsigned char vol, unsigned int pitch)
{	
	psg_pitch(chan,pitch);
	psg_vol(chan,vol);
}

unsigned int note_lookup(unsigned char note)
{
	switch (note)
	{
		case PSG_NOTE_C:
			return PSG_BASE_C;
		case PSG_NOTE_Db:
			return PSG_BASE_Db;
		case PSG_NOTE_D:
			return PSG_BASE_D;
		case PSG_NOTE_Eb:
			return PSG_BASE_Eb;
		case PSG_NOTE_E:
			return PSG_BASE_E;
		case PSG_NOTE_F:
			return PSG_BASE_F;
		case PSG_NOTE_Gb:
			return PSG_BASE_Gb;
		case PSG_NOTE_G:
			return PSG_BASE_G;
		case PSG_NOTE_Ab:
			return PSG_BASE_Ab;
		case PSG_NOTE_A:
			return PSG_BASE_A;
		case PSG_NOTE_Bb:
			return PSG_BASE_Bb;
		case PSG_NOTE_B:
			return PSG_BASE_B;
	}
	return PSG_BASE_C;
}

void psg_note(unsigned char chan, unsigned char note, unsigned char octave)
{
	unsigned int base = note_lookup(note);
	base = base << 1;
	base = base >> octave;
	psg_pitch(chan,base);
}
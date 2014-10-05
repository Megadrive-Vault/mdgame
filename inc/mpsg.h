#ifndef MPSG_H
#define MPSG_H

// Some very simple PSG manipulation functions
void psg_vol(unsigned char chan, unsigned char vol);
void psg_pitch(unsigned char chan, unsigned int pitch);
void psg_tone(unsigned char chan, unsigned char vol, unsigned int pitch);

#endif

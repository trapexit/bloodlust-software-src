/* лллллллл Copyright 1994 by Ethan Brodsky.  All rights reserved. лллллллл */

/* Support for Motorola's big-endian data format */

typedef unsigned char MOTOROLA_LONG[4];
typedef unsigned char MOTOROLA_TRIP[3];
typedef unsigned char MOTOROLA_WORD[2];

unsigned long get_motorola_long(MOTOROLA_LONG *ml)
  {
    return((*ml)[0]*0x1000000 + (*ml)[1]*0x10000 + (*ml)[2]*0x100 + (*ml)[3]);
  }

unsigned long get_motorola_trip(MOTOROLA_TRIP *mt)
  {
    return((*mt)[0]*0x10000 + (*mt)[1]*0x100 + (*mt)[2]);
  }

unsigned int  get_motorola_word(MOTOROLA_WORD *mw)
  {
    return((*mw)[0]*0x100 + (*mw)[1]);
  }

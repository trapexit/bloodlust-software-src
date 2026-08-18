#ifndef _GEN_
#define _GEN_

#include "g68k.h"
#include "gvdp.h"

extern class gmachine *gm;

//genesis machine
class gmachine
{
 public:
 byte romloaded;
 char romname[128]; //rom name
 int romsize; //size of rom in bytes
 byte *ROM; //system ROM
 byte *RAM; //system RAM

 //68000 cpu
 g68state gcpu;

 //genesis video display processor
 VDP vdp;

 //---------------------
 gmachine();
 ~gmachine();
 void reset();

 int  emulateframe(char *dest);

 int  loadrom(char *name);
 void freerom();

 //debug
 void writeram(char *fname);
};


#endif

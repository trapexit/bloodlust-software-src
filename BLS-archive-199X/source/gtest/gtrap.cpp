//G68 memory trap functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "types.h"

#include "gen.h"

//---------------------------------------------------
// IO read/write  A10000-A10FFF

byte IOreadb(dword a)
{
// printf("IOreadb[%08X]\n",a);
 switch (a)
 {
  case 0xA10001: return 0x20; //version
  case 0xA10003: return 0; //joystick 1
  case 0xA10005: return 0; //joystick 2              
//    printf("IOreadb[%08X]\n",a);

 }
 return 0;
}

void IOwriteb(dword a,byte d)
{
// printf("IOwriteb[%08X]=%02X\n",a,d);
 switch (a)
 {
  case 0xA10003: return; //joystick 1 flip/flop
  case 0xA10005: return; //joystick 2 flip/flop

  case 0xA10009: return;
  case 0xA1000B: return;
  case 0xA1000D: return;
// printf("IOwriteb[%08X]=%02X\n",a,d);
 }
}

//-----------------------------------------------
// Control read/write  A11000-A11FFF

byte Z80busreq;

dword CONTROLreadw(dword a)
{
// printf("CONTROLreadw[%08X]\n",a);
 return 0;
}

void CONTROLwritew(dword a,word d)
{
// printf("CONTROLwritew[%08X]=%08X\n",a,d);
 switch (a)
 {
  case 0xA11100: Z80busreq=(d&0x100) ? 1 : 0; break;
  case 0xA11200:  break; //Z80 reset (0)
 };
}


//------------------------------------------------
// Z80 read/write  A00000-A0FFFF

byte Z80readb(dword a)
{
// printf("Z80readb[%08X]\n",a);
 return 0;
}

void Z80writeb(dword a,byte d)
{
// printf("Z80writeb[%08X]=%02X\n",a,d);
}

word Z80readw(dword a)
{
 printf("Z80readw[%08X]\n",a);
 return 0;
}

void Z80writew(dword a,word d)
{
 printf("Z80writew[%08X]=%04X\n",a,d);
}


//--------------------------------------------------
//trap functions

byte g68_readb(dword a)
{
// printf("readb[%08X]\n",a);
 if (a==0xC00011) return 0;
 if (a>=0xC00000) return gm->vdp.readb(a);
 if (a>=0xA11000) return CONTROLreadw(a&(~1));
 if (a>=0xA10000) return IOreadb(a);
 if (a>=0xA00000) return Z80readb(a);
 return 0;
}
word g68_readw(dword a)
{
// printf("readw[%08X]\n",a);
 if (a>=0xC00000) return gm->vdp.readw(a&(~1));
 if (a>=0xA11000) return CONTROLreadw(a&(~1));
 if (a>=0xA10000) return (word)IOreadb(a|1);
 if (a>=0xA00000) return Z80readw(a&(~1));
 return 0;
}
dword g68_readd(dword a)
{
// printf("readd[%08X]\n",a);
 if (a>=0xC00000) return gm->vdp.readd(a&(~3));
 if (a>=0xA11000) return (CONTROLreadw(a) | (CONTROLreadw(a+2)<<16));
 if (a>=0xA10000) return (IOreadb(a|1)    | (IOreadb((a+2)|1)<<16));
 if (a>=0xA00000) return (Z80readw(a)     | (Z80readw(a+2)<<16));
 return 0;
}

//----------

void g68_writeb(byte d,dword a)
{
// printf("writeb[%08X]=%08X\n",a,d);
 if (a==0xC00011) return;
 if (a>=0xC00000) {gm->vdp.writeb(a,d); return;}
 if (a>=0xA11000) {CONTROLwritew(a&(~1),(word)d); return;}
 if (a>=0xA10000) {IOwriteb(a,d); return;}
 if (a>=0xA00000) {Z80writeb(a,d); return;}
}
void g68_writew(word d,dword a)
{
// printf("writew[%08X]=%08X\n",a,d);
 if (a>=0xC00000) {gm->vdp.writew(a,d); return;}
 if (a>=0xA11000) {CONTROLwritew(a,d); return;}
 if (a>=0xA10000) {IOwriteb(a|1,(byte)d); return;}
 if (a>=0xA00000) {Z80writew(a,d); return;}
}
void g68_writed(dword d,dword a)
{
// printf("writed[%08X]=%08X\n",a,d);
 if (a>=0xC00000) {gm->vdp.writed(a,d); return;}
 if (a>=0xA11000) {CONTROLwritew(a,(word)(d>>16)); CONTROLwritew(a,(word)d);return;}
 if (a>=0xA10000) {IOwriteb(a|1,(byte)(d>>16)); IOwriteb((a+2)|1,(byte)d); return;}
 if (a>=0xA00000) {Z80writew(a,(word)(d>>16)); Z80writew(a,(word)d); return;}
}







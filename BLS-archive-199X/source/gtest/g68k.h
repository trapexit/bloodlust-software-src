#ifndef _G68K_
#define _G68K_

extern "C" {
extern struct g68state *g68stateptr; //current state

extern dword g68_readbtrap,g68_readwtrap,g68_readdtrap;
extern dword g68_writebtrap,g68_writewtrap,g68_writedtrap;
extern dword g68disasmsize;

void __cdecl g68_reset(); //resets
int  __cdecl g68_execute(int numinst); //executes instructions
void __cdecl g68_init();
char *__cdecl g68_disasm();
void __cdecl g68_inttrigger(int vector);
};

#define G68FLAG_C 1
#define G68FLAG_V 2
#define G68FLAG_Z 4
#define G68FLAG_N 8
#define G68FLAG_X 16

//memory swap
word SWAPW(word x)
 {return ((x&0x00FF)<<8) | ((x&0xFF00)>>8);};

dword SWAPD(dword x)
 {return ((x&0x000000FF)<<24) | ((x&0x0000FF00)<<8) | ((x&0x00FF0000)>>8) | ((x&0xFF000000)>>24);};


//processor state
struct g68state
{
 dword Dn[8]; //data registers
 dword An[8]; //address registers

 byte *rom;
 byte *ram;
 dword PC;
 byte *pcbase;
 byte CCR;
 byte SR;

 dword breakpoint;

 void reset() {g68stateptr=this; g68_reset();}
 int execute(int numinst) {g68stateptr=this; return g68_execute(numinst);}
 void doint(int vector) {g68stateptr=this; g68_inttrigger(vector);}

 //debug functions
 int getflag(int f) {return *((word *)&CCR) & f;}
 void dumpreg();
 void dumpdisasm();
 void dumpflags();
 int disasm(char *s)
  {
   g68stateptr=this;
   if (s) strcpy(s,g68_disasm());
   return g68disasmsize;
  }
 void dumpinvalid();
 void debugkey(char key);
 byte  readb(dword d);
 word  readw(dword d);
 dword readd(dword d);
};

#endif
